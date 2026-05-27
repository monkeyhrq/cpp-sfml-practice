#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
	using V2i = sf::Vector2i;
	using V2f = sf::Vector2f;

	//���a�j�p �e20, ��15
	V2i fieldSize(20, 15);

	//��r
	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {
		return EXIT_FAILURE;
	}

	sf::Text Pass("Pass", font, 50);
	sf::Text Loss("Loss", font, 50);
	sf::Text Eaten("Eaten", font, 20);
	
	//�Ϥ�
	sf::Texture blockTexture;
	if (!blockTexture.loadFromFile("block.png")) {
		std::cout << "block.png is not found " << std::endl;
		return EXIT_FAILURE;
	}
	sf::Sprite block(blockTexture);

	//�Ϥ��j�p
	V2f blockSize(
		block.getLocalBounds().width,
		block.getLocalBounds().height);

	// Head FOOD��m
	std::vector<V2i> snake = { V2i(3, 4) }; //�i�W�[�j�p���}�C 
	srand(time(0));
	sf::Vector2i food(rand() % fieldSize.x, rand() % fieldSize.y);

	//�ɶ�
	sf::Clock clock;

	//��V
	enum class Direction { UP, DOWN, LEFT, RIGHT };
	Direction direction = Direction::RIGHT;

	//���a�j�p
	const int windowWidth = fieldSize.x * blockSize.x;
	const int windowHeight = fieldSize.y * blockSize.y;
	sf::VideoMode mode(windowWidth, windowHeight);
	sf::RenderWindow w(mode, L"�g�Y�D");

	//background�Ϥ�
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("background.png")) {
		return EXIT_FAILURE;
	}
	backgroundTexture.setRepeated(true);
	sf::Sprite backgroundSprite(
		backgroundTexture,
		sf::IntRect(0, 0, windowWidth, windowHeight));

	//�O�_����
	bool isDead = false;

	//�Y��������
	int eaten = 0;

	//�t��
	float Speed = 0.0;


	while (w.isOpen()) {
		sf::Event evt;
		if (w.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				w.close();
			}
			//�����V
			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Left) {
					direction = Direction::LEFT;
				}
				if (evt.key.code == sf::Keyboard::Right) {
					direction = Direction::RIGHT;
				}
				if (evt.key.code == sf::Keyboard::Up) {
					direction = Direction::UP;
				}
				if (evt.key.code == sf::Keyboard::Down) {
					direction = Direction::DOWN;
				}
			}
			//�ץ�N���䭫�s�}�l
			if (evt.type == sf::Event::KeyPressed) {
				if (isDead) {
					isDead = false;
					snake = { V2i(3, 4) }; //�i�W�[�j�p���}�C 
					food.x = rand() % fieldSize.x;
					food.y = rand() % fieldSize.y;
					direction = Direction::RIGHT;
				}
			}
		}
		
		//HEAD����
		V2i head = snake[0]; //head�ƻs�_�l��m(3, 4)
		if (clock.getElapsedTime().asSeconds() >= (0.4f - Speed)) {
			if (direction == Direction::LEFT) {
				head.x--;
			}
			if (direction == Direction::RIGHT) {
				head.x++;
			}
			if (direction == Direction::UP) {
				head.y--;
			}
			if (direction == Direction::DOWN) {
				head.y++;
			}
			

			//������ɥX��
			if (head.x < 0 || head.x >= fieldSize.x ||
				head.y < 0 || head.y >= fieldSize.y) {
				isDead = true;
			}
			//���쨭��X��
			for (const V2i& body : snake) {
				if (head == body) {
					isDead = true;
				}
			}

			//********** 
			//�����||�q���N���|�A�~���
			if (!isDead && eaten < 10) {
				// ���ʰ}�C snake {(3, 4)}  > snake {(4, 4), (3, 4)}
				snake.insert(snake.begin(), head);

				//�Y�쭹��
				if (food == head) {
					food = V2i(rand() % fieldSize.x, rand() % fieldSize.y);
					eaten++;
					std::cout << "Eaten" << eaten << std::endl;
					//����t��
					if (eaten == 3 || eaten == 6) {
						Speed = Speed + 0.1;
					}
				}
				else {
					//�S�Y���̫�@�Ӱ}�C�R�� snake {(4, 4), (3, 4)} >�@snake {(4, 4)}
					snake.pop_back();
				}
				clock.restart();
			}
		}

		if (isDead == true) {
			eaten = 0;
			Speed = 0.0;
			w.clear(sf::Color::Red);
			w.draw(Loss);
		}else
		if (eaten == 10) {
			w.clear(sf::Color::Green);
			w.draw(Pass);
		}
		else {
			w.clear();
		}
		//ø�s�I���Ϥ�
		if (!isDead && eaten < 10) {
			w.draw(backgroundSprite);
		}
		//�Y����
		std::string eatNumber = std::to_string(eaten);
		Eaten.setString("Eaten" + eatNumber);
		w.draw(Eaten);
		//ø�sfood
		V2f foodpos(food.x * blockSize.x,
			food.y * blockSize.y);
		block.setPosition(foodpos);
		w.draw(block);
		//ø�ssnake
		for (const V2i& body : snake) {
			V2f bodypos(body.x * blockSize.x,
				body.y * blockSize.y);
			block.setPosition(bodypos);
			w.draw(block);
		}
		w.display();
	}
	return EXIT_SUCCESS;
}