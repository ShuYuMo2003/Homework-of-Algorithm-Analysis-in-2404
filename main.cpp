#include <iostream>
#include <string>
#include <random>
#include <fstream>
#include <set>

#include "car.hpp"
#include "recording.hpp"
#include "user.hpp"
#include "utils.hpp"

car_list cars;
record_list records;
user_list users = ([]{
    std::ifstream file("user.txt");
    if (!file.is_open()) {
        std::cout << " Not Found User File, Created a new one" << std::endl;
        std::ofstream out("user.txt");
        out << "admin admin" << std::endl;
        out.flush();
        out.close();
        file.open("user.txt");
    }

    if(!file.is_open()) {
        std::cerr << "Failed to open user file." << std::endl;
        exit(1);
    }

    user_list _users;
    std::string username, password;
    while(file >> username >> password) {
        _users.add_customer(username, password);
    }

    return _users;
})();

// fixed seed for reproducibility in debugging.
std::mt19937 random_generator((unsigned int)'ytq');


enum OPT {
    op_add_new_car = 1,
    op_modify_car_data = 2,
    op_remove_a_car = 3,

    op_sort_cars_by_brand_or_price = 4,
    op_search_for_a_car_by_its_unique_id = 5,
    op_search_best_selling_brand = 6,
    op_display_all_cars = 7,
    op_display_cars_list_by_brand_or_price = 8,
    op_track_the_number_of_cars_sold = 9,

    op_create_new_bill_to_purchase_cars = 10,
    op_search_bills_by_customerID_or_Date = 11,
    op_search_bills_by_date_range = 12,
    op_display_all_bills = 13,
    op_exit = 14
};

const std::string menu_prompt = (
    "+---------------------- Car Menu ----------------------+\n"
    "| 1. Add a new car.                                    |\n"
    "| 2. Modify car data.                                  |\n"
    "| 3. Remove a car.                                     |\n"
    "+------------------------------------------------------+\n"
    "| 4. Sort cars by brand or price.                      |\n"
    "| 5. Search car by its unique ID.                      |\n"
    "| 6. Search best-selling brand.                        |\n"
    "| 7. Display all cars                                  |\n"
    "| 8. Display sorted cars list by brand price or id.    |\n"
    "| 9. Track the number of the car sold.                 |\n"
    "+------------------------------------------------------+\n"
    "| 10. Create new bill to purchase cars.                |\n"
    "| 11. Search bills by `customerID` or `Date`.          |\n"
    "| 12. Search bills by date range.                      |\n"
    "| 13. Display all bills.                               |\n"
    "| 14. Exit.                                            |\n"
    "+------------------------------------------------------+\n"
);

bool login(){
    std::cerr << "--- For debugging purpose, the default username and password is `admin` ---\n" << std::endl;
    std::string username, password;
    std::cout << "Please input your username: ";
    std::cin >> username;
    std::cout << "Please input your password: ";
    std::cin >> password;
    if(users.isAuthenticated(username, password)){
        std::cout << "Login successfully. Hello, " << username << std::endl;
        return true;
    } else {
        std::cout << "Invalid username or password, please try again." << std::endl;
        return false;
    }
}

int read_in_option() {
    while(true) {
        std::cout << ">> ";
        std::string line;
        try {
            std::getline(std::cin, line);
            if(!line.size()) continue;
            int opt = std::stoi(line);
            return opt;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid input\n";
        } catch (const std::out_of_range& e) {
            std::cerr << "Invalid input\n";
        }
    }
    return -1;
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
    std::cout << " + ? by brand/price/id: ";
    std::string opt; std::cin >> opt;
    std::vector<car_t> _cars;
    if(opt[0] == 'b') {
        _cars = cars.list_all_cars_sort_by([](const car_t & lhs, const car_t & rhs) -> bool{
            return lhs.get_brand() < rhs.get_brand();
        });
    } else if(opt[0] == 'p'){
        _cars = cars.list_all_cars_sort_by([](const car_t & lhs, const car_t & rhs) -> bool{
            return lhs.get_price() < rhs.get_price();
        });
    } else {
        _cars = cars.list_all_cars_sort_by([](const car_t & lhs, const car_t & rhs) -> bool{
            return lhs.get_id() < rhs.get_id();
        });
    }
    print_car_t_list(_cars);
}

void search_for_a_car_by_id_handler() {
    std::cout << " + Unique Id: ";
    std::string id; std::cin >> id;
    auto car = cars.find_car_by_id(id);
    if(cars.is_null_car(car)) {
        std::cout << "Not Found Such Car." << std::endl;
    } else {
        std::vector<car_t> _cars = {car};
        print_car_t_list(_cars);
    }
}

void track_the_number_of_cars_sold_handler() {
    std::cout << "Input the car id for which you want to track the number of cars sold: ";
    std::string car_id;
    std::cin >> car_id;
    auto car = cars.find_car_by_id(car_id);
    if(cars.is_null_car(car)) {
        std::cout << "Not Found Such Car." << std::endl;
    } else {
        print_items({"Car ID", "Number of Cars Sold"}, {car.get_id(), std::to_string(records.get_car_sold_by_brandname(car.get_brand()))});
    }
}

