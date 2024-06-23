#include <iostream>
#include <vector>
#include "car.hpp"  
#include "recording.hpp"  
#include "user.hpp"
int main() {
    //创建随机数种子
    srand(time(0));
    //创建记录表
    car_list cl;
    record_list recordList;
    customer_list customerList;

    // 创建用户
    customerList.add_customer("John Doe", "JD1234", "pd1");
    std::string userID;
    std::string userPwd;

    // 添加一些汽车记录
    cl.sell_car("Toyota", "Red", "Japan", 2020, 300000);
    recordList.add_sale("Toyota",1);
    cl.sell_car("Honda", "Blue", "Japan", 2019, 250000);
    cl.sell_car("Honda", "Blue", "Japan", 2019, 250000);
    recordList.add_sale("Honda",2);
    cl.sell_car("BMW", "Black", "Germany", 2021, 450400);
    cl.sell_car("BMW", "Black", "Germany", 2021, 450500);
    recordList.add_sale("BMW",2);
    cl.sell_car("Audi", "White", "Germany", 2018, 410000);
    cl.sell_car("Audi", "White", "Germany", 2018, 401000);
    cl.sell_car("Audi", "White", "Germany", 2018, 400100);
    recordList.add_sale("Audi",3);

    //-------------------------------------------------------------------------Q1 测试
    while (true) {
        std::cout << "Enter UserID: ";
        std::getline(std::cin, userID);

        std::cout << "Enter Pwd: ";
        std::getline(std::cin, userPwd);

        if (customerList.isAuthenticated(userID, userPwd)) {
            std::cout << "Welcome" << std::endl;
            break;
        } else {
            std::cout << "Please try again" << std::endl;
        }
    }


    //-------------------------------------------------------------------------Q6 测试
    cl.list_all_cars();

    //-------------------------------------------------------------------------Q7 测试
    // 获取所有未排序的汽车列表
    std::vector<car_t> cars = cl.get_all_cars();
    
    std::cout << "Unsorted car list:" << std::endl;
    for (const auto& car : cars) {
        std::cout << car.get_id() << " " << car.get_brand() << " " << car.get_price() << std::endl;
    }
    // 按 ID 排序
    auto sorted_by_id = cl.list_all_cars_sort_by([](const car_t& a, const car_t& b) {
        return a.get_id() < b.get_id();
    });
    std::cout << "Sorted by ID:" << std::endl;
    for (const auto& car : sorted_by_id) {
        std::cout << car.get_id() << " " << car.get_brand() << " " << car.get_price() << std::endl;
    }
    // 按价格排序
    auto sorted_by_price = cl.list_all_cars_sort_by([](const car_t& a, const car_t& b) {
        return a.get_price() < b.get_price();
    });
    std::cout << "Sorted by Price:" << std::endl;
    for (const auto& car : sorted_by_price) {
        std::cout << car.get_id() << " " << car.get_brand() << " " << car.get_price() << std::endl;
    }
    // 按品牌排序
    auto sorted_by_brand = cl.list_all_cars_sort_by([](const car_t& a, const car_t& b) {
        return a.get_brand() < b.get_brand();
    });
    std::cout << "Sorted by Brand:" << std::endl;
    for (const auto& car : sorted_by_brand) {
        std::cout << car.get_id() << " " << car.get_brand() << " " << car.get_price() << std::endl;
    }

    //-------------------------------------------------------------------------Q8 测试
    recordList.search_best_selling_car_by_brand();

    // 防闪退
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}
