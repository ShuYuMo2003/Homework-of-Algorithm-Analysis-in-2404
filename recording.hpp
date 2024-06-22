#ifndef RECORDING_HPP
#define RECORDING_HPP

#include <string>
#include <vector>
#include <tuple>
#include <stdexcept>
#include <algorithm> // for std::lower_bound and std::upper_bound

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
           const std::vector<std::pair<car_t, int>>& paid_cars) : record_id(record_id),
                                                                  customer_name(customer_name),
                                                                  customer_id(customer_id),
                                                                  total_price(total_price),
                                                                  any_applicable_discount(any_applicable_discount),
                                                                  paid_cars(paid_cars)
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

public:
    bill_t add_record(const std::string& record_id,
                      const std::string& customer_name,
                      const std::string& customer_id,
                      const double& total_price,
                      const std::string& any_applicable_discount,
                      const std::vector<std::pair<car_t, int>>& paid_cars);
    bill_t find_record_by_customer_id(const std::string& customer_id);
    bill_t find_record_by_date(const std::tuple<int, int, int> date);
    std::vector<bill_t> find_records_by_date_range(const std::tuple<int, int, int> start_date, const std::tuple<int, int, int> end_date);
    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::tuple<int, int, int>, double>> generate_purchase_report(const std::tuple<int, int, int>& start_date, const std::tuple<int, int, int>& end_date);
};

bill_t record_list::add_record(const std::string& record_id,
                               const std::string& customer_name,
                               const std::string& customer_id,
                               const double& total_price,
                               const std::string& any_applicable_discount,
                               const std::vector<std::pair<car_t, int>>& paid_cars) {
    bill_t bill = bill_t(record_id, customer_name, customer_id, total_price, any_applicable_discount, paid_cars);
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
//二分查找
std::vector<bill_t> record_list::find_records_by_date_range(const std::tuple<int, int, int> start_date, const std::tuple<int, int, int> end_date) {
    std::vector<bill_t> result;

    auto lower = std::lower_bound(records.begin(), records.end(), start_date,
                                  [](const bill_t& bill, const std::tuple<int, int, int>& start_date) {
                                      return bill.get_date() < start_date;
                                  });

    auto upper = std::upper_bound(records.begin(), records.end(), end_date,
                                  [](const std::tuple<int, int, int>& end_date, const bill_t& bill) {
                                      return end_date < bill.get_date();
                                  });

    for (auto it = lower; it != upper; ++it) {
        result.push_back(*it);
    }

    return result;
}
//二分查找以及快速排序
std::vector<std::tuple<std::string, std::string, std::string, std::string, std::tuple<int, int, int>, double>> record_list::generate_purchase_report(const std::tuple<int, int, int>& start_date, const std::tuple<int, int, int>& end_date) {
    std::vector<std::tuple<std::string, std::string, std::string, std::string, std::tuple<int, int, int>, double>> report;

    auto lower = std::lower_bound(records.begin(), records.end(), start_date,
                                  [](const bill_t& bill, const std::tuple<int, int, int>& start_date) {
                                      return bill.get_date() < start_date;
                                  });

    auto upper = std::upper_bound(records.begin(), records.end(), end_date,
                                  [](const std::tuple<int, int, int>& end_date, const bill_t& bill) {
                                      return end_date < bill.get_date();
                                  });

    std::sort(lower, upper, [](const bill_t& a, const bill_t& b) {
        return a.get_car_price() < b.get_car_price();
    });

    double total_price = 0.0;
    for (auto it = lower; it != upper; ++it) {
        total_price += it->get_car_price();
    }

    for (auto it = lower; it != upper; ++it) {
        report.push_back(std::make_tuple(it->get_car_id(), it->get_car_brand(), it->get_car_color(),
                                         it->get_customer_id(), it->get_date(), it->get_car_price()));
    }

    report.push_back(std::make_tuple("", "", "", "Total Price of Sold Cars", std::tuple<int, int, int>(), total_price));

    return report;
}

#endif // RECORDING_HPP
