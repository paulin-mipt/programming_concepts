#include <string>
#include <set>
#include <map>
typedef std::set<std::string> PARENTS;
typedef std::map<std::string, PARENTS> CLASSES;

#define IMPORT_RTTI_STUFF CLASSES global_classes;
namespace {
    int hash_name(std::string s) {
        int answer = 0;
        int base = 1000000007;
        int mult = 1;
        for (auto i = 0; i < s.length(); i++) {
            answer += (mult * s[i]) % base;
            mult = (mult * 7) % base;
        }
        return answer;
    }

    template <typename target_ptr, typename classname_ptr>
    target_ptr cast_forced(classname_ptr obj_ptr) {
        std::string name = obj_ptr->real_name;
        target_ptr answer = reinterpret_cast<target_ptr>(obj_ptr);
        answer->real_name = name;
        return answer;
    }

    bool can_reach(std::string current_classname, std::string goal, CLASSES* global_classes) {
        if (current_classname == goal)
            return true;
        for (auto it = (*global_classes)[current_classname].begin(); it != (*global_classes)[current_classname].end(); it++) {
            if (can_reach(*it, goal, global_classes)) {
                return true;
            }
        }
        return false;
    }

    std::set<std::string> split(std::string s, char delimiter) {
        std::set<std::string> internal;
        std::stringstream ss(s);
        std::string tok;
        size_t found;

        while (getline(ss, tok, delimiter)) {
            while ((found = tok.find(" ")) != std::string::npos) {
                tok = tok.substr(found + 1, tok.length());
            }
            internal.insert(tok);
        }
        return internal;
    }

    struct typeinfo {
        typeinfo(std::string _name, bool _is_comparable, int _hash_code) {
            name = _name;
            hash_code = _hash_code;
        }
        std::string name;
        bool is_comparable;
        int hash_code;
    };
}

#define TYPEID(obj_ptr) ((obj_ptr)->class_info)
#define DYNAMIC_CAST(obj_ptr, current_classname, classname) ((can_reach((obj_ptr)->real_name, classname::class_info.name, &global_classes)) ? (cast_forced<classname*, current_classname*>(obj_ptr)) : nullptr)

#define DEFINE_CLASS(classname) class classname {\
                                public:\
                                    classname();\
                                    static void init() {\
                                        global_classes.insert(std::pair<std::string, PARENTS>(#classname, classname::parents));\
                                    }\
                                    static PARENTS parents;\
                                    static typeinfo class_info;\
                                    std::string real_name;\

#define DEFINE_DERIVED_CLASS(classname, ...) class classname: __VA_ARGS__ {\
                                public:\
                                    classname();\
                                    static void init() {\
                                        classname::parents = split(#__VA_ARGS__, ',');\
                                        global_classes.insert(std::pair<std::string, PARENTS>(#classname, classname::parents));\
                                    }\
                                    static PARENTS parents;\
                                    static typeinfo class_info;\
                                    std::string real_name;\

#define END_DEFINE(classname) }; PARENTS classname::parents;\
                                    typeinfo classname::class_info = typeinfo(#classname, true, hash_name(#classname));\
                                    classname::classname() {\
                                        real_name = #classname;\
                                    }
