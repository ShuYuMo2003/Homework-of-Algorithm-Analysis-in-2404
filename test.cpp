#include <iostream>
#include <vector>
#include "car.hpp"  
#include "recording.hpp"  
int main() {
    car_list cl;
    record_list recordList;
    //Q7 测试
    
    // 获取所有未排序的汽车列表
    std::vector<car_t> cars = cl.list_all_cars();
    
    std::cout << "Unsorted car list:" << std::endl;
    for (const auto& car : cars) {
        std::cout << car.get_id() << " " << car.get_brand() << " " << car.get_price() << std::endl;
    }
    // 添加一些汽车记录
    cl.sell_car("Toyota", "Red", "Japan", 2020, 30000,recordList);
    cl.sell_car("Honda", "Blue", "Japan", 2019, 25000,recordList);
    cl.sell_car("BMW", "Black", "Germany", 2021, 45000,recordList);
    cl.sell_car("Audi", "White", "Germany", 2018, 41000,recordList);
    cl.sell_car("Audi", "White", "Germany", 2018, 40100,recordList);
    cl.sell_car("Audi", "White", "Germany", 2018, 40010,recordList);
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
    //Q8 测试

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}
