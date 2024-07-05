#ifndef RECORDING_HPP
#define RECORDING_HPP
#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <iostream>
#include "car.hpp"

class bill_t {
    std::string record_id;
    std::string customer_name;
    std::string customer_id;
    double total_price;
    std::string any_applicable_discount;
    std::vector<std::pair<car_t, int>> paid_cars;
    std::tuple<int, int, int> date;

public:
    bill_t() = default;
    bill_t(const std::string& record_id,
           const std::string& customer_name,
           const std::string& customer_id,
           const double& total_price,
           const std::string& any_applicable_discount,
           const std::vector<std::pair<car_t, int>>& paid_cars,
           const std::tuple<int, int, int>& date) : record_id(record_id),
                                                    customer_name(customer_name),
                                                    customer_id(customer_id),
                                                    total_price(total_price),
                                                    any_applicable_discount(any_applicable_discount),
                                                    paid_cars(paid_cars),
                                                    date(date)
    {}

    const std::string& get_customer_id() const { return customer_id; }
    const std::tuple<int, int, int>& get_date() const { return date; }
    const std::string& get_car_id() const { return paid_cars.front().first.get_id(); }
    const std::string& get_car_brand() const { return paid_cars.front().first.get_brand(); }
    const std::string& get_car_color() const { return paid_cars.front().first.get_color(); }
    double get_car_price() const { return paid_cars.front().first.get_price(); }
};

class record_list {
    std::vector<bill_t> records;
    std::map<std::string, int> sale_count; //记录某个品牌的总销售量
    //Q8: Bubble + Binary 找按品牌排序的最好销量车
    void bubble_sort_by_selling(std::vector<std::pair<std::string, int>>& vec) const;
    std::pair<std::string, int> binary_search_best_selling_car(const std::vector<std::pair<std::string, int>>& vec) const;
public:
    bill_t add_record(const std::string& record_id,
                      const std::string& customer_name,
                      const std::string& customer_id,
                      const double& total_price,
                      const std::string& any_applicable_discount,
                      const std::vector<std::pair<car_t, int>>& paid_cars,
                      const std::tuple<int, int, int>& date);
    bill_t find_record_by_customer_id(const std::string& customer_id);
    bill_t find_record_by_date(const std::tuple<int, int, int> date);
    std::vector<bill_t> find_records_by_date_range(const std::tuple<int, int, int> start_date, const std::tuple<int, int, int> end_date);
    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::tuple<int, int, int>, double>> generate_purchase_report(const std::tuple<int, int, int>& start_date, const std::tuple<int, int, int>& end_date);
    // 处理销量的方法
    void add_sale(const std::string& product, int count);
    void add_sale(const std::string& product);
    int find_sale(const std::string& product) const;
    void print_sales() const;
    std::map<std::string, int> get_sale_count() const { return sale_count; };
    //Q8：Bubble + Binary 找按品牌排序的最好销量车
    void search_best_selling_car_by_brand() const;
};

bill_t record_list::add_record(const std::string& record_id,
                               const std::string& customer_name,
                               const std::string& customer_id,
                               const double& total_price,
                               const std::string& any_applicable_discount,
                               const std::vector<std::pair<car_t, int>>& paid_cars,
                               const std::tuple<int, int, int>& date) {
    bill_t bill = bill_t(record_id, customer_name, customer_id, total_price, any_applicable_discount, paid_cars, date);
    records.push_back(bill);
    return bill;
}

//线性查找
bill_t record_list::find_record_by_customer_id(const std::string& customer_id) {
    for (const auto& record : records) {
        if (record.get_customer_id() == customer_id) {
            return record;
        }
    }
    throw std::runtime_error("Record not found");
}

//线性查找
bill_t record_list::find_record_by_date(const std::tuple<int, int, int> date) {
    for (const auto& record : records) {
        if (record.get_date() == date) {
            return record;
        }
    }
    throw std::runtime_error("Record not found");
}

// lower_bound 实现
std::vector<bill_t>::iterator lower_bound(std::vector<bill_t>::iterator first,
                                          std::vector<bill_t>::iterator last,
                                          const std::tuple<int, int, int>& start_date) {
    while (first < last) {
        auto middle = first + (last - first) / 2;
        if (middle->get_date() < start_date) {
            first = middle + 1;
        } else {
            last = middle;
        }
    }
    return first;
}

