#include <iostream>

class Parent {
public:
    Parent() { std::cout << "Parent Constructor\n"; }
    ~Parent() { std::cout << "Parent Destructor\n"; } // Note: Not virtual
    virtual void speak() { std::cout << "Parent speaks\n"; }
};

class Child : public Parent {
public:
    Child() { std::cout << "Child Constructor\n"; }
    ~Child() { std::cout << "Child Destructor\n"; }
    void speak() override { std::cout << "Child speaks\n"; }
};

int main() {
    Parent* p = new Child();
    p->speak();
    
    return 0;
}