#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <locale.h>

using namespace std;

// Смещения для движения (вверх, вниз, влево, вправо)
const int dr[] = { -1, 1, 0, 0 };
const int dc[] = { 0, 0, -1, 1 };

// Структура для хранения координат
struct Point {
    int row, col;
};

// Функция для загрузки лабиринта из фиксированного файла
vector<vector<int>> loadMazeFromFixedFile() {
    vector<vector<int>> maze;

    
    string filename = "C:\\Users\\Аня\\Desktop\\maze.txt";
   

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return maze;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<int> row;
        int cell;
        while (ss >> cell) {
            row.push_back(cell);
        }
        maze.push_back(row);
    }

    file.close();
    return maze;
}

// Генерация случайного лабиринта 
vector<vector<int>> generateMaze(int rows, int cols) {
    vector<vector<int>> maze(rows, vector<int>(cols, 1)); // 1 - стена, 0 - проход
    srand(static_cast<unsigned>(time(0))); // Инициализация случайного числа

    for (int r = 1; r < rows - 1; r++) {
        for (int c = 1; c < cols - 1; c++) {
            maze[r][c] = rand() % 2;
        }
    }

    // Убедимся, что стартовая и конечная точки проходимы
    maze[0][0] = 0; // Начальная точка
    maze[1][0] = 0; // Начальная точка
    maze[rows - 1][cols - 1] = 0; // Конечная точка
    maze[rows - 1][cols - 2] = 0; // Конечная точка

    return maze;
}

// Проверка, что точка находится внутри лабиринта и проходима
bool isValid(int row, int col, const vector<vector<int>>& maze, const vector<vector<bool>>& visited) {
    return row >= 0 && row < maze.size() && col >= 0 && col < maze[0].size() &&
        maze[row][col] == 0 && !visited[row][col];
}

// Алгоритм поиска кратчайшего пути 
vector<Point> findShortestPath(const vector<vector<int>>& maze) {
    int rows = maze.size();
    int cols = maze[0].size();

    queue<pair<Point, vector<Point>>> q; // Очередь, храним точку и текущий путь
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    // Начальная точка
    Point start = { 0, 0 };
    q.push({ start, {start} });
    visited[0][0] = true;

    while (!q.empty()) {
        auto currentPair = q.front();
        Point current = currentPair.first;
        vector<Point> path = currentPair.second;
        q.pop();

        // Если достигли выхода
        if (current.row == rows - 1 && current.col == cols - 1) {
            return path;
        }

        // Пробуем двигаться в 4 направлениях
        for (int i = 0; i < 4; i++) {
            int newRow = current.row + dr[i];
            int newCol = current.col + dc[i];

            if (isValid(newRow, newCol, maze, visited)) {
                visited[newRow][newCol] = true;
                vector<Point> newPath = path;
                newPath.push_back({ newRow, newCol });
                q.push({ {newRow, newCol}, newPath });
            }
        }
    }

    return {};
}

// Функция для добавления зигзага от начала до конца
void addZigZagPath(vector<vector<int>>& maze) {
    int rows = maze.size();
    int cols = maze[0].size();

    int r = 0, c = 0;
    while (r < rows - 1 || c < cols - 1) {
        // Двигаемся вниз, если это возможно
        if (r < rows - 1) {
            maze[r][c] = 0;
            r++;
        }
        // Двигаемся вправо, если это возможно
        if (c < cols - 1) {
            maze[r][c] = 0;
            c++;
        }
    }
    // Убедимся, что конечная точка тоже проходима
    maze[rows - 1][cols - 1] = 0;
}

// Вывод лабиринта
void printMaze(const vector<vector<int>>& maze, const vector<Point>& path = {}) {
    vector<vector<char>> displayMaze(maze.size(), vector<char>(maze[0].size(), ' '));

    // Заполняем лабиринт символами стены или прохода
    for (int r = 0; r < maze.size(); ++r) {
        for (int c = 0; c < maze[0].size(); ++c) {
            displayMaze[r][c] = (maze[r][c] == 1) ? '#' : '.';
        }
    }

    // Помечаем путь символом '*'
    for (const auto& point : path) {
        displayMaze[point.row][point.col] = '*';
    }

    // Выводим лабиринт
    for (const auto& row : displayMaze) {
        for (char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

// Вывод пути
void printPath(const vector<Point>& path) {
    if (path.empty()) {
        cout << "Путь не найден!" << endl;
    }
    else {
        cout << "Кратчайший путь:" << endl;
        for (const auto& point : path) {
            cout << "(" << point.row << ", " << point.col << ") ";
        }
        cout << endl;
    }
}

// Главная функция
int main() {
    setlocale(LC_ALL, "rus");

    int choice;
    cout << "Выберите вариант:\n";
    cout << "1. Загрузить лабиринт из файла\n";
    cout << "2. Сгенерировать новый лабиринт\n";
    cout << "Ваш выбор: ";
    cin >> choice;

    vector<vector<int>> maze;

    if (choice == 1) {
        // Загрузка лабиринта из файла
        maze = loadMazeFromFixedFile();
        if (maze.empty()) {
            cout << "Не удалось загрузить лабиринт. Завершение программы." << endl;
            return 1;
        }
    }
    else if (choice == 2) {
        int rows, cols;
        cout << "Введите количество строк лабиринта: ";
        cin >> rows;
        cout << "Введите количество столбцов лабиринта: ";
        cin >> cols;

        // Генерация лабиринта
        maze = generateMaze(rows, cols);
    }
    else {
        cout << "Некорректный выбор. Завершение программы." << endl;
        return 1;
    }

    cout << "Лабиринт:" << endl;
    printMaze(maze);

    // Поиск кратчайшего пути
    vector<Point> path = findShortestPath(maze);

    // Если путь не найден, добавляем зигзаг
    if (path.empty()) {
        cout << "Путь не найден, добавляем зигзаг" << endl;
        addZigZagPath(maze);

        cout << "Лабиринт после добавления зигзага:" << endl;
        printMaze(maze);

        // Повторный поиск пути
        path = findShortestPath(maze);
    }

    // Вывод результата с путем
    printPath(path);
    cout << "Лабиринт с путем:" << endl;
    printMaze(maze, path);

    return 0;
}
