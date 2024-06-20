#ifndef RECORDING_HPP
#define RECORDING_HPP

#include <string>
#include <vector>

#include "car.hpp"

class bill_t {
    std::string record_id;
    std::string customer_name;
    std::string customer_id;
    double      total_price;
    std::string any_applicable_discount;
    // sold car object | and | how many
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
};

class record_list{
    std::vector<bill_t> records;
public:
    bill_t add_record(const std::string& record_id,
                    const std::string& customer_name,
                    const std::string& customer_id,
                    const double& total_price,
                    const std::string& any_applicable_discount,
                    const std::vector<std::pair<car_t, int>>& paid_cars);
    void apply_sort_by(std::function<bool(const bill_t&, const bill_t&)> cmp);
    bill_t find_record_by_customer_id(const std::string& customer_id);
    bill_t find_record_by_date(const std::tuple<int, int, int> date);
    std::vector<bill_t> find_records_by_date_range(const std::tuple<int, int, int> start_date, const std::tuple<int, int, int> end_date);
};

bill_t record_list::add_record(const std::string& record_id,
                             const std::string& customer_name,
                             const std::string& customer_id,
                             const double& total_price,
                             const std::string& any_applicable_discount,
                             const std::vector<std::pair<car_t, int>>& paid_cars){
    bill_t bill = bill_t(record_id, customer_name, customer_id, total_price, any_applicable_discount, paid_cars);
    records.push_back(bill);
}

// apply sort on records
void record_list::apply_sort_by(std::function<bool(const bill_t&, const bill_t&)> cmp){ }

// try to use `record_list::apply_sort_on_records` before searching.
bill_t record_list::find_record_by_customer_id(const std::string& customer_id){}
bill_t record_list::find_record_by_date(const std::tuple<int, int, int> date){}
std::vector<bill_t> record_list::find_records_by_date_range(const std::tuple<int, int, int> start_date, const std::tuple<int, int, int> end_date){}

#endif // RECORDING_HPP