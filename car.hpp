#ifndef CAR_HPP
#define CAR_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <functional>

class car_t
{
    std::string car_id;
    std::string brand;
    std::string color;
    std::string country_of_manufacture;
    int year_of_manufacture;
    double price_in_usd;
    int sell_count = 0;

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
};

class car_list
{
    std::vector<car_t> cars;

public:
    void add_car(const std::string &car_id,
          const std::string &brand,
          const std::string &color,
          const std::string &country_of_manufacture,
          const int year_of_manufacture,
          const double price_in_usd);

    car_t& find_car_by_id(const std::string car_id);

    std::vector<car_t> list_all_cars_sort_by(std::function<bool(const car_t&, const car_t&)> cmp) const;

    void sell_car(const std::string car_id, const int& howMany);

    car_t get_best_sell_car() const;
};



// Implement the functions in `car_list`.
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

car_t& car_list::find_car_by_id(const std::string car_id) {}

// std::function 期望传递一个函数对象，可以传递一个 lambda 表达式。
// 用于定义对象的比较规则，例如按照价格升序排序。
// 可以重写为其他的实现方式。
std::vector<car_t> car_list::list_all_cars_sort_by(std::function<bool(const car_t&, const car_t&)> cmp) const {}

void car_list::sell_car(const std::string car_id, const int& howMany) {}

// 返回列表（？）
car_t car_list::get_best_sell_car() const {}

#endif // CAR_HPP