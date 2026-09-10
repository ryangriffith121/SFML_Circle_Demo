# Dot Matrix Visualizer
 
A small C++ demo that turns any image into a live halftone-style "dot matrix" rendering. The image is sampled on a grid, and each grid cell is drawn as a circle whose **radius is proportional to the average brightness** of that region — brighter areas get bigger dots, darker areas get smaller (or invisible) ones.
 
Rendering is done in real time with **SFML**; image loading and pixel sampling is done with **OpenCV**.

<img width="1536" height="768" alt="Screenshot 2026-09-09 213308" src="https://github.com/user-attachments/assets/1ec7c2ca-7b31-41c2-a19f-fee9dbf62b38" />

## How It Works
 
1. **Load the image** — `cv::imread("image.jpg")` loads the source image from disk using OpenCV.
2. **Downsample into a grid** — the image is divided into a `CIRCLE_GRID_WIDTH × CIRCLE_GRID_HEIGHT` grid (64×64 by default). The size of each sample block is `image size / grid size`.
3. **Average brightness per cell** — for every grid cell, the B, G, and R channel values of each pixel in that block are summed and normalized to a `0.0–1.0` range (`sum / (pixels × 3 × 255)`).
4. **Map brightness → circle radius** — each cell's normalized brightness is multiplied by `CIRCLE_RAD_MAX` to get that cell's circle radius.
5. **Render** — an SFML window (1024×1024 by default) draws one `sf::CircleShape` per grid cell, positioned so each circle is centered in its cell, sized by the brightness computed above.
The result is a classic "dot screen" / halftone effect: the brighter a region of the source image, the larger (whiter) its dot.
 
## Features
 
- Real-time rendering of a static image as a scalable dot-matrix grid
- Simple, tunable parameters for grid resolution, window size, and max dot radius
- A ready-made (currently commented out) **interactive mode**: dots pulse in size based on distance from the mouse cursor, with `W` / `S` keys adjusting the pulse radius — a nice starting point for turning this into a live, input-driven visualizer instead of a static-image one
## Requirements
 
- A C++17-capable compiler
- [SFML](https://www.sfml-dev.org/) 3.0+ (this project uses the SFML 3 API — `sf::Event::Closed`, `window.pollEvent()`, `sf::VideoMode({width, height})`, `shape.setPosition({x, y})`)
- [OpenCV](https://opencv.org/) (only `core` and `imgcodecs` are needed, for `cv::Mat` / `cv::imread`)

## Usage
 
1. Place an image named **`image.jpg`** in the same directory the executable is run from.
2. Run the executable.
3. A window titled **"Dot Matrix Visualizer"** opens, showing your image rendered as a grid of dots.
4. Close the window to exit.
## Configuration
 
All the key parameters are constants at the top of `main.cpp`:
 
| Constant | Description |
|---|---|
| `WINDOW_WIDTH`, `WINDOW_HEIGHT` | Size of the render window in pixels (default `1024 × 1024`) |
| `CIRCLE_GRID_WIDTH`, `CIRCLE_GRID_HEIGHT` | Number of dot columns/rows sampled from the image (default `64 × 64`) |
| `CIRCLE_RAD_MARGIN` | Padding subtracted from the maximum circle radius, to leave a gap between dots |
| `CIRCLE_RAD_MAX` | Largest radius a fully-bright dot can reach |
| `CIRCLE_RAD_TOTAL` | Half the width/height of one grid cell — used to space dots evenly across the window |
 
Increasing the grid dimensions gives finer detail (more, smaller dots); decreasing them gives a chunkier, more abstract look.
 
## Known Limitations
 
- The image path is hardcoded to `image.jpg` — there's no command-line argument for choosing a file yet.
- The `circles` vector is rebuilt from scratch every frame even though the source image never changes after load — this is wasted work and could be hoisted outside the main loop.
- If the image dimensions aren't evenly divisible by the grid size, integer division truncates the remainder, so a thin strip of pixels along the right/bottom edge is never sampled.
- Dots are always solid white; color information from the source image isn't used.
