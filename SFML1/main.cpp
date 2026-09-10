#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>

const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 1024;

const int CIRCLE_GRID_WIDTH = 64;
const int CIRCLE_GRID_HEIGHT = 64;

const int CIRCLE_RAD_MARGIN = 0;
const int CIRCLE_RAD_MAX = (((WINDOW_WIDTH / CIRCLE_GRID_WIDTH) / 2) - CIRCLE_RAD_MARGIN);
const int CIRCLE_RAD_TOTAL = ((WINDOW_WIDTH / CIRCLE_GRID_WIDTH) / 2);

const bool COLORMODE = true;

struct GridSample {
    float magnitude = 0.f;
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
};

using MagnitudeGrid = std::array<std::array<GridSample, CIRCLE_GRID_HEIGHT>, CIRCLE_GRID_WIDTH>;

int main() {
    cv::Mat image = cv::imread("image2.jpg");

    if (image.empty()) {
        std::cout << "Could not load image\n";
        return 1;
    }

    std::cout << "Image size: "
        << image.cols << " x "
        << image.rows << "\n";

    int imageSampleWidth = (image.cols / CIRCLE_GRID_WIDTH);
    int imageSampleHeight = (image.rows / CIRCLE_GRID_HEIGHT);

    MagnitudeGrid circleMagnitudes{};

    std::cout << "Image sample size: "
        << imageSampleWidth << " x "
        << imageSampleHeight << "\n";

    for (int i = 0; i < CIRCLE_GRID_WIDTH; ++i) {
        for (int j = 0; j < CIRCLE_GRID_HEIGHT; ++j) {

            int redMagnitude = 0;
            int greenMagnitude = 0;
            int blueMagnitude = 0;
            int totalPixelMagnitude = 0;

            for (int k = 0; k < imageSampleWidth; ++k) {
                for (int l = 0; l < imageSampleHeight; ++l) {
                    cv::Vec3b pixel = image.at<cv::Vec3b>(j * imageSampleHeight + l, i * imageSampleWidth + k);

                    redMagnitude += pixel[2];
                    greenMagnitude += pixel[1];
                    blueMagnitude += pixel[0];
                    totalPixelMagnitude += pixel[0] + pixel[1] + pixel[2];
                }
            }

            double sampleCount = imageSampleWidth * imageSampleHeight;

            circleMagnitudes[i][j].magnitude = float(totalPixelMagnitude / (sampleCount * 3.0 * 255.0));
            circleMagnitudes[i][j].r = float(redMagnitude / (sampleCount * 255.0));
            circleMagnitudes[i][j].g = float(greenMagnitude / (sampleCount * 255.0));
            circleMagnitudes[i][j].b = float(blueMagnitude / (sampleCount * 255.0));
        }
    }

    sf::RenderWindow window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Dot Matrix Visualizer");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        std::vector<sf::CircleShape> circles;
        circles.reserve(CIRCLE_GRID_WIDTH * CIRCLE_GRID_HEIGHT);

        for (int i = 0; i < CIRCLE_GRID_WIDTH; ++i) {
            for (int j = 0; j < CIRCLE_GRID_HEIGHT; ++j) {
                const GridSample& sample = circleMagnitudes[i][j];

                float circleGridX = (i * (CIRCLE_RAD_TOTAL) * 2) + (CIRCLE_RAD_TOTAL);
                float circleGridY = (j * (CIRCLE_RAD_TOTAL) * 2) + (CIRCLE_RAD_TOTAL);

                float dynamicRadius = sample.magnitude * CIRCLE_RAD_MAX;

                float circleDrawX = circleGridX - dynamicRadius;
                float circleDrawY = circleGridY - dynamicRadius;

                sf::CircleShape shape(dynamicRadius);

                if (COLORMODE) {
                    shape.setFillColor(sf::Color(
                        static_cast<std::uint8_t>(sample.r * 255.f),
                        static_cast<std::uint8_t>(sample.g * 255.f),
                        static_cast<std::uint8_t>(sample.b * 255.f)
                    ));
                }
                else {
                    shape.setFillColor(sf::Color::White);
                }

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