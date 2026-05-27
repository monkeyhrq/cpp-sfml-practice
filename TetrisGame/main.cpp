#include<SFML/Graphics.hpp>
#include<iostream>

int main() {
	//圖片匯入
	sf::Texture yellowblock;
	if (!yellowblock.loadFromFile("yellow.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture lightBlueblock;
	if (!lightBlueblock.loadFromFile("light_blue.png")) {
		return EXIT_FAILURE;
	}
	float blockWidth = yellowblock.getSize().x;
	float blockHeight = yellowblock.getSize().y;

	//形狀種類
	enum class Type {
		None,
		O,
		I
	};

	//場地大小
	const int fieldWidth = 10;
	const int fieldHeight = 20;

	//視窗
	const int windowWidth = fieldWidth * blockWidth;
	const int windowHeight = fieldHeight * blockHeight;
	sf::VideoMode mode(
		unsigned int(fieldWidth * blockWidth),
		unsigned int(fieldHeight * blockHeight));
	sf::RenderWindow window(mode, L"俄羅斯方塊");

	//背景圖片
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("background.png")) {
		return EXIT_FAILURE;
	}
	backgroundTexture.setRepeated(true);
	sf::Sprite backgroundSprite(backgroundTexture,
		sf::IntRect(0, 0, windowWidth, windowHeight));

	//預設位置
	sf::Vector2i origin(fieldWidth / 2, 0);
	sf::Vector2i pos(origin);

	//場地方塊
	Type field[fieldWidth][fieldHeight] = {};

	//方塊O I
	std::map<Type, std::vector<std::vector<sf::Vector2i>>>shapes{
		//shape_Os
		{
			Type::O,
			std::vector<std::vector<sf::Vector2i>> {
				{
					sf::Vector2i(0, -1),
						sf::Vector2i(0, 0),
						sf::Vector2i(1, -1),
						sf::Vector2i(1, 0)
				}
			},
		},
		{
			Type::I,
			//shpae_Is
			std::vector<std::vector<sf::Vector2i>> {
				std::vector<sf::Vector2i>{
					sf::Vector2i(-1, 0),
					sf::Vector2i(0, 0),
					sf::Vector2i(1, 0),
					sf::Vector2i(2, 0)
				},
					std::vector<sf::Vector2i> {
					sf::Vector2i(0, -2),
					sf::Vector2i(0, -1),
					sf::Vector2i(0, 0),
					sf::Vector2i(0, 1)
				}
			}
		}
	};

	//預設 O或I
	Type currentType = Type(rand() % 2 + 1);
	std::vector<sf::Vector2i> currentShape;
	//方向預設為0
	int currentIndex = 0;

	//Sprite
	std::map<Type, sf::Sprite> Sprites{
		{Type::O, sf::Sprite(yellowblock)},
		{ Type::I,sf::Sprite(lightBlueblock) }
	};
	sf::Sprite currentSprite;

	sf::Clock clock;
	while (window.isOpen()) {
		currentShape = shapes[currentType][currentIndex];
		currentSprite = Sprites[currentType];
		enum class Action {
			Hold,
			MoveDown,
			MoveLeft,
			MoveRight,
			Rotate
		};
		Action action = Action::Hold;
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Left) {
					action = Action::MoveLeft;
				}
				if (evt.key.code == sf::Keyboard::Right) {
					action = Action::MoveRight;
				}
				if (evt.key.code == sf::Keyboard::Down) {
					action = Action::MoveDown;
				}
				if (evt.key.code == sf::Keyboard::Up) {
					action = Action::Rotate;
				}

			}
		}
		if (clock.getElapsedTime().asSeconds() >= 0.4f) {
			action = Action::MoveDown;
			clock.restart();
		}

		//碰撞偵測
		int nextIndex = currentIndex;
		if (action == Action::Rotate) {
			nextIndex = int(nextIndex + 1) % shapes[currentType].size();
		}
		std::vector<sf::Vector2i> nextShape = shapes[currentType][nextIndex];
		int countEmpty = 0; //四個方塊移動次數，預設為0可以就+1
		sf::Vector2i nextpos = pos;
		switch (action)
		{
		case Action::Hold:
			break;
		case Action::MoveDown:
			nextpos.y++;
			break;
		case Action::MoveLeft:
			nextpos.x--;
			break;
		case Action::MoveRight:
			nextpos.x++;
			break;
		default:
			break;
		}
		for (const sf::Vector2i& d : nextShape) {
			sf::Vector2i np = d + nextpos;
			if (np.x >= 0 && np.x < fieldWidth &&
				np.y < fieldHeight &&
				(np.y <= 0 || field[np.x][np.y] == Type::None)) {
				countEmpty++;
			}
		}
		if (countEmpty == 4) {
			pos = nextpos;
			currentIndex = nextIndex;
			currentShape = nextShape;
		}
		else {
			if (action == Action::MoveDown) {
				for (const sf::Vector2i& d : currentShape) {
					sf::Vector2i np = d + pos;
					field[np.x][np.y] = currentType;
				}
				for (int y = 0; y < fieldHeight; y++) {
					bool isFull = true;
					for (int x = 0; x < fieldWidth; x++) {
						if (field[x][y] == Type::None) {
							isFull = false;
						}
					}
					if (isFull) {
						for (int ty = y; ty > 0; ty--) {
							for (int x = 0; x < fieldWidth; x++) {
								field[x][ty] = field[x][ty - 1];
							}
						}
						for (int x = 0; x < fieldWidth; x++) {
							field[x][0] = Type::None;
						}
					}
				}

				pos = origin;
				currentType = Type(rand() % 2 + 1);
				currentIndex = 0;
			}
		}
		window.clear();

		//畫背景圖片
		window.draw(backgroundSprite);
		//底部方塊
		for (int x = 0; x < fieldWidth; x++) {
			for (int y = 0; y < fieldHeight; y++) {
				if (field[x][y] == Type::None) continue;

				sf::Sprite& s = Sprites[field[x][y]];

				s.setPosition(
					float(x * blockWidth),
					float(y * blockHeight));
				window.draw(s);
			}
		}
		//移動方塊
		for (const sf::Vector2i& d : currentShape) {
			sf::Vector2i np = pos + d;
			currentSprite.setPosition(
				float(np.x * blockWidth),
				float(np.y * blockHeight));
			window.draw(currentSprite);
		}
		window.display();
	}
	return EXIT_SUCCESS;
}