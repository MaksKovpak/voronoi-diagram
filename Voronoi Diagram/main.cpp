#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

// #define COLORS

using namespace sf;

int main(int argc, char const* argv[]) {
	if (!Shader::isAvailable()) {
		std::cerr << "Shaders are not available on this PC!" << std::endl;
		return EXIT_FAILURE;
	}

	const int WIDTH = 1200, HEIGHT = 800;
	const int MAX_POINTS_NUMBER = 512;
	int pointsNumber = 20;

	std::random_device dev;
	std::default_random_engine gen(dev());

	// With a margin of 30 pixels from the edges of the window
	std::uniform_real_distribution<> wRand(30.0, WIDTH - 30.0), hRand(30.0, HEIGHT - 30.0); 

	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Voronoi Diagram", Style::Close | Style::Titlebar, settings);
	
	std::vector<Vector2f> coordinates(pointsNumber);
	std::generate(coordinates.begin(), coordinates.end(), [&]() { return Vector2f(wRand(gen), hRand(gen)); });

	#ifdef COLORS
		std::uniform_real_distribution<> frand(70.0 / 255, 1.0); // Pastel colors (also may be 90.0 / 255)
		std::vector<Vector3f> colors(pointsNumber); // Colors of cells
		std::generate(colors.begin(), colors.end(), [&]() { return Vector3f(frand(gen), frand(gen), frand(gen)); });
	#endif

	// To draw appropriate points
	std::vector<std::pair<CircleShape, bool>> circles; // The second parameter tells us whether the point is movable
	float radius = 4;
	CircleShape tempPoint(radius, 100);
	tempPoint.setFillColor(Color::Black);
	tempPoint.setOrigin(radius, radius);
	tempPoint.setOutlineColor(Color::Green);

	for (auto& coord : coordinates) {
		tempPoint.setPosition(coord);
		circles.push_back({ tempPoint, false });
	}

	std::string shaderName;

	#ifdef COLORS
		shaderName = "voronoiColors.frag";
	#else 
		shaderName = "voronoi.frag";
	#endif

	Shader shader;
	if (!shader.loadFromFile(shaderName, Shader::Fragment))
		return EXIT_FAILURE;

	while (window.isOpen()) {
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				for (auto& circle : circles) {
					if (circle.first.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window)))) {
						circle.second = !circle.second;
						circle.first.setOutlineThickness(circle.second ? 2 : 0);
					}
				}
			}

			// Create a new point by clicking if Left Alt is pressed
			if (Keyboard::isKeyPressed(Keyboard::LAlt) && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				bool outOfCircle = std::none_of(circles.begin(), circles.end(), [&](auto circle) {
					return circle.first.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window)));
				});
				
				if (outOfCircle) {
					coordinates.push_back(Vector2f(Mouse::getPosition(window)));
					tempPoint.setPosition(coordinates.back());
					circles.push_back({ tempPoint, false });

					#ifdef COLORS
						colors.push_back(Vector3f(frand(gen), frand(gen), frand(gen)));
					#endif

					pointsNumber++;
				}
			}
		} 

		for (size_t i = 0; i < pointsNumber; i++) {
			auto mousePos = Mouse::getPosition(window);
			if (std::hypot(mousePos.x - coordinates[i].x, mousePos.y - coordinates[i].y) <= 10.0) {
				radius = 6.0;
			} else {
				radius = 4.0;
			}

			circles[i].first.setRadius(radius);
			circles[i].first.setOrigin(radius, radius);

			if (circles[i].second) {
				coordinates[i] = Vector2f(Mouse::getPosition(window));
				circles[i].first.setPosition(coordinates[i]);
			}
		}

		// Converting SFML coordinates to OpenGL
		auto toCoord = [&](Vector2f vec) {
			return Vector2f(vec.x, window.getSize().y - vec.y);
		};

		std::vector<Vector2f> copy(coordinates.size());
		std::transform(coordinates.begin(), coordinates.end(), copy.begin(), toCoord);

		shader.setUniform("size", pointsNumber);
		shader.setUniformArray("seeds", copy.data(), MAX_POINTS_NUMBER);

		#ifdef COLORS
			shader.setUniformArray("colors", colors.data(), MAX_POINTS_NUMBER);
		#endif

		window.clear(Color::White);
		window.draw(RectangleShape(Vector2f(WIDTH, HEIGHT)), &shader);

		for (auto& c : circles) {
			window.draw(c.first);
		}

		window.display();
	}

	return EXIT_SUCCESS;
}