#include <iostream>
#include <thread>
#include <ctime>
#include <conio.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

struct currency {
    std::string code;
    std::vector<double> allTimeValues;
    double allTimeSum = 0;
};

long cyclesCnt = 0;

currency currencies[] = {{"AUD"}, {"AZN"}, {"GBP"}, {"AMD"}, {"BYN"}, {"BGN"}, {"BRL"},
                         {"HUF"}, {"HKD"}, {"DKK"}, {"USD"}, {"EUR"}, {"INR"}, {"KZT"},
                         {"CAD"}, {"KGS"}, {"CNY"}, {"MDL"}, {"NOK"}, {"PLN"}, {"RON"},
                         {"XDR"}, {"SGD"}, {"TJS"}, {"TRY"}, {"TMT"}, {"UZS"}, {"UAH"},
                         {"CZK"}, {"SEK"}, {"CHF"}, {"ZAR"}, {"KRW"}, {"JPY"}};

size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string& buf = *static_cast<std::string*>(userdata);
    buf.append(ptr, std::next(ptr, nmemb * size));
    return nmemb * size;
}

void jsonRequest(const std::string& url) {
    cyclesCnt++;
    std::string str_buffer;
    CURL *curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str_buffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return;
        }
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "could not initialize curl" << std::endl;
    }
    nlohmann::json json = nlohmann::json::parse(str_buffer.erase(0, 90).erase(str_buffer.length() - 2, 2));

    for (auto & currencyUnit : currencies) {
        std::string unit = json["Valute"][currencyUnit.code]["Nominal"].get<int>() == 1 ? " currency unit = " : " currency units = ";
        std::cout << currencyUnit.code << ": " << json["Valute"][currencyUnit.code]["Nominal"] << unit << json["Valute"][currencyUnit.code]["Value"] << " roubles." << std::endl;
        currencyUnit.allTimeValues.push_back(json["Valute"][currencyUnit.code]["Value"].get<double>());
        currencyUnit.allTimeSum += json["Valute"][currencyUnit.code]["Value"].get<double>();
    }
    std::cout << std::endl << "********************************************************************************" << std::endl << std::endl;
}

int main() {

    long long startTime = time(nullptr) - 10;

    while (true) {
        if (kbhit()) break;

        if (time(nullptr) - startTime >= 10) {
            jsonRequest("https://www.cbr-xml-daily.ru/daily_jsonp.js");
            startTime = time(nullptr);
        }
    }

    for (auto & currencyUnit : currencies) {
        std::cout << currencyUnit.code << ": Average value: " << currencyUnit.allTimeSum / (double) cyclesCnt << ". Medium value: " << currencyUnit.allTimeValues[cyclesCnt / 2] << std::endl;
    }

    return 0;
}
