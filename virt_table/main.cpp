#include <iostream>
#include "virt_table_macro.h"
#include <map>
#include <string>

// base class
VIRTUAL_CLASS(Base)
    int a = 1;
END(Base)

IMPLEMENT_METHOD(Base, Both)
    std::cout << "a is " << that->a << std::endl;
END_IMPLEMENT_METHOD

IMPLEMENT_METHOD(Base, OnlyBase)
    //empty method
END_IMPLEMENT_METHOD

START_DECLARE_METHODS(Base)

DECLARE_METHOD(Base, Both)
DECLARE_METHOD(Base, OnlyBase)

END_DECLARE_METHODS

// derived class
VIRTUAL_CLASS(Derived)
    int b;
END(Derived)

IMPLEMENT_METHOD(Derived, Both)
    std::cout << "b is " << that->b << std::endl;
END_IMPLEMENT_METHOD

IMPLEMENT_METHOD(Derived, OnlyDerived)
    that->b = 42;
END_IMPLEMENT_METHOD

START_DECLARE_METHODS(Derived)

DECLARE_METHOD(Derived, Both)
DECLARE_METHOD(Derived, OnlyDerived)
IMPORT_METHODS_DERIVED(Derived, Base)

END_DECLARE_METHODS


int main() {
    Base base = Base();
    base.a = -13;
    Derived derived = Derived();

    Base* reallyDerived = reinterpret_cast<Base*>(&derived);

    VIRTUAL_CALL(&base, Both)
    VIRTUAL_CALL(reallyDerived, Both)
    VIRTUAL_CALL(reallyDerived, OnlyBase)
    VIRTUAL_CALL(reallyDerived, OnlyDerived)
    return 0;
}
