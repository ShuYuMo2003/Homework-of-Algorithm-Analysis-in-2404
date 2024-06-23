#ifndef CAR_HPP
#define CAR_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
#include <ctime>

#include "recording.hpp"

class car_t
{
    std::string car_id;
    std::string brand;
    std::string color;
    std::string country_of_manufacture;
    int year_of_manufacture;
    double price_in_usd;
    // int sell_count = 0;

public:
    car_t() = default;
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
                                       price_in_usd(price_in_usd)
    {}

    const std::string& get_id() const { return car_id; }
    const std::string& get_brand() const { return brand; }
    const std::string& get_color() const { return color; }
    const std::string& get_country_of_manufacture() const { return country_of_manufacture; }
    int get_year_of_manufacture() const { return year_of_manufacture; }
    double get_price() const { return price_in_usd; }
    // int get_sell_count() const { return sell_count; }

    // void increment_sell_count() { sell_count++; }
};

class car_list
{
    std::vector<car_t> cars;
    
    //Q7：通过ID,价格或者品牌进行归并排序
    void merge_sort(std::vector<car_t> &arr, int left, int right, std::function<bool(const car_t&, const car_t&)> cmp) const;
    void merge(std::vector<car_t> &arr, int left, int mid, int right, std::function<bool(const car_t&, const car_t&)> cmp) const;
    //Q8: Bubble + Binary 找按品牌排序的最好销量车
    void bubble_sort_by_selling(std::vector<std::pair<std::string, int>>& vec) const;
    std::pair<std::string, int> binary_search_best_selling_car(const std::vector<std::pair<std::string, int>>& vec) const;

public:
    void add_car(const std::string &car_id,
          const std::string &brand,
          const std::string &color,
          const std::string &country_of_manufacture,
          const int year_of_manufacture,
          const double price_in_usd);

    car_t& find_car_by_id(const std::string car_id);


    void sell_car(  
                    const std::string &brand,
                    const std::string &color,
                    const std::string &country_of_manufacture,
                    const int year_of_manufacture,
                    const double price_in_usd,
                    record_list& recordList
                    );

    car_t get_best_sell_car() const;

    //Q6 :展示未排序的汽车列表
    std::vector<car_t> list_all_cars() const;
    //Q7：通过ID,价格或者品牌进行归并排序
    std::vector<car_t> list_all_cars_sort_by(std::function<bool(const car_t&, const car_t&)> cmp) const;
    //Q8: Bubble + Binary 找按品牌排序的最好销量车并展示
    void search_best_selling_car_by_brand(record_list &recordList) const;

};



// Implement the functions in car_list.
void car_list::add_car(const std::string &car_id,
          const std::string &brand,
          const std::string &color,
          const std::string &country_of_manufacture,
          const int year_of_manufacture,
          const double price_in_usd)
{
    
    cars.push_back(car_t(car_id, brand, color, country_of_manufacture,
                year_of_manufacture, price_in_usd));
}

car_t& car_list::find_car_by_id(const std::string car_id) {
        for (auto& car : cars)
    {
        if (car.get_id() == car_id)
        {
            return car;
        }
    }
    throw std::invalid_argument("Car ID not found");
}

void car_list::sell_car(const std::string &brand,
                        const std::string &color,
                        const std::string &country_of_manufacture,
                        const int year_of_manufacture,
                        const double price_in_usd,
                        record_list& recordList
                        ) {
        std::mt19937 generator(static_cast<unsigned int>(std::time(0))); // 使用当前时间作为种子
        std::uniform_int_distribution<int> distribution(1000000, 9999999); // 生成范围在1000000到9999999之间的随机数
        int random_number = distribution(generator);

        std::string prefix = brand.substr(0, 3); // 获取品牌的前三位
        std::string car_ID = prefix + std::to_string(random_number);
        //添加汽车到汽车列表中
        add_car(car_ID, brand, color, country_of_manufacture, year_of_manufacture, price_in_usd);
        //添加销量到销售列表
        recordList.add_sale(brand, 1);
    }
    
//Q6 :展示未排序的汽车列表
std::vector<car_t> car_list::list_all_cars() const {
    return cars;
}

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
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// //Q8: Bubble + Binary 找按品牌排序的最好销量车
// void car_list::bubble_sort_by_selling(std::vector<car_t> &arr) const {
//     int n = arr.size();
//     for (int i = 0; i < n - 1; ++i) {
//         for (int j = 0; j < n - i - 1; ++j) {
//             // if (arr[j].get_sell_count() > arr[j + 1].get_sell_count()) {
//             //     std::swap(arr[j], arr[j + 1]);
//             // }
//         }
//     }
// }

// int car_list::binary_search_best_selling_car(const std::vector<car_t> &arr, const std::string &brand) const {
//     int left = 0;
//     int right = arr.size() - 1;
//     while (left <= right) {
//         int mid = left + (right - left) / 2;
//         if (arr[mid].get_brand() == brand) {
//             return mid;
//         } else if (arr[mid].get_brand() < brand) {
//             left = mid + 1;
//         } else {
//             right = mid - 1;
//         }
//     }
//     return -1;
// }

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

//Q8: Bubble + Binary 找按品牌排序的最好销量车
 void car_list::search_best_selling_car_by_brand(record_list &recordList) const {
    std::map<std::string, int> sale_count = recordList.get_sale_count();
    std::vector<std::pair<std::string, int>> vec(sale_count.begin(), sale_count.end());
    // 先按冒泡排序得到不同品牌的销量顺序排序
    bubble_sort_by_selling(vec);
    // 再二分查找得到销量最大的品牌和对应销量
    
    std::pair<std::string, int> bestSeller = binary_search_best_selling_car(vec);
    std::cout << "The best seller is " << bestSeller.first << " with " << bestSeller.second << " sales." << std::endl;
 };
// 对map转换的pair进行冒泡排序
 void car_list::bubble_sort_by_selling(std::vector<std::pair<std::string, int>>& vec) const{
    int n = vec.size();
    bool swapped;
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (vec[j].second > vec[j + 1].second) {
                // 交换元素
                std::swap(vec[j], vec[j + 1]);
                swapped = true;
            }
        }
        // 如果本轮没有发生交换，说明已经有序
        if (!swapped) break;
    }
}

// 二分查找函数
std::pair<std::string, int> car_list::binary_search_best_selling_car(const std::vector<std::pair<std::string, int>>& vec) const {

    if (vec.empty()) {
        return {"", 0}; 
    }
    int left = 0;
    int right = vec.size() - 1;
    int mid;
    while (left < right) {
        mid = left + (right - left) / 2;
        if (vec[mid].second < vec[right].second) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    // 返回最大值对应的 pair
    return vec[right];
}
#endif // CAR_HPP添加get方法