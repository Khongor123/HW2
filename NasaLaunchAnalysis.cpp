#include "TimeCode.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

// keep quoted values intact because csv fields can contain commas inside them
vector<string> split_csv_line(const string& line) {
    vector<string> fields;
    string current;
    bool insideQuotes = false;

    for (char c : line) {
        if (c == '"') {
            insideQuotes = !insideQuotes;
        } else if (c == ',' && !insideQuotes) {
            fields.push_back(current);
            current = "";
        } else {
            current += c;
        }

    }
    fields.push_back(current);

    return fields;
}

// extracts launch time from the Datum field and returns true if an exact time was found. False otherwise.
bool parse_time(const string& datum, TimeCode& tc) {
    // launch times end with UTC
    vector<string> parts;
    stringstream ss(datum);
    string word;

    while(ss >> word) {
        parts.push_back(word);
    }

    if (parts.size() < 6) {
        return false;
    }

    string timeString = parts[4];

    unsigned int hour = stoi(timeString.substr(0, 2));
    unsigned int minute = stoi(timeString.substr(3, 2));

    tc = TimeCode(hour, minute, 0);

    return true;
}

int main() {
    ifstream inputFile("Space_Corrected.csv");

    if (!inputFile.is_open()) {
        cout << "Could not open Space_Corrected.csv" << endl;
        return 1;
    }

    vector<TimeCode> launchTimes;

    string line;

    // skip header row
    getline(inputFile, line);

    while (getline(inputFile, line))
    {
        vector<string> fields = split_csv_line(line);

        // Datum is the fourth column
        if (fields.size() < 4)
        {
            continue;
        }

        string datum = fields[3];

        TimeCode launchTime;

        // ignore the launches that have the date but no time
        if (parse_time(datum, launchTime))
        {
            launchTimes.push_back(launchTime);
        }
    }

    inputFile.close();

    TimeCode total;

    // add every valid launch time so the average can be computed from the sum
    for (const TimeCode& launchTime : launchTimes)
    {
        total = total + launchTime;
    }

    TimeCode average;

    // avoid dividing by zero when there are no valid launch times in the file
    if (!launchTimes.empty())
    {
        average = total / static_cast<double>(launchTimes.size());
    }

    cout << launchTimes.size() << " data points." << endl;
    cout << "avg: " << average.ToString() << endl;

    return 0;
}