void remove_a_car_handler() {
    std::cout << "Input the car id that you want to remove: ";
    std::string car_id;
    std::cin >> car_id;
    auto car = cars.find_car_by_id(car_id);
    if(cars.is_null_car(car)) {
        std::cout << "Not Found Such Car." << std::endl;
    } else {
        cars.remove_car(car_id);
        std::cout << "Car " << car_id << " removed successfully." << std::endl;
    }
}

void sort_cars_by_best_selling_brand_handler() {
    std::set<std::string> brands;
    for(auto & car : cars.list_all_cars()) {
        brands.insert(car.get_brand());
    }
    std::vector<std::string> brand_list(brands.begin(), brands.end());
    auto best_selling_brand = records.search_best_selling_car_by_brand(brand_list);
    print_items({"Brand", "Number of Cars Sold"}, {best_selling_brand.first, std::to_string(best_selling_brand.second)});
}

void create_new_bill_to_purchase_cars_handler() {
    std::cout << "Please input the information of the bill" << std::endl;
    bill_t bill;
    bill.self_generate_record_id(random_generator);
    std::cout << " + Customer ID: ";
    std::cin >> bill.customer_id;
    std::cout << " + Customer Name: ";
    std::cin >> bill.customer_name;

    std::cout << " + Car Id: ";
    std::string car_id;
    std::cin >> car_id;
    auto car = cars.find_car_by_id(car_id);
    if (cars.is_null_car(car)) {
        std::cout << "Not Found Such Car." << std::endl;
        return;
    }
    bill.paid_cars = car;

    std::cout << " + How many cars you want to buy: ";
    std::cin >> bill.number_of_sold;

    std::cout << " +  Date(as format `YY mm dd`, eg: `2024 7 6`): ";
    std::cin >> std::get<0>(bill.date) >> std::get<1>(bill.date) >> std::get<2>(bill.date);

    std::cout << " + Any applicable discount: ";
    std::cin >> bill.any_applicable_discount;

    bill.compute_price();

    records.add_record(bill);
}

void search_bills_by_customerID_or_Date_handler() {
    std::cout << " + Search by `customerID`(c) or `Date`(d)? ";
    std::string opt; std::cin >> opt;
    bill_t bill;
    if (opt[0] == 'c') {
        std::cout << " + Input the customer ID: ";
        std::string customer_id;
        std::cin >> customer_id;
        bill = records.find_record_by_customer_id(customer_id);
    } else {
        std::cout << " + Input the date(as format `YY mm dd`, eg: `2024 7 6`): ";
        std::tuple<int, int, int> date;
        std::cin >> std::get<0>(date) >> std::get<1>(date) >> std::get<2>(date);
        bill = records.find_record_by_date(date);
    }
    if(records.is_non_bill_instance(bill)) {
        std::cout << "Not Found Such Bill." << std::endl;
    } else {
        std::vector<bill_t> _bills = {bill};
        print_bill_t_list(_bills);
    }
}

void search_bills_by_date_range_handler() {
    std::tuple<int, int, int> date1, date2;
    std::cout << " + Input the lower date range(as format `YY mm dd`, eg: `2024 7 6`): ";
    std::cin >> std::get<0>(date1) >> std::get<1>(date1) >> std::get<2>(date1);
    std::cout << " + Input the upper date range(as format `YY mm dd`, eg: `2024 7 6`): ";
    std::cin >> std::get<0>(date2) >> std::get<1>(date2) >> std::get<2>(date2);

    auto bills = records.generate_purchase_report_by_date_range(date1, date2);
    print_bill_t_list(bills.first);
    std::cout << "Total Bills' Price: " << bills.second << std::endl;
}

void display_all_bills_handler() {
    print_bill_t_list(records.get_records());
}

int main() {
    while(!login()) ;

    int current_option;
    while((std::cout << menu_prompt),
        (current_option = read_in_option()) != op_exit) {

        switch(current_option) {
// menu part 1 BEGIN
            case op_add_new_car:
                add_new_car_handler();
                break;
            case op_modify_car_data:
                modify_car_data_handler();
                break;
            case op_remove_a_car:
                remove_a_car_handler();
                break;
// menu part 1 END

// menu part 2 BEGIN
            case op_sort_cars_by_brand_or_price:
                sort_cars_by_brand_or_price_handler();
                break;
            case op_search_for_a_car_by_its_unique_id:
                search_for_a_car_by_id_handler();
                break;
            case op_search_best_selling_brand:
                sort_cars_by_best_selling_brand_handler();
                break;
            case op_display_all_cars:
                display_all_cars_handler();
                break;
            case op_display_cars_list_by_brand_or_price:
                display_car_list_sorted_by_brand_or_price();
                break;
            case op_track_the_number_of_cars_sold:
                track_the_number_of_cars_sold_handler();
                break;
// menu part 2 END

// menu part 3 BEGIN
            case op_create_new_bill_to_purchase_cars:
                create_new_bill_to_purchase_cars_handler();
                break;
            case op_search_bills_by_customerID_or_Date:
                search_bills_by_customerID_or_Date_handler();
                break;
            case op_search_bills_by_date_range:
                search_bills_by_date_range_handler();
                break;
            case op_display_all_bills:
                display_all_bills_handler();
                break;
// menu part 3 END
            case op_exit:
                break;
            default:
                std::cout << "Invalid option\n";
                break;
        }
        std::cout << "\n======================== END OF OUTPUT ========================\n" << std::endl;
    }
    std::cout << "Bye!" << std::endl;
    return 0;
}