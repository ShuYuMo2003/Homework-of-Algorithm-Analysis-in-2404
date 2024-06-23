#ifndef DISPLAY_HPP
#define DISPLAY_HPP
#include <iostream>
#include <string>
#include <vector>


class customer_t {
    std::string customer_name;
    std::string customer_id;
    std::string customer_pwd;
public:
    customer_t() = default;
    customer_t( const std::string& customer_name,
                const std::string& customer_id,
                const std::string& customer_pwd):customer_name(customer_name),
                                                 customer_id(customer_id),
                                                 customer_pwd(customer_pwd)
    {}

    const bool isAuthenticated(const std::string& customer_id, const std::string& customer_pwd){return (this->customer_id == customer_id && this->customer_pwd == customer_pwd);};
    const std::string get_customer_id(){return customer_id;}
    const std::string get_customer_name(){return customer_name;}
};

class customer_list {
    std::vector<customer_t> customer_List;
    bool isExit(std::string customer_id);
public:
    customer_list() = default;
    void add_customer(const std::string& customer_name,
                const std::string& customer_id,
                const std::string& customer_pwd);
    bool isAuthenticated(const std::string& customer_id, const std::string& customer_pwd);

};

//Implement
//鉴定该ID是否以及存在
bool customer_list::isExit(std::string customer_id){
    for(auto customer : customer_List){
        if(customer.get_customer_id() == customer_id){
            return true;
        }
    }
    return false;
};
void customer_list::add_customer(const std::string& customer_name,
                const std::string& customer_id,
                const std::string& customer_pwd)
{
    if(isExit(customer_id)){
        std::cout << "Registered ID" << std::endl;
    }else{
        customer_List.push_back(customer_t(customer_name, customer_id, customer_pwd));
        std::cout << "Add Successfully" << std::endl;
    }
}

//鉴权
bool customer_list::isAuthenticated(const std::string& customer_id, const std::string& customer_pwd){
    if(!isExit(customer_id)){
        std::cout << "Unregistered ID" << std::endl;
        return false;
    } else {
        for(auto customer : customer_List){
            if (customer.isAuthenticated(customer_id, customer_pwd)) {
                return true;
            };
        }
    }
    std::cout << "Wrong Password" << std::endl;
    return false;
};

#endif // DISPLAY_HPP