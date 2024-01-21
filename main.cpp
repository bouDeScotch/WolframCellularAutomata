#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500
#define HEIGHT 100
#define WIDTH (HEIGHT*2+1)
#define RULESET 0b00011110


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
    for (int i = 0; i < WIDTH; i++) {
        grid[i][0] = rand() % 2;
    }
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
    // Effacement de la fenêtre
    window.clear();

    // Fond blanc
    sf::RectangleShape rectangle(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    rectangle.setPosition(sf::Vector2f(0, 0));
    rectangle.setFillColor(sf::Color::White);
    window.draw(rectangle);

    auto start = std::chrono::high_resolution_clock::now();
    for (int w = 0; w < WIDTH; w++) {
        for (int h = currentLine; h < HEIGHT; h++) {
            if (grid[w][h]) {
                sf::RectangleShape rectangle(sf::Vector2f(WINDOW_WIDTH / WIDTH, WINDOW_HEIGHT / HEIGHT));
                rectangle.setPosition(sf::Vector2f(w * WINDOW_WIDTH / WIDTH, h * WINDOW_HEIGHT / HEIGHT));
                rectangle.setFillColor(sf::Color::Black);
                window.draw(rectangle);
            }
        }
    }

    // Affichage du contenu de la fenêtre
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
    for (int height = 1; height < HEIGHT; height++) {
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
        draw();
    }
}

int main() {
    convertRuleset();
    setup();
    randomize();

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

    return 0;
}