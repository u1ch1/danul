#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// Структурка 
struct DailyWeatherForecast {
    string date;
    double temperature;
    string description;
};

size_t writeCallback(char* ptr, size_t size, size_t nmemb, string* data) {
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

//К сайту
json getWeatherForecast(const string& city) {
    string apiKey = "e126731101f197fb0c486a0da335d277";
    string url = "http://api.openweathermap.org/data/2.5/forecast?q=" + city + "&appid=" + apiKey;

    CURL* curl = curl_easy_init();
    string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;

        curl_easy_cleanup(curl);
    }

    return json::parse(response);
}

//Пути для каждой
vector<DailyWeatherForecast> fillWeatherForecast(const json& forecastData) {
    vector<DailyWeatherForecast> weatherForecast;

    for (const auto& item : forecastData["list"]) {
        DailyWeatherForecast dailyForecast;

        dailyForecast.date = item["dt_txt"];
        dailyForecast.temperature = item["main"]["temp"];
        dailyForecast.description = item["weather"][0]["description"];

        weatherForecast.push_back(dailyForecast);
    }

    return weatherForecast;
}

// пяти дневка
void showWeatherForecast(const vector<DailyWeatherForecast>& weatherForecast) {
    for (const auto& forecast : weatherForecast) {
        cout << "День: " << forecast.date.substr(0, 10) << endl;
        cout << "Время: " << forecast.date.substr(11, 8) << endl;
        cout << "Температура: " << forecast.temperature << " K" << endl;
        cout << "Описание: " << forecast.description << endl;
        cout << endl;
    }
}

// Конретный
void showSpecificDayWeatherForecast(const vector<DailyWeatherForecast>& weatherForecast) {
    cout << "Введите дату прогноза (yyyy-mm-dd): ";
    string date;
    cin >> date;

    bool isWeatherAvailable = false;

    for (const auto& forecast : weatherForecast) {
        if (forecast.date.substr(0, 10) == date) {
            isWeatherAvailable = true;

            cout << "День: " << forecast.date.substr(0, 10) << endl;
            cout << "Температура: " << forecast.temperature << " K" << endl;
            cout << "Описание: " << forecast.description << endl;
            break;
        }
    }

    if (!isWeatherAvailable)
        cout << "Прогноз погоды на указанную дату отсутствует." << endl;
}
//кейсррррр
int main() {
    string city;
    cout << "Введите название города: ";
    cin >> city;

    json forecastData = getWeatherForecast(city);
    vector<DailyWeatherForecast> weatherForecast = fillWeatherForecast(forecastData);

    int choice = 0;

    while (choice != 3) {
        cout << endl;
        cout << "Меню:" << endl;
        cout << "1. Просмотр прогноза на 5 дней" << endl;
        cout << "2. Просмотр прогноза на конкретный день" << endl;
        cout << "3. Выход" << endl;

        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        
        case 1:
            showWeatherForecast(weatherForecast);
            break;
        case 2:
            showSpecificDayWeatherForecast(weatherForecast);
            break;
        case 3:
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Выбрано неверное действие. Попробуйте снова." << endl;
            break;
        }

        cout << endl;
    }

    return 0;
}
