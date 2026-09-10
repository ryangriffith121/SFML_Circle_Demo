#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

const unsigned int WINDOW_WIDTH = 768;
const unsigned int WINDOW_HEIGHT = 768;

const int CIRCLE_GRID_WIDTH = 32;
const int CIRCLE_GRID_HEIGHT = 32;

const int CIRCLE_RAD_MARGIN = 2;
const int CIRCLE_RAD_MAX = (((WINDOW_WIDTH / CIRCLE_GRID_WIDTH) / 2) - CIRCLE_RAD_MARGIN);
const int CIRCLE_RAD_TOTAL = ((WINDOW_WIDTH / CIRCLE_GRID_WIDTH) / 2);

int main() {
    cv::Mat image = cv::imread("image.jpg");

    if (image.empty()) {
        std::cout << "Could not load image\n";
        return 1;
    }

    std::cout << "Image size: "
        << image.cols << " x "
        << image.rows << "\n";

    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_WIDTH }), "Dot Matrix Visualizer");

    float circleRadius = WINDOW_WIDTH;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            circleRadius += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && circleRadius > 0.f) {
            circleRadius -= 1.0f;
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        float mouseX = static_cast<float>(mousePos.x);
        float mouseY = static_cast<float>(mousePos.y);

        std::vector<sf::CircleShape> circles;
        for (int i = 0; i < CIRCLE_GRID_WIDTH; ++i) {
            for (int j = 0; j < CIRCLE_GRID_HEIGHT; ++j) {

                float circleGridX = (i * (CIRCLE_RAD_TOTAL) * 2) + (CIRCLE_RAD_TOTAL);
                float circleGridY = (j * (CIRCLE_RAD_TOTAL) * 2) + (CIRCLE_RAD_TOTAL);

                float distFromMouse = std::sqrt(std::pow((circleGridX - mouseX), 2) + std::pow((circleGridY - mouseY), 2));

                float dynamicRadius = ((static_cast<float>(circleRadius) - distFromMouse) / static_cast<float>(circleRadius)) * CIRCLE_RAD_MAX;

                if (dynamicRadius < 0.f) dynamicRadius = 0.f;

                float circleDrawX = circleGridX - dynamicRadius;
                float circleDrawY = circleGridY - dynamicRadius;

                sf::CircleShape shape(dynamicRadius);
                shape.setFillColor(sf::Color::White);
                shape.setPosition({ circleDrawX, circleDrawY });

                circles.push_back(shape);
            }
        }

        window.clear();

        for (const auto& circle : circles) {
            window.draw(circle);
        }

        window.display();
    }
}