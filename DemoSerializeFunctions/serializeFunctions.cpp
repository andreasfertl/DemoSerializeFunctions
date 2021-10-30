#include "serializeFunctions.h"
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>

class seriealizeFunctionsImpl {

public:
	seriealizeFunctionsImpl() :
		m_Stop(false),
		m_Queue(),
		m_Event(),
		m_RunThread(&seriealizeFunctionsImpl::handler, this)
	{
	}

	~seriealizeFunctionsImpl()
	{
		run([this]() { this->m_Stop = true; });
		m_RunThread.join();
	}

	void run(const std::function<void()>& func)
	{
		{
			std::lock_guard<std::mutex> lk(m_Event.m_Mutex);
			m_Queue.push(func);
		}
		m_Event.m_Condition.notify_one();
	}

private:
	struct Event {
		std::mutex m_Mutex;
		std::condition_variable m_Condition;
	};

	void handler()
	{
		while (!m_Stop)
		{
			std::queue<std::function<void()>> handlerQueue;

			{
				std::unique_lock<std::mutex> lk(m_Event.m_Mutex);
				m_Event.m_Condition.wait(lk, [this]() { return !this->m_Queue.empty(); });
				//just "swap" the workload to another queue to be able to hold the lock as little as possible
				m_Queue.swap(handlerQueue);
			}

			//and finally call all the functions to be handled
			while (!handlerQueue.empty())
			{
				auto& callFunction = handlerQueue.front();
				callFunction();
				handlerQueue.pop();
			}

		}
	}

private:
	bool m_Stop;
	std::queue<std::function<void()>> m_Queue;
	Event m_Event;
	std::thread m_RunThread;
};



//------------------------------------------------------------------------------------------------------------------------

seriealizeFunctions::seriealizeFunctions() :
	m_SeriealizeFunctionsImpl(std::make_unique<seriealizeFunctionsImpl>())
{
}

seriealizeFunctions::~seriealizeFunctions()
{
}

void seriealizeFunctions::run(const std::function<void()>& func)
{
	m_SeriealizeFunctionsImpl->run(func);
}
