#ifndef USER_HPP
#define USER_HPP
#include <iostream>
#include <string>
#include <vector>


class user_t {
    std::string customer_id;
    std::string customer_pwd;
public:
    user_t() = default;
    user_t( const std::string& customer_id,
                const std::string& customer_pwd):customer_id(customer_id),
                                                 customer_pwd(customer_pwd)
    {}

    const bool isAuthenticated(const std::string& customer_id, const std::string& customer_pwd){return (this->customer_id == customer_id && this->customer_pwd == customer_pwd);};
    const std::string get_customer_id(){return customer_id;}
};

class user_list {
    std::vector<user_t> customer_List;
    bool isExit(std::string customer_id);
public:
    user_list() = default;
    user_list(const std::vector<user_t>& customer_List):customer_List(customer_List){}
    void add_customer(const std::string& customer_id,
                const std::string& customer_pwd);
    bool isAuthenticated(const std::string& customer_id, const std::string& customer_pwd);

};

bool user_list::isExit(std::string customer_id){
    for(auto customer : customer_List){
        if(customer.get_customer_id() == customer_id){
            return true;
        }
    }
    return false;
};
void user_list::add_customer(const std::string& customer_id,
                                 const std::string& customer_pwd)
{
    if(isExit(customer_id)){
        std::cout << "Registered ID" << std::endl;
    }else{
        customer_List.push_back(user_t(customer_id, customer_pwd));
        std::cout << "Add Successfully" << std::endl;
    }
}


bool user_list::isAuthenticated(const std::string& customer_id, const std::string& customer_pwd){
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

#endif // USER_HPP