// upper_bound 实现
std::vector<bill_t>::iterator upper_bound(std::vector<bill_t>::iterator first,
                                          std::vector<bill_t>::iterator last,
                                          const std::tuple<int, int, int>& end_date) {
    while (first < last) {
        auto middle = first + (last - first) / 2;
        if (end_date < middle->get_date()) {
            last = middle;
        } else {
            first = middle + 1;
        }
    }
    return first;
}

std::vector<bill_t>::iterator partition(std::vector<bill_t>::iterator & first, std::vector<bill_t>::iterator & last) {
    auto pivot = first + (last - first) / 2;
    auto pivot_value = pivot->get_car_price();
    // auto tight_last_bound = last - 1;
    std::iter_swap(pivot, last - 1);
    auto store = first;
    for (auto it = first; it < last - 1; ++it) {
        if (it->get_car_price() < pivot_value) {
            std::iter_swap(it, store);
            ++store;
        }
    }
    std::iter_swap(store, last - 1);
    return store;
}

//快速排序
void quicksort(std::vector<bill_t>::iterator first, std::vector<bill_t>::iterator last) {
    if (first < last - 1) {
        auto pivot = partition(first, last);
        quicksort(first, pivot);
        quicksort(pivot + 1, last);
    }
}

//生成report
std::vector<
    std::tuple<std::string, std::string, std::string, std::string, std::tuple<int, int, int>, double>>
    record_list::generate_purchase_report(
        const std::tuple<int, int, int>& start_date, const std::tuple<int, int, int>& end_date
    ) {
    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::tuple<int, int, int>, double>> report;

    auto lower = lower_bound(records.begin(), records.end(), start_date);
    auto upper = upper_bound(records.begin(), records.end(), end_date);

    std::vector<bill_t> sorted_records(lower, upper);
    quicksort(sorted_records.begin(), sorted_records.end());

    double total_price = 0.0;
    for (const auto& record : sorted_records) {
        total_price += record.get_car_price();
    }

    for (const auto& record : sorted_records) {
        report.push_back(std::make_tuple(record.get_car_id(), record.get_car_brand(), record.get_car_color(),
                                         record.get_customer_id(), record.get_date(), record.get_car_price()));
    }

    report.push_back(std::make_tuple("", "", "", "Total Price of Sold Cars", std::tuple<int, int, int>(), total_price));

    return report;
}

// 处理销量的方法
 // 添加或更新销售记录
    void record_list::add_sale(const std::string& brand, int count) {
        sale_count[brand] += count;
    }
     void record_list::add_sale(const std::string& brand) {
        sale_count[brand] += 1;
    }

    // 查找销售记录
    int record_list::find_sale(const std::string& brand) const {
        auto it = sale_count.find(brand);
        if (it != sale_count.end()) {
            return it->second;
        } else {
            std::cerr << "Product not found: " << brand << std::endl;
            return 0; // 或者选择其他适当的返回值或抛出异常
        }
    }

    // 打印所有销售记录
    void record_list::print_sales() const {
        for (const auto& pair : sale_count) {
            std::cout << "Product: " << pair.first << ", Sales Count: " << pair.second << std::endl;
        }
    }

    //Q8: Bubble + Binary 找按品牌排序的最好销量车
    void record_list::search_best_selling_car_by_brand() const {
        std::vector<std::pair<std::string, int>> vec(sale_count.begin(), sale_count.end());
        // 先按冒泡排序得到不同品牌的销量顺序排序
        bubble_sort_by_selling(vec);
        // 再二分查找得到销量最大的品牌和对应销量

        std::pair<std::string, int> bestSeller = binary_search_best_selling_car(vec);
        std::cout << "The best seller is " << bestSeller.first << " with " << bestSeller.second << " sales." << std::endl;
    };
    //对map转换的pair进行冒泡排序
    void record_list::bubble_sort_by_selling(std::vector<std::pair<std::string, int>>& vec) const{
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
    std::pair<std::string, int> record_list::binary_search_best_selling_car(const std::vector<std::pair<std::string, int>>& vec) const {

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
#endif // RECORDING_HPP
