#include <iostream>
#include <csetjmp>
#include <string>
#include <cstdio>
#include "exceptions_macro.h"

//all classes should be inherited from Unwindable
class A: public Unwindable {
    int a;
public:
    A() {
        Unwindable();
        a = 42;
        std::cout << "A constructed without an argument" << std::endl;
    }
    A(int val) {
        Unwindable();
        a = val;
        std::cout << "A constructed: " << a << std::endl;
    }
    virtual ~A() {
        //RAISE(123); // testing exit on dectruction exception
        std::cout << "A destructed: " << a << std::endl;
    }
};

int func_raising(A a, A b) {
    TRY {
        RAISE(500);
    }
    EXCEPT {
        std::cout << "Caught exception in function with code " << errcode << std::endl;
    }
    // new exception for the main TRY
    RAISE(404);
    ENDTRY
    return 1;
}

int func(A a) {
    func_raising(a, A(3));
    return 1;
}

int main() {
    A a(1);
    TRY {
        func(A(2));
        std::cout << "This should not be printed!" << std::endl;
    } EXCEPT {
        std::cout << "Caught exception with code " << errcode << std::endl;
    }
    ENDTRY

    return 0;
}
