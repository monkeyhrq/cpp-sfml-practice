#include<iostream>
#include<SFML/Graphics.hpp>

int main() {
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("background.png")) {
		return EXIT_FAILURE;
	}
	sf::Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setPosition(0, 40);
	sf::VideoMode mode(642, 600);
	
	sf::RenderWindow window(mode, L"小畫家");

	// 畫筆
	sf::CircleShape cursor(10);
	cursor.setFillColor(sf::Color::Black);
	cursor.setOrigin(10, 10);

	// 畫布
	sf::RenderTexture canvas;
	canvas.create(600, 560);
	canvas.clear(sf::Color::White);
	sf::Sprite canvasSprite(canvas.getTexture());
	sf::Vector2f canvasOffset(20, 60);
	canvasSprite.setPosition(canvasOffset);

	// 調色盤
	std::vector<sf::Color> colors = {
		sf::Color::White,
		sf::Color::Black,
		sf::Color::Red,
		sf::Color::Green
	};

	std::vector<sf::RectangleShape> swatches;

	int swatchOffset = 20;
	for (const auto& color : colors) {
		sf::RectangleShape swatch(sf::Vector2f(20, 20));
		swatch.setFillColor(color);
		swatch.setOutlineColor(sf::Color(230, 230, 230));
		swatch.setOutlineThickness(2);
		swatch.setPosition(swatchOffset, 10);
		swatches.push_back(swatch);
		swatchOffset += 20;
	}

	while (window.isOpen()) {
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
			if (evt.type == sf::Event::MouseButtonPressed) {
				if (evt.mouseButton.button == sf::Mouse::Left) {
					for (const auto& swatch : swatches) {
						if (swatch.getGlobalBounds().contains(
							evt.mouseButton.x,
							evt.mouseButton.y)) {
							cursor.setFillColor(swatch.getFillColor());
						}
					}
				}
			}
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::S) {
					canvas.getTexture().copyToImage().saveToFile("canvas.png");
				}
			}
		}
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		// 計算畫筆在畫布上的座標
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			cursor.setPosition(sf::Vector2f(mousePos) - canvasOffset);
			canvas.draw(cursor);
			canvas.display();
		}

		window.clear(sf::Color::White);
		// 背景
		window.draw(backgroundSprite);
		// 調色盤
		for (const auto swatch : swatches) {
			window.draw(swatch);
		}
		// 畫布
		window.draw(canvasSprite);
		// 畫筆
		cursor.setPosition(sf::Vector2f(mousePos));
		window.draw(cursor);
		window.display();
	}
	return EXIT_SUCCESS;
}