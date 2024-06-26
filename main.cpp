#include <iostream>
#include <string>
#include <random>

#include "car.hpp"
#include "recording.hpp"
#include "utils.hpp"

car_list cars;
record_list records;

std::mt19937 random_generator(time(nullptr));


enum OPT {
    op_add_new_car = 1,
    op_modify_car_data = 2,
    op_display_all_cars = 3,
    op_sort_cars_by_brand_or_price = 4,
    op_search_for_a_car_by_its_unique_id = 5,
    op_track_the_number_of_cars_sold = 6,
    op_remove_a_car_record = 7,
    op_sort_carts_by_best_selling_brand = 8,
    op_exit = 9
};

const std::string menu_prompt = (
    "+----------------  Menu  -----------------+\n"
    "|1. Add new car.                          |\n"
    "|2. Modify car data.                      |\n"
    "|3. Display all cars                      |\n"
    "|4. Sort cats by brand or price.          |\n"
    "|5. Search for a car by its unique ID.    |\n"
    "|6. Track the number of cars sold.        |\n"
    "|7. Remoce a car record.                  |\n"
    "|8. Sort carts by best-selling brand.     |\n"
    "|9. Exit.                                 |\n"
    "+-----------------------------------------+\n"
);

int read_in_option() {
    std::cout << ">> ";
    int opt; std::cin >> opt;
    return opt;
}

void add_new_car_handler() {
    std::cout << "please input the infomation of the car\n";
    car_t car;
    std::cout << " + brand > ";
    std::cin >> car.brand;

    std::cout << " + color > ";
    std::cin >> car.color;

    std::cout << " + price(usd) > ";
    std::cin >> car.price_in_usd;

    std::cout << " + country of manufacture > ";
    std::cin >> car.country_of_manufacture;

    std::cout << " + year_of_manufacture > ";
    std::cin >> car.year_of_manufacture;
    car.self_generate_car_id(random_generator);

    cars.add_car(car);
}


void modify_car_data_handler() {
    std::string car_id;
    car_t target;
    do {
        std::cout << "Input the car id that you want to modify: ";
        std::cin >> car_id;
        target = cars.find_car_by_id(car_id);
    } while(cars.is_null_car(target)
        && (std::cout << "Wrong car id, please input again." << std::endl));

    std::cout << (
        "Input the fieid id that you want to modify:\n"
        " - 1. Brand.\n"
        " - 2. Color.\n"
        " - 3. Price.\n"
        " - 4. Country of Manufacture.\n"
        " - 5. Year of Manufacture.\n"
        " - 6. I do not want to change.\n"
    );
    int opt;
    std::string old_value;
    double old_db_value;
    switch ((std::cin >> opt), opt)
    {
        case 1:
            std::cout << "Input the new brand: ";
            old_value = target.brand;
            std::cin >> target.brand;
            std::cout << "Change brand of " << target.car_id
                << " from " << old_value << " to " << target.brand << std::endl;
            break;
        case 2:
            std::cout << "Input the new color: ";
            old_value = target.color;
            std::cin >> target.color;
            std::cout << "Change color of " << target.car_id
                << " from " << old_value << " to " << target.color << std::endl;
            break;
        case 3:
            std::cout << "Input the new price: ";
            old_db_value = target.price_in_usd;
            std::cin >> target.price_in_usd;
            std::cout << "Change price of " << target.car_id
                << " from " << old_db_value << " to " << target.price_in_usd << std::endl;
            break;
        case 4:
            std::cout << "Input the new country of manufacture: ";
            old_value = target.country_of_manufacture;
            std::cin >> target.country_of_manufacture;
            std::cout << "Change country of manufacture of " << target.car_id
                << " from " << old_value << " to " << target.country_of_manufacture << std::endl;
            break;
        case 5:
            std::cout << "Input the new year of manufacture: ";
            old_db_value = target.year_of_manufacture;
            std::cin >> target.year_of_manufacture;
            std::cout << "Change year of manufacture of " << target.car_id
                << " from " << old_db_value << " to " << target.year_of_manufacture << std::endl;
            break;
        case 6:
            std::cout << "Do not change the car data." << std::endl;
            break;
        default:
            std::cout << "Invalid option." << std::endl;
            break;
    }
    cars.find_car_by_id(car_id) = target;
}


void display_all_cars_handler() {
    auto _cars = cars.list_all_cars_sort_by([](const car_t &lhs, const car_t &rhs) {
        return 1;
    });
    // print the header
    std::vector<std::string> header = {
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


int main() {
    int current_option;
    while((std::cout << menu_prompt),
        (current_option = read_in_option()) != op_exit) {

        switch(current_option) {
            case op_add_new_car: add_new_car_handler(); break;
            case op_modify_car_data: modify_car_data_handler(); break;
            case op_display_all_cars: display_all_cars_handler(); break;
            case op_sort_cars_by_brand_or_price:
                std::cout << "Sort cars by brand or price\n";
                break;
            case op_search_for_a_car_by_its_unique_id:
                std::cout << "Search for a car by its unique ID\n";
                break;
            case op_track_the_number_of_cars_sold:
                std::cout << "Track the number of cars sold\n";
                break;
            case op_remove_a_car_record:
                std::cout << "Remove a car record\n";
                break;
            case op_sort_carts_by_best_selling_brand:
                std::cout << "Sort carts by best-selling brand\n";
                break;
            default:
                std::cout << "Invalid option\n";
                break;
        }
    }
    std::cout << "Bye!" << std::endl;
    return 0;
}