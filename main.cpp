#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <random>


#define WINDOW_HEIGHT 750
#define WINDOW_WIDTH (WINDOW_HEIGHT*2+1)
#define HEIGHT 200
#define WIDTH (HEIGHT*2+1)
#define RANDOMIZE false
#define RULESET 182


// Convert the ruleset to a boolean array
bool ruleset[8];
void convertRuleset() {
    for (int i = 0; i < 8; i++) {
        ruleset[i] = (RULESET >> i) & 1;
    }
}

sf::RenderWindow window;
sf::Vector2f currentPosition;
bool grid[WIDTH][HEIGHT];
int currentLine = 0;


// Create a random WIDTH length array of boolean
void randomize() {
    if (RANDOMIZE) {
        std::random_device rd{};
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(0, 1);

        for (int i = 0; i < WIDTH; i++) {
            grid[i][0] = dist(mt);
        }
        return;
    }

    for (int i = 0; i < WIDTH; i++) {
        grid[i][0] = false;
    }
    grid[WIDTH/2][0] = true;
}

// Fonction de configuration
// Elle est appelée une seule fois au début du programme
void setup() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; ++j) {
            grid[i][j] = false;
        }
    }
    randomize();
    // Création de la fenêtre
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Deplacement de carre");
}


void draw() {
    window.clear();

    sf::RectangleShape rectangle(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    rectangle.setFillColor(sf::Color::White);
    window.draw(rectangle);

    sf::RectangleShape cell(sf::Vector2f(WINDOW_WIDTH / WIDTH, WINDOW_HEIGHT / HEIGHT));
    cell.setFillColor(sf::Color::Black);

    for (int w = 0; w < WIDTH; w++) {
        for (int h = currentLine; h < HEIGHT; h++) {
            if (grid[w][h]) {
                cell.setPosition(sf::Vector2f(w * WINDOW_WIDTH / WIDTH, h * WINDOW_HEIGHT / HEIGHT));
                window.draw(cell);
            }
        }
    }

    window.display();
}

bool findVal(const bool neighbour[3]) {
    int valNeighbour = 0;
    for (int i = 0; i < 3; i++) {
        valNeighbour += neighbour[i] * (1 << i);
    }
    return ruleset[valNeighbour];
}

void compute() {
    randomize();

    float totalTime = 0;
#pragma omp parallel for
    for (int height = 1; height < HEIGHT; height++) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int width = 0; width < WIDTH; width++) {
            bool neighbours[3];
            if (width == 0) {
                neighbours[0] = false;
                neighbours[1] = grid[width][height-1];
                neighbours[2] = grid[width+1][height-1];
            } else if (width == WIDTH-1) {
                neighbours[0] = grid[width-1][height-1];
                neighbours[1] = grid[width][height-1];
                neighbours[2] = false;
            } else {
                neighbours[0] = grid[width-1][height-1];
                neighbours[1] = grid[width][height-1];
                neighbours[2] = grid[width+1][height-1];
            }

            grid[width][height] = findVal(neighbours);
        }
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
        totalTime += duration.count();
        draw();
    }
}

int main() {
    convertRuleset();
    setup();

    auto start = std::chrono::high_resolution_clock::now();
    compute();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
    std::cout << "Computing time: " << duration.count() << "ms" << std::endl;

    // Boucle principale du programme
    while (window.isOpen()) {

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        draw();
    }

    return 666;
}