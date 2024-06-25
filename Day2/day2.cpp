#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
using namespace std;

constexpr int maxRedCubes = 12, maxBlueCubes = 14, maxGreenCubes = 13;
tuple<int, int, int> getCubes(string_view roll)
{
    int red = 0, blue = 0, green = 0;
    auto setColor = [&](string_view cube) {
        auto spacePos = cube.rfind(" ");
        
        auto num = cube[0] == ' ' ? stoi(string(cube.substr(1, spacePos))) : stoi(string(cube.substr(0, spacePos)));
        auto color = cube.substr(spacePos + 1);
        if (color == "red")
        {
            red = num;
        }
        else if (color == "blue")
        {
            blue = num;
        }
        else if (color == "green")
        {
            green = num;
        }
    };
    size_t start = 0, end = 0;
    while ((end = roll.find(", ", start)) != string::npos)
    {
        auto cube = roll.substr(start, end - start); /// 2 blue, 10 red, 4 green
        setColor(cube);
        start = end + 1;
    }
    auto lastCube = roll.substr(start);
    setColor(lastCube);
    return make_tuple(red, blue, green);
}
bool isRollValid(string_view roll)
{
    auto [red, blue, green] = getCubes(roll);
    // cout << "Roll: " << roll << endl;
    // cout << "Red: " << red << ", Blue: " << blue << ", Green: " << green << endl;
    if (red > maxRedCubes || blue > maxBlueCubes || green > maxGreenCubes) {
        return false;
    }
    return true;
}

void determinePossibleGames()
{
    ifstream inputFile("input.txt");
    if (!inputFile.is_open())
    {
        cerr << "Error opening input file." << endl;
        return;
    }

    //! Part 1 -> int idSums(0);
    int cubePowersSum(0);

    string line;
    while (getline(inputFile, line))
    {
        //! Part 1 -> bool isValidGame = true;
        // min needed Red/Blue/Green cubes for game
        int minRed = 0, minBlue = 0, minGreen = 0;
        // line format: Game <id>: <red> red, <blue> blue, <green> green; <red> redd, ....
        string_view currGame{line};
        auto start = line.find(":");
        int id = stoi(line.substr((line.find("Game ") + 5), start));
        auto end = string::npos;
        while ((end = line.find(";", start)) != string::npos)
        {
            auto currRoll = currGame.substr(start + 1, end - start - 1);
            ///! Part 1 -> if (!isRollValid(currRoll)) isValidGame = false;
            auto [red, blue, green] = getCubes(currRoll);
            if (red > minRed) minRed = red; if (blue > minBlue) minBlue = blue; if (green > minGreen) minGreen = green;
            start = end + 1;
        }
        // last roll
        auto lastRoll = currGame.substr(start);
        ///! Part 1 -> if (!isRollValid(lastRoll)) isValidGame = false;
        auto [red, blue, green] = getCubes(lastRoll);
        if (red > minRed) minRed = red; if (blue > minBlue) minBlue = blue; if (green > minGreen) minGreen = green;
        ///! Part 1 -> if (isValidGame) idSums += id;
        cubePowersSum += minRed * minBlue * minGreen;
    }

    inputFile.close();
    //! Part 1 -> cout << "Sum of IDs of valid games: " << idSums << endl;
    cout << "Sum of cube powers of the minimum dice rolls in valid games: " << cubePowersSum << endl;
    return;
}

int main()
{
    determinePossibleGames();
    return 0;
}