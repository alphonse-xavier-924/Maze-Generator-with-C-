#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <stack>

using namespace std;

class Cell {
public:
    int i, j;
    bool walls[4] = { true, true, true, true };
    bool visited = false;

    Cell(int i, int j) {
        this->i = i;
        this->j = j;
    }

    int index(int i, int j, int cols, int rows) {
        if (i < 0 || j < 0 || i >= cols || j >= rows) {
            return -1;
        }
        return i + j * cols;
    }

    int checkNeighbors(vector<Cell>& grid, int cols, int rows) {
        vector<Cell*> neighbors;

        int top = index(i, j - 1, cols, rows);
        int right = index(i + 1, j, cols, rows);
        int bottom = index(i, j + 1, cols, rows);
        int left = index(i - 1, j, cols, rows);

        if (top != -1 && !grid[top].visited) {
            neighbors.push_back(&grid[top]);
        }
        if (right != -1 && !grid[right].visited) {
            neighbors.push_back(&grid[right]);
        }
        if (bottom != -1 && !grid[bottom].visited) {
            neighbors.push_back(&grid[bottom]);
        }
        if (left != -1 && !grid[left].visited) {
            neighbors.push_back(&grid[left]);
        }

        if (!neighbors.empty()) {
            int r = rand() % neighbors.size();
            return neighbors[r] - &grid[0]; // Return the index of the chosen neighbor
        }
        else {
            return -1;
        }
    }

    void show(sf::RenderWindow& window, int w) {
        int x = this->i * w;
        int y = this->j * w;

        if (walls[0]) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, y)),
                sf::Vertex(sf::Vector2f(x + w, y))
            };
            window.draw(line, 2, sf::Lines);
        }
        if (walls[1]) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x + w, y)),
                sf::Vertex(sf::Vector2f(x + w, y + w))
            };
            window.draw(line, 2, sf::Lines);
        }
        if (walls[2]) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x + w, y + w)),
                sf::Vertex(sf::Vector2f(x, y + w))
            };
            window.draw(line, 2, sf::Lines);
        }
        if (walls[3]) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, y + w)),
                sf::Vertex(sf::Vector2f(x, y))
            };
            window.draw(line, 2, sf::Lines);
        }

        if (visited) {
            sf::RectangleShape rectangle(sf::Vector2f(w, w));
            rectangle.setPosition(x, y);
            rectangle.setFillColor(sf::Color(0, 0, 255, 100));
            window.draw(rectangle);
        }
    }

    void highlight(sf::RenderWindow& window, int w) {
        int x = this->i * w;
        int y = this->j * w;

        sf::RectangleShape rectangle(sf::Vector2f(w, w));
        rectangle.setPosition(x, y);
        rectangle.setFillColor(sf::Color(0, 255, 0, 100)); // Green for highlight
        window.draw(rectangle);
    }
};

void removeWalls(Cell& a, Cell& b) {
    int x = a.i - b.i;
    if (x == 1) {
        a.walls[3] = false;
        b.walls[1] = false;
    }
    else if (x == -1) {
        a.walls[1] = false;
        b.walls[3] = false;
    }
    int y = a.j - b.j;
    if (y == 1) {
        a.walls[0] = false;
        b.walls[2] = false;
    }
    else if (y == -1) {
        a.walls[2] = false;
        b.walls[0] = false;
    }
}

// Global variables
int w = 20;
vector<Cell> grid;
int modeWidth = 400;
int modeHeight = 400;
int cols = modeWidth / w;
int rows = modeHeight / w;
Cell* current;
stack<Cell*> stackCells;

int main() {
    sf::RenderWindow window(sf::VideoMode(modeWidth, modeHeight), "Maze Generator");
    window.setFramerateLimit(40); // Set frame rate

    // Create grid of cells
    for (int j = 0; j < rows; j++) {
        for (int i = 0; i < cols; i++) {
            grid.emplace_back(i, j);
        }
    }

    current = &grid[0];

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black); // Background color

        for (auto& cell : grid) {
            cell.show(window, w);
        }

        current->visited = true;
        current->highlight(window, w);

        int nextIndex = current->checkNeighbors(grid, cols, rows);
        if (nextIndex != -1) {
            Cell* next = &grid[nextIndex];
            next->visited = true;

            stackCells.push(current);
            removeWalls(*current, *next);

            current = next;
        }
        else if (!stackCells.empty()) {
            current = stackCells.top();
            stackCells.pop();
        }

        window.display();
    }

    return 0;
}
