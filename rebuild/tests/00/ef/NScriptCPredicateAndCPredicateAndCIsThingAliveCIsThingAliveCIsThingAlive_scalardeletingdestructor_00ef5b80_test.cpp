// behaviour test for 0x00EF5B80 the CPredicate_And deleting destructor (model: Right then Left destroyed, then freed)
#include <cstdio>
static int g_order[4]; static int g_n = 0; static int g_freed = 0;
struct Base { virtual ~Base() { } };
struct Pred : Base { int id; Pred(int i) : id(i) { } ~Pred() { g_order[g_n++] = id; } };
struct And : Base {
    Pred Left, Right;
    And() : Left(1), Right(2) { }
    static void operator delete(void* p) { ++g_freed; ::operator delete(p); }
};
int main()
{
    Base* p = new And;
    delete p;
    int ok = g_n == 2 && g_order[0] == 2 && g_order[1] == 1 && g_freed == 1;
    if (!ok) { std::printf("00ef5b80_TEST FAIL\n"); return 1; }
    std::printf("00ef5b80_TEST PASS\n");
    return 0;
}