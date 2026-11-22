#ifndef SCORERECORD_HPP
#define SCORERECORD_HPP
#include <string>

using namespace std;


class ScoreRecord {
    private:
        string playerName;
        int score;
        string date;

    public:
        ScoreRecord(const string& name, int score, const string& date);
        bool operator>(const ScoreRecord& other) const;
        string toCsv() const;
        static ScoreRecord fromCsv(const string& line);
};

#endif
