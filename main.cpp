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
    op_display_cars_list_by_brand_or_price = 5,
    op_search_for_a_car_by_its_unique_id = 6,
    op_track_the_number_of_cars_sold = 7,
    op_remove_a_car_record = 8,
    op_sort_carts_by_best_selling_brand = 9,
    op_exit = 10
};

const std::string menu_prompt = (
    "+------------------ Car  Menu ------------------+\n"
    "| 1. Add new car.                               |\n"
    "| 2. Modify car data.                           |\n"
    "| 3. Display all cars                           |\n"
    "| 4. Sort cars by brand or price.               |\n"
    "| 5. Display sorted cars list by brand or price.|\n"
    "| 6. Search for a car by its unique ID.         |\n"
    "| 7. Track the number of cars sold.             |\n"
    "| 8. Remoce a car record.                       |\n"
    "| 9. Sort carts by best-selling brand.          |\n"
    "| 10. Exit.                                     |\n"
    "+-----------------------------------------------+\n"
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
    print_car_t_list(cars.list_all_cars());
}

void sort_cars_by_brand_or_price_handler() {
    std::cout << " + ? by brand/price: ";
    std::string opt; std::cin >> opt;
    if(opt[0] == 'b') {
        cars.sort_by([](const car_t & lhs, const car_t & rhs) -> bool{
            return lhs.get_brand() < rhs.get_brand();
        });
    } else {
        cars.sort_by([](const car_t & lhs, const car_t & rhs) -> bool{
            return lhs.get_price() < rhs.get_price();
        });
    }
    std::cout << "Sort done with key: " << opt << "." << std::endl;
    display_all_cars_handler();
}

void display_car_list_sorted_by_brand_or_price() {
    std::cout << " + ? by brand/price: ";
    std::string opt; std::cin >> opt;
    std::vector<car_t> _cars;
    if(opt[0] == 'b') {
        _cars = cars.list_all_cars_sort_by([](const car_t & lhs, const car_t & rhs) -> bool{
            return lhs.get_brand() < rhs.get_brand();
        });
    } else {
        _cars = cars.list_all_cars_sort_by([](const car_t & lhs, const car_t & rhs) -> bool{
            return lhs.get_price() < rhs.get_price();
        });
    }
    print_car_t_list(_cars);
}

void search_for_a_car_by_id_handler() {
    std::cout << " + Unique Id: ";
    std::string id; std::cin >> id;
    auto car = cars.find_car_by_id(id);
    // print the header
    if(cars.is_null_car(car)) {
        std::cout << "Not Found Such Car." << std::endl;
    } else {
        std::vector<car_t> _cars = {car};
        print_car_t_list(_cars);
    }
}


int main() {
    int current_option;
    while((std::cout << menu_prompt),
        (current_option = read_in_option()) != op_exit) {

        switch(current_option) {
            case op_add_new_car:
                add_new_car_handler();
                break;
            case op_modify_car_data:
                modify_car_data_handler();
                break;
            case op_display_all_cars:
                display_all_cars_handler();
                break;
            case op_sort_cars_by_brand_or_price:
                sort_cars_by_brand_or_price_handler();
                break;
            case op_search_for_a_car_by_its_unique_id:
                search_for_a_car_by_id_handler();
                break;
            case op_display_cars_list_by_brand_or_price:
                display_car_list_sorted_by_brand_or_price();
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