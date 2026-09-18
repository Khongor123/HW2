#include "TimeCode.h"
#include <stdexcept> // provides "invalid_argument"
#include <string>

// hr/min/sec -> t
long long unsigned int TimeCode::ComponentsToSeconds(unsigned int hr, unsigned int min, unsigned long long int sec)
{
    // cast before multiplying so hr * 3600 doesn't overflow
    return static_cast<unsigned long long>(hr) * 3600 + static_cast<unsigned long long>(min) * 60 + sec;
}

// inverse of ComponentsToSeconds()
void TimeCode::GetComponents(unsigned int& hr, unsigned int& min, unsigned int& sec) const
{
    hr = static_cast<unsigned int>(t / 3600);
    unsigned long long remaining = t % 3600;
    min = static_cast<unsigned int>(remaining / 60);
    sec = static_cast<unsigned int>(remaining % 60);
}

// rolls over instead of rejecting. e.g. TimeCode(2, 71, 3801) is valid.
TimeCode::TimeCode(unsigned int hr, unsigned int min, long long unsigned int sec)
{
    t = ComponentsToSeconds(hr, min, sec);
}

// copying t alone gives a fully independent copy
TimeCode::TimeCode(const TimeCode& tc)
{
    t = tc.GetTimeCodeAsSeconds();
}

unsigned int TimeCode::GetHours() const
{
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec);
    return hr;
}

unsigned int TimeCode::GetMinutes() const
{
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec);
    return min;
}

unsigned int TimeCode::GetSeconds() const
{
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec);
    return sec;
}


void TimeCode::SetHours(unsigned int hours)
{
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec);

    t = ComponentsToSeconds(hours, min, sec);
}

// setters reject out-of-range input instead of rolling it over.
void TimeCode::SetMinutes(unsigned int minutes)
{
    if (minutes > 59)
    {
        throw invalid_argument("Minutes should be between 0 adn 59");
    }

    unsigned int hr, min, sec;
    GetComponents(hr, min, sec);

    t = ComponentsToSeconds(hr, minutes, sec);
}

void TimeCode::SetSeconds(unsigned int seconds)
{
    if (seconds > 59)
    {
        throw invalid_argument("Seconds should be between 0 and 59");
    }

    unsigned int hr, min, sec;
    GetComponents(hr, min, sec);

    t = ComponentsToSeconds(hr, min, seconds);
}

void TimeCode::reset()
{
    t = 0;
}

string TimeCode::ToString() const
{
    unsigned int hr, min, sec;
    GetComponents(hr, min, sec);

    return to_string(hr) + ":" + to_string(min) + ":" + to_string(sec);
}

// adding raw t values sidesteps roll-over logic
TimeCode TimeCode::operator+(const TimeCode& other) const
{
    unsigned long long total = GetTimeCodeAsSeconds() + other.GetTimeCodeAsSeconds();

    return TimeCode(0, 0, total);
}

// t is unsigned, so an out-of-range subtraction would wrap instead of going negative.
TimeCode TimeCode::operator-(const TimeCode& other) const
{
    if (other.GetTimeCodeAsSeconds() > GetTimeCodeAsSeconds())
    {
        throw invalid_argument("Timecode cannot be negative");
    }

    return TimeCode(0, 0, GetTimeCodeAsSeconds() - other.GetTimeCodeAsSeconds());
}

TimeCode TimeCode::operator*(double a) const
{
    if (a < 0) {
        throw invalid_argument("Negative multiplier not allowed");
    }

    unsigned long long total =
        static_cast<unsigned long long>(GetTimeCodeAsSeconds() * a);

    return TimeCode(0, 0, total);
}

// divisor <= 0 is meaningless or produces negative result
TimeCode TimeCode::operator/(double a) const
{
    if (a <= 0) {
        throw invalid_argument("Divisor must be greater than zero");
    }

    unsigned long long total =
        static_cast<unsigned long long>(GetTimeCodeAsSeconds() / a);

    return TimeCode(0, 0, total);
}

bool TimeCode::operator==(const TimeCode& other) const
{
    return GetTimeCodeAsSeconds() == other.GetTimeCodeAsSeconds();
}

bool TimeCode::operator!=(const TimeCode& other) const
{
    return GetTimeCodeAsSeconds() != other.GetTimeCodeAsSeconds();
}

bool TimeCode::operator<(const TimeCode& other) const
{
    return GetTimeCodeAsSeconds() < other.GetTimeCodeAsSeconds();
}

bool TimeCode::operator<=(const TimeCode& other) const
{
    return GetTimeCodeAsSeconds() <= other.GetTimeCodeAsSeconds();
}

bool TimeCode::operator>(const TimeCode& other) const
{
    return GetTimeCodeAsSeconds() > other.GetTimeCodeAsSeconds();
}

bool TimeCode::operator>=(const TimeCode& other) const
{
    return GetTimeCodeAsSeconds() >= other.GetTimeCodeAsSeconds();
}