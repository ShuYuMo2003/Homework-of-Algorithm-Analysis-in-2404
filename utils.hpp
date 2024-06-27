#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstring>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

void print_table(std::vector<std::string> & header, std::vector<std::vector<std::string>> & data);

void print_car_t_list(std::vector<car_t> _cars) {
    static std::vector<std::string> header = {
        "Car ID", "Brand", "Color", "Country of Manufacture", "Year of Manufacture", "Price in USD"
    };
    std::vector<std::vector<std::string>> data;
    for (auto &car : _cars) {
        data.push_back({
            car.get_id(),
            car.get_brand(),
            car.get_color(),
            car.get_country_of_manufacture(),
            std::to_string(car.get_year_of_manufacture()),
            car.get_string_price()
        });
    }
    print_table(header, data);
}

void print_table(std::vector<std::string> & header, std::vector<std::vector<std::string>> & data) {
    std::vector<int> placeholder_width(header.size(), 0);
    for (int i = 0; i < static_cast<int>(header.size()); i++) {
        placeholder_width[i] = header[i].size();
    }
    for (auto &row : data) {
        for (int i = 0; i < static_cast<int>(row.size()); i++) {
            placeholder_width[i] = std::max(placeholder_width[i], (int)row[i].size());
        }
    }

    std::cout << "+";
    for(int i = 0; i < static_cast<int>(header.size()); i++) {
        std::cout << "-" <<  std::string(placeholder_width[i], '-') << "-+";
    }
    std::cout << std::endl;

    std::cout << "|";
    for (int i = 0; i < static_cast<int>(header.size()); i++) {
        std::cout << " " << std::setw(placeholder_width[i]) << header[i] << " |";
    }
    std::cout << std::endl;

    std::cout << "+";
    for(int i = 0; i < static_cast<int>(header.size()); i++) {
        std::cout << "-" << std::string(placeholder_width[i], '-') << "-+";
    }
    std::cout << std::endl;

    for (auto &row : data) {
        std::cout << "|";
        for (int i = 0; i < static_cast<int>(row.size()); i++) {
            std::cout << " " << std::setw(placeholder_width[i]) << row[i] << " |";
        }
        std::cout << std::endl;
    }

    std::cout << "+";
    for(int i = 0; i < static_cast<int>(header.size()); i++) {
        std::cout << "-" << std::string(placeholder_width[i], '-') << "-+";
    }
    std::cout << std::endl;
}


#endif // UTILS_HPP