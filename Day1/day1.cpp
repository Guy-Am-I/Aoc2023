/**
Link to problem: https://adventofcode.com/2023/day/1#part2
*/
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <string_view>
#include <map>

using namespace std::string_view_literals;

int main() {
    //parse input.txt file line by line
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    int sum = 0;
    std::string line;
    while (std::getline(inputFile, line)) {
        //! Part 1
        /*
        auto firstDigit = std::find_if(line.begin(), line.end(), [](char c) { return std::isdigit(c); });
        auto lastDigit = std::find_if(line.rbegin(), line.rend(), [](char c) { return std::isdigit(c); });
        sum += std::stoi(std::string{*firstDigit} + *lastDigit);
        */
       //! Part 2
       //digits can be spelled out, i.e. "one", "two", "three", "four", "five", "six" or just a digit
       // can use find and rfind for each number (both spelled out and literal) hold in map 
       // then simply choose smallest & largest valid indicies
        std::map<std::string_view, std::pair<int, int> > digitsPos = {
        {"one", {-1,-1}},
        {"two", {-1,-1}},
        {"three", {-1,-1}},
        {"four", {-1,-1}},
        {"five", {-1,-1}},
        {"six", {-1,-1}},
        {"seven", {-1,-1}},
        {"eight", {-1,-1}},
        {"nine", {-1,-1}},
        {"1", {-1,-1}},
        {"2", {-1,-1}},
        {"3", {-1,-1}},
        {"4", {-1,-1}},
        {"5", {-1,-1}},
        {"6", {-1,-1}},
        {"7", {-1,-1}},
        {"8", {-1,-1}},
        {"9", {-1,-1}}
        };

        for (auto& [key, value] : digitsPos) {
            auto firstPos = line.find(key);
            auto lastPos = line.rfind(key);
            value.first = firstPos == std::string::npos ? -1 : firstPos;
            value.second = lastPos == std::string::npos ? -1 : lastPos;
        }

        //get the map element with smallest and largest valid index
        auto smallest = std::min_element(digitsPos.begin(), digitsPos.end(), [](const auto& a, const auto& b) {
            return a.second.first >= 0 && (b.second.first < 0 || a.second.first < b.second.first);
        });

        auto largest = std::max_element(digitsPos.begin(), digitsPos.end(), [](const auto& a, const auto& b) {
            return a.second.second < b.second.second;
        });

        //convert elements to int digits (if "one" or one -> 1, "two" or two -> 2, etc.)
        auto firstDigit = smallest->first;
        auto lastDigit = largest->first;
        std::map<std::string_view, std::string> spelledOutDigits = {
            {"one", "1"},
            {"two", "2"},
            {"three", "3"},
            {"four", "4"},
            {"five", "5"},
            {"six", "6"},
            {"seven", "7"},
            {"eight", "8"},
            {"nine", "9"},
            {"1", "1"},
            {"2", "2"},
            {"3", "3"},
            {"4", "4"},
            {"5", "5"},
            {"6", "6"},
            {"7", "7"},
            {"8", "8"},
            {"9", "9"}
        };
        
        auto num = spelledOutDigits[firstDigit] + spelledOutDigits[lastDigit];
        
        sum += std::stoi(num);
    }

    inputFile.close();

    std::cout << "Sum of all calibration values: " << sum << std::endl;

    return 0;
}
 