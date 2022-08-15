#pragma once
#include <string>
#include <iostream>
#include <time.h>
#include <vector>

#define DATATYPE float

class Timer
{
public:
	Timer() : time(DATATYPE(clock())) {};
	~Timer() {};
	/**
	 * Restart the timer.
	 */
	void restart()
	{
		time = DATATYPE(clock());
	}
	/**
	 * Measures elapsed time.
	 *
	 * @return The elapsed time
	 */
	DATATYPE elapsed()
	{
		return (DATATYPE(clock()) - time) / CLOCKS_PER_SEC;
	}
private:
	DATATYPE time;
};

class progress_display
{
public:
	explicit progress_display(
		unsigned long expected_count,
		std::ostream& os = std::cout,
		const std::string& s1 = "\n",
		const std::string& s2 = "",
		const std::string& s3 = "")
		: m_os(os), m_s1(s1), m_s2(s2), m_s3(s3)
	{
		restart(expected_count);
	}
	void restart(unsigned long expected_count)
	{
		_count = _next_tic_count = _tic = 0;
		_expected_count = expected_count;
		m_os << m_s1 << "0%   10   20   30   40   50   60   70   80   90   100%\n"
			<< m_s2 << "|----|----|----|----|----|----|----|----|----|----|"
			<< std::endl
			<< m_s3;
		if (!_expected_count)
		{
			_expected_count = 1;
		}
	}
	unsigned long operator += (unsigned long increment)
	{
		if ((_count += increment) >= _next_tic_count)
		{
			display_tic();
		}
		return _count;
	}
	unsigned long  operator ++ ()
	{
		return operator += (1);
	}
	unsigned long count() const
	{
		return _count;
	}
	unsigned long expected_count() const
	{
		return _expected_count;
	}
private:
	std::ostream& m_os;
	const std::string m_s1;
	const std::string m_s2;
	const std::string m_s3;
	unsigned long _count, _expected_count, _next_tic_count;
	unsigned _tic;
	void display_tic()
	{
		unsigned tics_needed = unsigned((DATATYPE(_count) / _expected_count) * 50.0);
		do
		{
			m_os << '*' << std::flush;
		} while (++_tic < tics_needed);
		_next_tic_count = unsigned((_tic / 50.0) * _expected_count);
		if (_count == _expected_count)
		{
			if (_tic < 51) m_os << '*';
			m_os << std::endl;
		}
	}
};

