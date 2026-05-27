#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>
#include <httplib.h>

// 將浮點數格式化為 1 位小數的字串
std::string formatFloat(float value) {
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << value;
	return oss.str();
}

// 將 open-meteo 的 weather code 對應到簡短描述
std::string weatherLabel(int code) {
	if (code == 0) return "Sunny";
	if (code >= 1 && code <= 3) return "Cloudy";
	if (code == 45 || code == 48) return "Fog";
	if (code >= 51 && code <= 67) return "Rain";
	if (code >= 71 && code <= 77) return "Snow";
	if (code >= 80 && code <= 82) return "Shower";
	if (code >= 95 && code <= 99) return "Storm";
	return "Cloudy";
}

int main() {
	httplib::Client client("api.open-meteo.com");
	auto res = client.Get(
		"/v1/forecast?latitude=24.15&longitude=120.67"
		"&current=temperature_2m,relative_humidity_2m,wind_speed_10m"
		"&daily=temperature_2m_max,temperature_2m_min,precipitation_probability_max,weather_code"
		"&timezone=Asia/Taipei&forecast_days=7"
	);
	if (!res || res->status != 200) {
		return EXIT_FAILURE;
	}
	nlohmann::json j = nlohmann::json::parse(res->body);

	// 即時資料
	float temperature = j["current"]["temperature_2m"];
	int humidity = j["current"]["relative_humidity_2m"];
	float windSpeed = j["current"]["wind_speed_10m"];

	// 七天預報資料
	auto dates = j["daily"]["time"];
	auto maxTemps = j["daily"]["temperature_2m_max"];
	auto minTemps = j["daily"]["temperature_2m_min"];
	auto rainProbs = j["daily"]["precipitation_probability_max"];
	auto codes = j["daily"]["weather_code"];

	// 字型
	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {
		return EXIT_FAILURE;
	}

	// === 即時天氣區塊 ===
	sf::Text title("Taichung Weather", font, 22);
	title.setPosition(15, 10);

	sf::Text currentInfo(
		"Now: " + formatFloat(temperature) + " C   " +
		"Humidity: " + std::to_string(humidity) + "%   " +
		"Wind: " + formatFloat(windSpeed) + " km/h",
		font, 16
	);
	currentInfo.setPosition(15, 45);

	// === 七天預報區塊 ===
	sf::Text forecastTitle("7-Day Forecast", font, 20);
	forecastTitle.setPosition(15, 85);

	std::vector<sf::Text> dayTexts;

	int startY = 120;
	int rowHeight = 35;

	for (size_t i = 0; i < dates.size(); i++) {
		std::string date = dates[i];
		float maxT = maxTemps[i];
		float minT = minTemps[i];
		int rain = rainProbs[i].is_null() ? 0 : (int)rainProbs[i];
		int code = codes[i];

		// 整行：天氣 + 日期 + 溫度 + 降雨
		std::string line =
			weatherLabel(code) + std::string(8 - weatherLabel(code).length(), ' ') +
			date + "   " +
			formatFloat(maxT) + "/" + formatFloat(minT) + " C   " +
			"Rain: " + std::to_string(rain) + "%";
		sf::Text dayText(line, font, 16);
		dayText.setPosition(15, (float)(startY + i * rowHeight));
		dayTexts.push_back(dayText);
	}

	sf::RenderWindow window(sf::VideoMode(500, 380), "Taichung Weather");

	while (window.isOpen()) {
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear(sf::Color(30, 30, 50));
		window.draw(title);
		window.draw(currentInfo);
		window.draw(forecastTitle);
		for (auto& t : dayTexts) window.draw(t);
		window.display();
	}
	return EXIT_SUCCESS;
}
