#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <array>
#include <cmath>

const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 1024;

const int CIRCLE_GRID_WIDTH = 64;
const int CIRCLE_GRID_HEIGHT = 64;

const int CIRCLE_RAD_MARGIN = 0;
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

    int imageSampleWidth = (image.cols / CIRCLE_GRID_WIDTH);
    int imageSampleHeight = (image.rows / CIRCLE_GRID_HEIGHT);

    std::array<std::array<float, CIRCLE_GRID_WIDTH>, CIRCLE_GRID_HEIGHT> circleMagnitudes = {};

    std::cout << "Image sample size: "
        << imageSampleWidth << " x "
        << imageSampleHeight << "\n";

    for (int i = 0; i < CIRCLE_GRID_WIDTH; ++i) {
        for (int j = 0; j < CIRCLE_GRID_HEIGHT; ++j) {

            int totalPixelMagnitude = 0;
            float gridPixelMagnitudeAverage;

            for (int k = 0; k < imageSampleWidth; ++k) {
                for (int l = 0; l < imageSampleHeight; ++l) {
                    cv::Vec3b pixel = image.at<cv::Vec3b>(j * imageSampleHeight + l, i * imageSampleWidth + k);

                    totalPixelMagnitude += pixel[0] + pixel[1] + pixel[2];
                    //std::cout << "Pixel at (" << (i * imageSampleWidth + k) << ", " << (j * imageSampleHeight + l) << "): "
                    //    << "R: " << static_cast<int>(pixel[2]) << ", "
                    //    << "G: " << static_cast<int>(pixel[1]) << ", "
                    //    << "B: " << static_cast<int>(pixel[0]) << "\n"
                    //    << "Grid at (" << (i) << ", " << (j) << "): ";
                }
            }

            gridPixelMagnitudeAverage = float(totalPixelMagnitude / (imageSampleWidth * imageSampleHeight * 3.0 * 255.0));
            circleMagnitudes[i][j] = gridPixelMagnitudeAverage;
        }
    }

    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Dot Matrix Visualizer");

    window.setFramerateLimit(60);

    //float circleRadius = WINDOW_WIDTH;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        std::vector<sf::CircleShape> circles;

        for (int i = 0; i < CIRCLE_GRID_WIDTH; ++i) {
            for (int j = 0; j < CIRCLE_GRID_HEIGHT; ++j) {
                float circleGridX = (i * (CIRCLE_RAD_TOTAL) * 2) + (CIRCLE_RAD_TOTAL);
                float circleGridY = (j * (CIRCLE_RAD_TOTAL) * 2) + (CIRCLE_RAD_TOTAL);

                float dynamicRadius = circleMagnitudes[i][j] * CIRCLE_RAD_MAX;

                float circleDrawX = circleGridX - dynamicRadius;
                float circleDrawY = circleGridY - dynamicRadius;

                sf::CircleShape shape(dynamicRadius);
                shape.setFillColor(sf::Color::White);
                shape.setPosition({ circleDrawX, circleDrawY });

                circles.push_back(shape);
            }
        }

        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        //    circleRadius += 100.0f;
        //}
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && circleRadius > 0.f) {
        //    circleRadius -= 100.0f;
        //}

        //sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        //float mouseX = static_cast<float>(mousePos.x);
        //float mouseY = static_cast<float>(mousePos.y);

        //std::vector<sf::CircleShape> circles;
        //for (int i = 0; i < CIRCLE_GRID_WIDTH; ++i) {
        //    for (int j = 0; j < CIRCLE_GRID_HEIGHT; ++j) {
        //
        //        float circleGridX = (i * (CIRCLE_RAD_TOTAL) * 2) + (CIRCLE_RAD_TOTAL);
        //        float circleGridY = (j * (CIRCLE_RAD_TOTAL) * 2) + (CIRCLE_RAD_TOTAL);
        //
        //        float distFromMouse = std::sqrt(std::pow((circleGridX - mouseX), 2) + std::pow((circleGridY - mouseY), 2));
        //
        //        float dynamicRadius = ((static_cast<float>(circleRadius) - std::pow(distFromMouse, 2)) / static_cast<float>(circleRadius)) * CIRCLE_RAD_MAX;
        //
        //        if (dynamicRadius < 0.f) dynamicRadius = 0.f;
        //
        //        float circleDrawX = circleGridX - dynamicRadius;
        //        float circleDrawY = circleGridY - dynamicRadius;
        //
        //        sf::CircleShape shape(dynamicRadius);
        //        shape.setFillColor(sf::Color::White);
        //        shape.setPosition({ circleDrawX, circleDrawY });
        //
        //        circles.push_back(shape);
        //    }
        //}

        window.clear();

        for (const auto& circle : circles) {
            window.draw(circle);
        }

        window.display();
    }
}