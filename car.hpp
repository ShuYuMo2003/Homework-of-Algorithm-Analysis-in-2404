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
    //展示汽车的所有信息
    void print_car_inf() const {
        std::ostringstream oss;
        oss << "Car ID: " << car_id << "\n"
            << "Brand: " << brand << "\n"
            << "Color: " << color << "\n"
            << "Country of Manufacture: " << country_of_manufacture << "\n"
            << "Year of Manufacture: " << year_of_manufacture << "\n"
            << "Price in USD: " << price_in_usd;
        std::cout<<oss.str()<<std::endl;
    }
    // int get_sell_count() const { return sell_count; }

    // void increment_sell_count() { sell_count++; }
};

class car_list
{
    std::vector<car_t> cars;
    int generateRandom() const;
    //Q7：通过ID,价格或者品牌进行归并排序
    void merge_sort(std::vector<car_t> &arr, int left, int right, std::function<bool(const car_t&, const car_t&)> cmp) const;
    void merge(std::vector<car_t> &arr, int left, int mid, int right, std::function<bool(const car_t&, const car_t&)> cmp) const;
    

public:
    void add_car(const std::string &car_id,
          const std::string &brand,
          const std::string &color,
          const std::string &country_of_manufacture,
          const int year_of_manufacture,
          const double price_in_usd);

    car_t& find_car_by_id(const std::string car_id);


    void sell_car(const std::string &brand,
                        const std::string &color,
                        const std::string &country_of_manufacture,
                        const int year_of_manufacture,
                        const double price_in_usd
                        );

    car_t get_best_sell_car() const;

    //Q6 :展示未排序的汽车列表
    void list_all_cars() const;
    //Q7：通过ID,价格或者品牌进行归并排序
    std::vector<car_t> get_all_cars() const {return cars;};
    std::vector<car_t> list_all_cars_sort_by(std::function<bool(const car_t&, const car_t&)> cmp) const;


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
// 随机数生成器
int car_list::generateRandom() const{
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(1000000, 9999999);
    return distribution(generator);
}
// 检查重复ID
bool isDuplicateID(const std::vector<car_t>& cars, const std::string& newID) {
    for (const auto& car : cars) {
        if (car.get_id() == newID) {
            return true; 
        }
    }
    return false; 
}
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

//Q6 :展示未排序的汽车列表
   void car_list::list_all_cars() const {
    std::cout << "Unsorted car list:" << std::endl;
        for(const auto& car : cars) {
            car.print_car_inf();
            std::cout << "--------------------------------------------------" << std::endl;
        }
   };

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