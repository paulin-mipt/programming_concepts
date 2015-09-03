#include <iostream>
#include <map>
#include <string>
#include <set>
#include <sstream>
#include "rtti_macro.h"
IMPORT_RTTI_STUFF

DEFINE_CLASS(A)
    int a;
    virtual void m() {}
    void methodSpecificToA() {
        std::cout << "I am A!" << std::endl;
	}
END_DEFINE(A)

DEFINE_CLASS(C)
    int c;
    virtual void m() {}
    void methodSpecificToC() {
        std::cout << "I am C!" << std::endl;
	}
END_DEFINE(C)

// A and C are oublic parents of B
DEFINE_DERIVED_CLASS(B, public A, public C)
    virtual void m() {}
	void methodSpecificToB() {
        std::cout << "I am B!" << std::endl;
	}
END_DEFINE(B)

// D is a child of B
DEFINE_DERIVED_CLASS(D, public B)
    //virtual void m() {}
	void methodSpecificToD() {
        std::cout << "I am D!" << std::endl;
	}
END_DEFINE(D)

int main() {
    A::init();
    B::init();
    C::init();
    D::init();

    A a = A();
    B b = B();
    C c = C();
    D d = D();

    std::cout << "Check inheritance:" << std::endl;
    d.a = 1;
    d.c = 2;
    d.methodSpecificToA();
    d.methodSpecificToB();
    d.methodSpecificToC();
    d.methodSpecificToD();

    std::cout << "Check TYPEID:" << std::endl;
    typeinfo a_info = TYPEID(&a);
    typeinfo b_info = TYPEID(&b);
    typeinfo c_info = TYPEID(&c);
    typeinfo d_info = TYPEID(&d);
    std::cout << TYPEID(&a).name << TYPEID(&b).name << TYPEID(&c).name << TYPEID(&d).name << std::endl;

    std::cout << "Check valid DYNAMIC_CAST:" << std::endl;
    auto b_a_ptr = DYNAMIC_CAST(&b, B, A);
    std::cout << b_a_ptr << std::endl;
    std::cout << TYPEID(b_a_ptr).name << std::endl;

    auto d_a_ptr = DYNAMIC_CAST(&d, D, A);
    std::cout << d_a_ptr << std::endl;
    std::cout << TYPEID(d_a_ptr).name << std::endl;

    auto d_b_ptr = DYNAMIC_CAST(d_a_ptr, A, B);
    std::cout << d_b_ptr << std::endl;
    std::cout << TYPEID(d_b_ptr).name << std::endl;
    A* a_ptr = new D();
    std::cout << a_ptr << std::endl;
    std::cout << TYPEID(a_ptr).name << std::endl;


    std::cout << "These must return null:" << std::endl;
    C* c_null_ptr = DYNAMIC_CAST(&a, A, C);
    std::cout << c_null_ptr << std::endl;
    D* d_null_ptr = DYNAMIC_CAST(&b, B, D);
    std::cout << d_null_ptr << std::endl;

    return 0;
}
