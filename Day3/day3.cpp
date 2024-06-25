#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <tuple>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

multimap<int, int> potentialGears{};
multimap<int, tuple<int, int, int>> partNumbers{};

pair<size_t, size_t> findNumber(string_view str, size_t start) {
    auto firstDigit = str.find_first_of("123456789", start);
    if (firstDigit == string::npos) {
        return make_pair(string::npos, string::npos);
    }
    auto endDigit = firstDigit;
    while (endDigit < str.size() && isdigit(str[endDigit])) {
        ++endDigit;
    }

    if (endDigit == str.size()) {
        return make_pair(firstDigit, endDigit);
    }

    return {firstDigit, endDigit - 1};
}
//* Func name isn't descriptive but I think it's fun (check challange info if you need explanation)
bool isValidEnginePart(string_view prevLine, string_view currLine, string_view nextLine, size_t numStart, size_t numEnd) {
    auto nonSymbolsBank = ".0123456789";
    for (size_t i = numStart; i <= numEnd; ++i) {
        auto checkSymbol = [&](string_view line) {
            if (line.empty()) return false;

            size_t begin = i == 0 ? i : i - 1;
            size_t end = i >= line.size() - 1 ? i : i + 1;
            
            return line.substr(begin, end - begin + 1).find_first_not_of(nonSymbolsBank) != string::npos;
        };
        if (checkSymbol(prevLine) || checkSymbol(currLine) || checkSymbol(nextLine)) {
            return true;
        }
    }
    return false;
}

int getValidEnginePartsSumForCurrLine(string_view prevLine, string_view currLine, string_view nextLine, int row) {
    int enginePartsSum = 0;
    for (auto [start, end] = findNumber(currLine, 0); end != string::npos; 
            tie(start, end) = findNumber(currLine, end + 1))
        {
            auto num = stoi(string(currLine.substr(start, end - start + 1)));
            
            if (isValidEnginePart(prevLine, currLine, nextLine, start, end)) {
                partNumbers.insert({row, {start, end, num}});
                enginePartsSum += num;
            }
        }
    return enginePartsSum;
}
void findAllPotentialGears(string_view str, int row) {
    size_t pos = str.find('*', 0);

    while (pos != std::string::npos) {
        potentialGears.insert({row, pos});
        pos = str.find('*', pos + 1);
    }
}

void calcEnginePartsSum() {
    ifstream inputFile("input.txt");
    if (!inputFile.is_open())
    {
        cerr << "Error opening input file." << endl;
        return;
    }
    int enginePartsSum = 0;
    int lineNum = 0;
    string prevLine = "", currLine = "", nextLine = "";
    getline(inputFile, currLine);
    while (getline(inputFile, nextLine))
    {
        enginePartsSum += getValidEnginePartsSumForCurrLine(prevLine, currLine, nextLine, lineNum);
        findAllPotentialGears(currLine, lineNum);
        prevLine = currLine;
        currLine = nextLine;
        ++lineNum;
    }
    //! CHECK LAST LINE - CLASSIC MISTAKE
    enginePartsSum += getValidEnginePartsSumForCurrLine(prevLine, currLine, "", lineNum);
    
    cout << "Sum of valid engine part numbers: " << enginePartsSum << endl;
    inputFile.close();
}

void calcGearRatiosSum() {
    int totalSum = 0;
    for_each(potentialGears.begin(), potentialGears.end(), [&totalSum](const pair<int, size_t>& p) {
        auto gearRow = p.first;
        auto gearIdx = p.second;
        vector<int> adjacentPartNumbers;
        for (const auto& [partRow, location] : partNumbers) {
            auto inBounds = [](int value, int low, int high) {
                return !(value < low) && (value <= high);
            };
            if (inBounds(gearRow, partRow - 1, partRow + 1)) {
                if (inBounds(gearIdx, get<0>(location) - 1, get<1>(location) + 1)) {
                    adjacentPartNumbers.push_back(get<2>(location));
                }
            }
        }
        if (adjacentPartNumbers.size() == 2) {
            // cout << "Gear " << gearRow << " ratio: " << adjacentPartNumbers[0] * adjacentPartNumbers[1] << endl;
            totalSum += adjacentPartNumbers[0] * adjacentPartNumbers[1];
        }
        adjacentPartNumbers.clear();
    });

    cout << "Sum of valid gear ratios: " << totalSum << endl;
}

int main() {
    calcEnginePartsSum(); 
    
    calcGearRatiosSum();
    return 0;
}