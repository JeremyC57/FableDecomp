#include <cstdio>

struct L3 { char pad18[0x18]; long value; };
struct L2 { char pad2c[0x2c]; L3* p2c; };
struct L1 { char pad08[0x08]; L2* p08; };
struct Root { char pad88[0x88]; L1* p88; };

// Test model of the retail singleton slot 0x013B89FC: the private static CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass
// (same mangled name as the row binds); TestPClass() is a test-only accessor.
class CQuestManager;
template<class T> class CCountedPointer { public: T* Data; void* Info; };
template<class T> class CTBaseSingletonNoDefaultAlloc { static CCountedPointer<T> PClass; public: static CCountedPointer<T>& TestPClass() { return PClass; } };
template<class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;

long model()
{
    L1* a = (reinterpret_cast<Root*&>(CTBaseSingletonNoDefaultAlloc<CQuestManager>::TestPClass().Data))->p88;
    if (a) {
        L2* b = a->p08;
        if (b) {
            return b->p2c->value;
        }
    }
    return 0;
}

int main()
{
    // Case 1: full chain -> value
    L3 l3; l3.value = 0x1234;
    L2 l2; l2.p2c = &l3;
    L1 l1; l1.p08 = &l2;
    Root r; r.p88 = &l1;
    (reinterpret_cast<Root*&>(CTBaseSingletonNoDefaultAlloc<CQuestManager>::TestPClass().Data)) = &r;
    if (model() != 0x1234) { printf("FAIL a\n"); return 1; }

    // Case 2: p88 null -> 0
    r.p88 = 0;
    if (model() != 0) { printf("FAIL b\n"); return 1; }

    // Case 3: p88 set but p08 null -> 0
    r.p88 = &l1; l1.p08 = 0;
    if (model() != 0) { printf("FAIL c\n"); return 1; }

    printf("BEHAVIOR_OK\n");
    return 0;
}