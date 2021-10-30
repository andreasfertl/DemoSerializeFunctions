#pragma once
#include <memory>
#include <functional>

//implements a function queue based on standard c++ which handles functions in sequence
//so that you can protect concurrent data access by running functions in sequence 
//e.g. think about a logfile which is globally used and several threads will post logs
class seriealizeFunctions
{
public:
	seriealizeFunctions();
	~seriealizeFunctions();

	void run(const std::function<void()>& func);

private:
	std::unique_ptr<class seriealizeFunctionsImpl> m_SeriealizeFunctionsImpl;
};



