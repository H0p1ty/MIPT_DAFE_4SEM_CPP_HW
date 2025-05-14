#include "TypeList.hpp"
#include <iostream>
#include <string>


int main() {
   constexpr TypeList<int, double, std::string, float, int> t1;

   std::cout << t1.size << std::endl;

   std::cout << t1.contains<std::string> << std::endl;

   std::cout << "index " << t1.get_index<int, 2> << std::endl; // == t1.size if no such type exists...

   using t1_type = TypeList<int, double, std::string, float, int>;
   std::cout << std::is_same_v<t1_type::type_by_index<3>, int> << std::endl;

   auto xj9 = t1.push_back<std::exception>;
   std::cout << t1.contains<std::exception> << std::endl;
   std::cout << xj9.contains<std::exception> << " " << xj9.get_index<std::exception> << std::endl;

   auto bebrhype = t1.push_front<std::exception>;
   std::cout << t1.contains<std::exception> << std::endl;
   std::cout << bebrhype.contains<std::exception> << " " << bebrhype.get_index<std::exception> << std::endl;
}