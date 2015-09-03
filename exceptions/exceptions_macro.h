#define TRY {\
    try_block _sl;\
    int errcode = setjmp(_sl.env);\
    if (errcode == 0) {

#define EXCEPT } else {

#define ENDTRY }\
    }

#define RAISE(exc) raise(exc);

namespace {
    struct try_block;
    try_block* head_block = NULL;

    class Unwindable {
    public:
        Unwindable();
        virtual ~Unwindable();
        Unwindable* previous;
    };

    struct try_block {
        try_block();
        ~try_block();
        jmp_buf env;
        try_block* previous;
        Unwindable* stack_objects;
    };

    try_block::try_block() {
        stack_objects = NULL;
        previous = head_block;
        head_block = this;
    }

    try_block::~try_block() {
        head_block = previous;
    }

    extern int raise(int code);

    Unwindable::Unwindable() {
        if (head_block) { //if there was any TRY block before
            previous = head_block->stack_objects;
            head_block->stack_objects = this;
        }
    }

    Unwindable::~Unwindable() {
        if (head_block && previous) {
            head_block->stack_objects = previous;
            previous = NULL;
        }
    }

    int pop() {
        head_block->~try_block();
        return 0;
    }

    int raise(int code) {
        try_block* head = head_block;
        Unwindable* head_object = head_block->stack_objects;
        //destroy
        TRY {
            while(head_object) {
                Unwindable* tmp = head_object;
                head_object = head_object->previous;
                tmp->~Unwindable();
            }
        } EXCEPT {
            exit(errcode);
        }
        ENDTRY
        pop();
        longjmp(head->env, code);
        return 0;
    }
}

