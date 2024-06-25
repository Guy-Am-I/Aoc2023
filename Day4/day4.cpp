#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <map>
#include <algorithm>
using namespace std;

using numbers_bank = unordered_set<int>;

pair<numbers_bank, numbers_bank> parseDataFromLine(string_view gameLine) {
    auto startPos = gameLine.find(':');
    auto delimPos = gameLine.find('|');
    auto winningNumbers = gameLine.substr(startPos + 1, delimPos - startPos - 1);

    auto myNumbersStr = gameLine.substr(delimPos + 1);
    
    auto extractNumbersFromStr = [](string_view str) -> numbers_bank {
        numbers_bank numbers;
        auto delim{" "};
        string::size_type prev = str.find_first_not_of(delim, 0);
        string::size_type next = 0; 
        while ((next = str.find(" ", prev)) != string::npos) 
        {   
            auto num = stoi(string(str.substr(prev, next-prev)));
            numbers.insert(num);
            prev = str.find_first_not_of(delim, next); 
        } 
        if (prev!= string::npos) { 
            auto num = stoi(string(str.substr(prev)));
            numbers.insert(num);
        }
        return numbers;
    };

    return make_pair(extractNumbersFromStr(winningNumbers), extractNumbersFromStr(myNumbersStr));
}

int countWinningNumbers(string_view cardLine) {
    int matchingNumbers = 0;
    auto [winningNumbers, myNumbers] = parseDataFromLine(cardLine);
    for (const auto& num : myNumbers) {
        if (winningNumbers.contains(num)) {
            ++matchingNumbers;
        }
    }
    return matchingNumbers;
}
void calcTotalPoints() {
    /**
     *  Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
        Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
        Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
        Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
        Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
        Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11
     */
    ifstream inputFile {"input.txt"};
    if (!inputFile.is_open()) {
        cerr << "Error opening input file." << endl;
        return;
    }
    // !Part1 -> int totalPoints {0};
     // count the newlines with an algorithm specialized for counting:
    ;
    
    auto numCards = count(istreambuf_iterator<char>(inputFile), istreambuf_iterator<char>(), '\n') + 1; //card nums start at 1
    inputFile.clear();
    inputFile.seekg(ios::beg);
    map<int, int> cardInstances;
    for (int i = 1; i <= numCards; ++i) {
        cardInstances[i] = 1;
    }

    int cardNum = 1;
    string cardLine;
    while (getline(inputFile, cardLine)) {
        //! Part2 
        // update next card #instances accordingly 
        // (iterating over keys so modyfying values is fine)
        auto numMatches = countWinningNumbers(cardLine);
        
        //update card instances
        for (int copyCardNum = cardNum + 1; copyCardNum <= cardNum + numMatches; ++copyCardNum) {
            //safely assume cards won't make copy of cardNum out of range
            cardInstances[copyCardNum] += cardInstances[cardNum];
        }
        // !Part1: uint points = numMatches == 0 ? 0 : 0 ^ ((uint)1 << (numMatches - 1));
        // totalPoints += points;

        ++cardNum;
    }
    inputFile.close();
    //cout << "Total points: " << totalPoints << endl;
    auto totalCards = reduce(cardInstances.begin(), cardInstances.end(), 0, [](auto sum, const auto& mapVal) {
        return sum + mapVal.second;
    });
    cout << "Total cards: " << totalCards << endl;
    return;
}

int main() {
    calcTotalPoints();
    return 0;
}