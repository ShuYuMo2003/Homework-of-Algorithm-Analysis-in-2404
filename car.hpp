#ifndef CAR_HPP
#define CAR_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
#include <ctime>
#include <chrono>
#include <sstream>
#include <cassert>
#include <iomanip>
#include <optional>
#include <string>
#include <functional>

class car_t
{
public:
    std::string car_id;
    std::string brand;
    std::string color;
    std::string country_of_manufacture;
    int year_of_manufacture;
    double price_in_usd;
    int number_of_sold;
    // int sell_count = 0;

    car_t() : car_id("null"), number_of_sold(0) {};
    car_t(const std::string &car_id,
          const std::string &brand,
          const std::string &color,
          const std::string &country_of_manufacture,
          const int year_of_manufacture,
          const double price_in_usd) : car_id(car_id),
                                       brand(brand),
                                       color(color),
                                       country_of_manufacture(country_of_manufacture),
                                       year_of_manufacture(year_of_manufacture),
                                       price_in_usd(price_in_usd),
                                       number_of_sold(0)
    {}

    const std::string& get_id() const { return car_id; }
    const std::string& get_brand() const { return brand; }
    const std::string& get_color() const { return color; }
    const std::string& get_country_of_manufacture() const { return country_of_manufacture; }
    int get_year_of_manufacture() const { return year_of_manufacture; }
    double get_price() const { return price_in_usd; }
    std::string get_string_price() {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(4) << price_in_usd;
        return stream.str();
    }

    void self_generate_car_id(std::mt19937 &random_generator) {
        std::string prefix = brand.substr(0, 3);
        std::uniform_int_distribution<int> dis(int(1e6), int(1e7) - 1);
        car_id = prefix + std::to_string(dis(random_generator));
    }

    int get_number_of_sold() { return number_of_sold; }
    void increase_sold(int delta) { number_of_sold += delta; }
};

class car_list
{
    std::vector<car_t> cars;
    void merge_sort(std::vector<car_t> &arr, int left, int right,
        std::function<bool(const car_t&, const car_t&)> cmp) const;
    void merge(std::vector<car_t> &arr, int left, int mid, int right,
        std::function<bool(const car_t&, const car_t&)> cmp) const;

public:
    car_t null_car_instance;
    bool is_null_car(const car_t & c) const { return &c == &null_car_instance; }
    void add_car(const car_t &car);

    car_t& find_car_by_id(const std::string car_id);
    void remove_car(const std::string& car_id);

    void sell_car(const std::string &brand,
                        const std::string &color,
                        const std::string &country_of_manufacture,
                        const int year_of_manufacture,
                        const double price_in_usd
                        );

    car_t get_best_sell_car() const;

    std::vector<car_t> get_all_cars() const {return cars;};
    std::vector<car_t> list_all_cars_sort_by(std::function<bool(const car_t&, const car_t&)> cmp) const;
    std::vector<car_t> list_all_cars() {
        return list_all_cars_sort_by([](const car_t &lhs, const car_t &rhs) {
            return 1;
        });
    }
    void sort_by(std::function<bool(const car_t&, const car_t&)> cmp) {
        cars = list_all_cars_sort_by(cmp);
    }
};


// Implement the functions in car_list.
void car_list::add_car(const car_t &car)
{
    for(auto e_car : cars) {
        assert(e_car.get_id() != car.get_id());
    }
    cars.push_back(car);
}
void car_list::remove_car(const std::string& car_id) {
    auto it = std::remove_if(cars.begin(), cars.end(), [&car_id](const car_t& car) {
        return car.get_id() == car_id;
    });
    if (it != cars.end()) {
        cars.erase(it, cars.end());
    }
}

car_t& car_list::find_car_by_id(const std::string car_id) {
    for (car_t& car : cars){
        if (car.get_id() == car_id)
            return car;
    }
    return null_car_instance;
}

/*
void car_list::sell_car(const std::string& brand,
                        const std::string& color,
                        const std::string& country_of_manufacture,
                        const int year_of_manufacture,
                        const double price_in_usd
                        ) {


    std::string prefix = brand.substr(0, 3); // 获取品牌的前三位
    std::string car_ID;
    //ID查重
    do {
        car_ID = prefix + std::to_string(generateRandom());
    } while (isDuplicateID(cars, car_ID));
    //添加汽车到汽车列表中
    add_car(car_ID, brand, color, country_of_manufacture, year_of_manufacture, price_in_usd);
    //添加销量到销售列表
    // recordList.add_sale(brand, 1);
}
*/

// std::function 期望传递一个函数对象，可以传递一个 lambda 表达式。
// 用于定义对象的比较规则，例如按照价格升序排序。
// 可以重写为其他的实现方式。
//Q7：通过ID,价格或者品牌进行归并排序
std::vector<car_t> car_list::list_all_cars_sort_by(std::function<bool(const car_t&, const car_t&)> cmp) const {
    std::vector<car_t> sorted_cars = cars;
    merge_sort(sorted_cars, 0, sorted_cars.size() - 1, cmp);
    return sorted_cars;
}
void car_list::merge_sort(std::vector<car_t> &arr, int left, int right, std::function<bool(const car_t&, const car_t&)> cmp) const {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid, cmp);
        merge_sort(arr, mid + 1, right, cmp);
        merge(arr, left, mid, right, cmp);
    }
}
void car_list::merge(std::vector<car_t> &arr, int left, int mid, int right, std::function<bool(const car_t&, const car_t&)> cmp) const {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<car_t> L(n1);
    std::vector<car_t> R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (cmp(L[i], R[j])) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}


// // 返回列表（？）
// car_t car_list::search_best_selling_car_by_brand(const std::string &brand,record_list &recordList) const {
//     std::map<std::string, int> sale_count = recordList.get_sale_count();

//     bubble_sort_by_selling(sorted_cars);
//     int index = binary_search_best_selling_car(sorted_cars, brand);
//     if (index != -1) {
//         return sorted_cars[index];
//     } else {
//         throw std::runtime_error("Brand not found");
//     }
// }



#endif // CAR_HPP添加get方法