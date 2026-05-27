#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
	sf::Font font;
	if (font.loadFromFile("arial.ttf") == false) {
		return EXIT_FAILURE;
	}
	enum class Direction { LEFT_TO_RIGHT, RIGHT_TO_LEFT };
	enum class Speed { SPEED1, SPEED2, SPEED3, SPEED4, SPEED5 };
	Direction direction = Direction::RIGHT_TO_LEFT;
	Speed speed = Speed::SPEED1;

	sf::Text text("Hello", font, 200);
	text.setFillColor(sf::Color(0, 255, 0));
	sf::CircleShape circle(2);
	circle.setFillColor(sf::Color(0, 0, 0));

	sf::RenderWindow w(sf::VideoMode(500, 250), "Hello");
	if (direction == Direction::RIGHT_TO_LEFT) {
		text.setPosition((float)(w.getSize().x), 0);
	}

	while (w.isOpen()) {
		sf::Event evt;
		if (w.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				std::cout << "Close" << std::endl;
				w.close();
			}
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Left) {
					direction = Direction::RIGHT_TO_LEFT;
				}
				if (evt.key.code == sf::Keyboard::Right) {
					direction = Direction::LEFT_TO_RIGHT;
				}

				if (evt.key.code == sf::Keyboard::F ||
					evt.key.code == sf::Keyboard::Num5) {
					speed = Speed::SPEED5;
				}
				if (evt.key.code == sf::Keyboard::S ||
					evt.key.code == sf::Keyboard::Num1) {
					speed = Speed::SPEED1;
				}
				if (evt.key.code == sf::Keyboard::Num4) {
					speed = Speed::SPEED4;
				}
				if (evt.key.code == sf::Keyboard::Num3) {
					speed = Speed::SPEED3;
				}
				if (evt.key.code == sf::Keyboard::Num2) {
					speed = Speed::SPEED2;
				}
			}
		}
		w.clear();
		w.draw(text);
		for (unsigned int i = 0; i < w.getSize().x; i += 4) {
			for (unsigned int j = 0; j < w.getSize().y; j += 4) {
				circle.setPosition((float)i, (float)j);
				w.draw(circle);
			}
		}
		w.display();

		float diff;
		switch (speed) {
		case Speed::SPEED5:
			diff = 0.12f * 40;
			break;
		case Speed::SPEED4:
			diff = 0.09f * 40;
			break;
		case Speed::SPEED3:
			diff = 0.06f * 40;
			break;
		case Speed::SPEED2:
			diff = 0.04f * 40;
			break;
		case Speed::SPEED1:
			diff = 0.01f * 40;
			break;
		}
		switch (direction) {
		case Direction::LEFT_TO_RIGHT:
			text.move(diff, 0);
			break;
		case Direction::RIGHT_TO_LEFT:
			text.move(-diff, 0);
			break;
		}

	}
	return EXIT_SUCCESS;
}