#include <iostream>
#include "car.hpp"
#include "recording.hpp"

int main() {
    record_list records;

    // 添加销售记录
    records.add_sale("Apple", 10);
    records.add_sale("Banana", 5);
    records.add_sale("Apple", 15);
    records.add_sale("Orange", 7);

    // 查找销售记录
    std::cout << "Apple sales count: " << records.find_sale("Apple") << std::endl;
    std::cout << "Banana sales count: " << records.find_sale("Banana") << std::endl;
    std::cout << "Orange sales count: " << records.find_sale("Orange") << std::endl;
    std::cout << "Grapes sales count: " << records.find_sale("Grapes") << std::endl; // 未找到产品

    // 打印所有销售记录
    std::cout << "All sales records:" << std::endl;
    records.print_sales();


    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}
