#ifndef RECORDING_HPP
#define RECORDING_HPP
#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <iostream>
#include "car.hpp"

void print_date(const std::tuple<int, int, int>& date) {
    std::cout << std::get<0>(date) << "/" << std::get<1>(date) << "/" << std::get<2>(date);
}

class bill_t {
public:

    std::string record_id;
    std::string customer_name;
    std::string customer_id;
    double total_price;
    std::string any_applicable_discount;
    car_t paid_cars;
    int number_of_sold;
    std::tuple<int, int, int> date;


    bill_t() = default;
    bill_t(const std::string& record_id,
           const std::string& customer_name,
           const std::string& customer_id,
           const double& total_price,
           const std::string& any_applicable_discount,
           const car_t & paid_cars,
           const int & number_of_sold,
           const std::tuple<int, int, int>& date) : record_id(record_id),
                                                    customer_name(customer_name),
                                                    customer_id(customer_id),
                                                    total_price(total_price),
                                                    any_applicable_discount(any_applicable_discount),
                                                    paid_cars(paid_cars),
                                                    number_of_sold(number_of_sold),
                                                    date(date)
    {}

    void self_generate_record_id(std::mt19937 &random_generator) {
        std::string prefix = "B";
        std::uniform_int_distribution<int> dis(int(1e6), int(1e7) - 1);
        record_id = prefix + std::to_string(dis(random_generator));
    }

    const std::string& get_customer_id() const { return customer_id; }
    const std::tuple<int, int, int>& get_date() const { return date; }
    const std::string& get_car_id() const { return paid_cars.get_id(); }
    const std::string& get_car_brand() const { return paid_cars.get_brand(); }
    const std::string& get_car_color() const { return paid_cars.get_color(); }
    double get_car_price() const { return paid_cars.get_price(); }
    void compute_price() {
        total_price = paid_cars.get_price() * number_of_sold;
    }
    double get_total_price() const { return total_price; }
};

class record_list {
    std::vector<bill_t> records;
    std::vector<std::pair<std::string, int>> sale_count;
public:
    bill_t non_bill_instance;
    bool is_non_bill_instance(const bill_t & bil) {
        return &bil == &non_bill_instance;
    }
    bill_t add_record(bill_t new_record) {
        records.push_back(new_record);
        add_sale(new_record.get_car_brand(), new_record.number_of_sold);
        return new_record;
    }
    bill_t find_record_by_customer_id(const std::string& customer_id);
    bill_t find_record_by_date(const std::tuple<int, int, int> date);
    std::pair<std::vector<bill_t>, double> generate_purchase_report_by_date_range(
        const std::tuple<int, int, int>& start_date,
        const std::tuple<int, int, int>& end_date
    );

    void add_sale(const std::string& product, int count);
    void print_sales() const;
    int& get_car_sold_by_brandname(std::string brandname);
    std::pair<std::string, int> search_best_selling_car_by_brand(std::vector<std::string> brands);
    void bubble_sort_brand_sale_pair_by_sale();
    std::vector<bill_t>& get_records() { return records; }
};

bill_t record_list::find_record_by_customer_id(const std::string& customer_id) {
    for (const auto& record : records) {
        if (record.get_customer_id() == customer_id) {
            return record;
        }
    }
    return non_bill_instance;
}

bill_t record_list::find_record_by_date(const std::tuple<int, int, int> date) {
    for (const auto& record : records) {
        if (record.get_date() == date) {
            return record;
        }
    }
    return non_bill_instance;
}

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

std::vector<bill_t>::iterator partition(std::vector<bill_t>::iterator & first, std::vector<bill_t>::iterator & last,
                                        std::function<bool(const bill_t&, const bill_t&)> cmp) {
    auto pivot = first + (last - first) / 2;
    auto pivot_value = *pivot;
    // std::cout << "Pivot: "; print_date(pivot->get_date()); std::cout << std::endl;
    std::iter_swap(pivot, last - 1);
    auto store = first;
    for (auto it = first; it < last - 1; ++it) {
        if (cmp(*it, pivot_value)) {
            std::iter_swap(it, store);
            ++store;
        }
    }
    std::iter_swap(store, last - 1);
    // for(auto it = first; it < last; ++it) {
    //     print_date(it->get_date());
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;
    return store;
}

void quicksort(std::vector<bill_t>::iterator first, std::vector<bill_t>::iterator last,
                std::function<bool(const bill_t&, const bill_t&)> cmp) {
    if (first < last - 1) {
        auto pivot = partition(first, last, cmp);
        quicksort(first, pivot, cmp);
        quicksort(pivot + 1, last, cmp);
    }
}

std::pair<std::vector<bill_t>, double>
    record_list::generate_purchase_report_by_date_range(
        const std::tuple<int, int, int>& start_date, const std::tuple<int, int, int>& end_date
    ) {

    quicksort(records.begin(), records.end(), [](const bill_t& a, const bill_t& b) {
        return a.get_date() < b.get_date();
    });

    // for (const auto& record : records) {
    //     std::cout << std::get<0>(record.get_date()) << std::get<1>(record.get_date()) << std::get<2>(record.get_date()) << std::endl;
    // }

    auto lower = lower_bound(records.begin(), records.end(), start_date);
    auto upper = upper_bound(records.begin(), records.end(), end_date);

    std::vector<bill_t> sorted_records(lower, upper);
    quicksort(sorted_records.begin(), sorted_records.end(), [](const bill_t& a, const bill_t& b) {
        return a.get_total_price() < b.get_total_price();
    });

    double total_price = 0.0;
    for (const auto& record : sorted_records) {
        total_price += record.get_total_price();
        // std::cout << record.get_total_price() << std::endl;
    }

    return make_pair(sorted_records, total_price);
}

void record_list::add_sale(const std::string& brand, int count) {
    get_car_sold_by_brandname(brand) += count;
    bubble_sort_brand_sale_pair_by_sale();
}

std::pair<std::string, int> record_list::search_best_selling_car_by_brand(std::vector<std::string> brands) {
    bubble_sort_brand_sale_pair_by_sale();
    int maxSold = -1;
    std::string maxSoldBrand = "";
    for(auto brand : brands) {
        // std::cout << "Checking brand: " << brand << "\n";
        int sold = get_car_sold_by_brandname(brand);
        if(sold > maxSold) {
            maxSold = sold;
            maxSoldBrand = brand;
        }
    }
    return std::make_pair(maxSoldBrand, maxSold);
};

void record_list::bubble_sort_brand_sale_pair_by_sale() {
    int n = sale_count.size();
    bool swapped;
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (sale_count[j].second > sale_count[j + 1].second) {
                std::swap(sale_count[j], sale_count[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

int& record_list::get_car_sold_by_brandname(std::string brandname) {
    int left = 0;
    int right = static_cast<int>(sale_count.size()) - 1;
    int ans = -1;
    int mid;
    while (left <= right) {
        // std::cout << left << " " << right << "\n";
        mid = left + (right - left) / 2;
        if (sale_count[mid].first == brandname) {
            ans = mid;
            break;
        } else if (sale_count[mid].first < brandname) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    if (ans == -1) {
        sale_count.insert(sale_count.begin(), std::make_pair(brandname, 0));
        ans = 0;
    }
    return sale_count[ans].second;
}
#endif // RECORDING_HPP
