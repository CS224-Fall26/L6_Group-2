#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP
#include <vector>
#include <string>
#include <ostream>
#include "ScoreRecord.hpp"

using namespace std;


class ScoreBoard {
    private:
        vector<ScoreRecord> records;
        int maxEntries;
        string filePath;

    public:
        ScoreBoard(const string& filePath, int maxEntries);
        void addScore(const string& name, int score);
        bool load();
        bool save() const;
        const vector<ScoreRecord>& getRecords() const;
        friend ostream& operator<<(ostream& os, const ScoreBoard& b);
};

#endif
