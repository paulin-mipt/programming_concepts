#include<map>
#include<string>

#define VTABLE(classname) std::map<std::string, void(*)(classname*)>

/* class declaring */
#define VIRTUAL_CLASS(classname) struct classname {\
                                    classname();\
                                    static VTABLE(classname) vtable;\
                                    VTABLE(classname) *vtable_ptr = &vtable;


#define VIRTUAL_CLASS_DERIVED(classname, parentname) struct classname {\
                                    classname();\
                                    static VTABLE(classname) vtable;\
                                    VTABLE(classname) *vtable_ptr = &vtable;

#define END(classname) };\
                    VTABLE(classname) classname::vtable;\

#define END_DERIVED(classname, parentname) };\
                    VTABLE(classname) classname::vtable;\

/* method declaration */
#define IMPLEMENT_METHOD(classname, methodname) void methodname(classname* that) {\
                            std::cout << #classname "::" #methodname " was called!\n";

#define END_IMPLEMENT_METHOD }

#define START_DECLARE_METHODS(classname) classname::classname() {

#define IMPORT_METHODS_DERIVED(classname, parentname) VTABLE(parentname)::iterator it;\
    for (it = parentname::vtable.begin(); it != parentname::vtable.end(); ++it) {\
        classname::vtable.insert(std::pair<std::string, void(*) (classname*)>(it->first, (void(*) (classname*))it->second));\
    }\

#define DECLARE_METHOD(classname, methodname) \
            classname::vtable.insert(std::pair<std::string, void(*)(classname*)>(#methodname, &methodname));

#define END_DECLARE_METHODS }

#define VIRTUAL_CALL(class_ptr, methodname) (*((*((class_ptr)->vtable_ptr))[#methodname]))(class_ptr);
