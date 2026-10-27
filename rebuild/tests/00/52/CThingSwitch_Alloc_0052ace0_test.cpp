// Behaviour test for 0052ace0 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
#include <cstdlib>
static int g_Ctors = 0; static bool g_Flag = true;
class CClass { public: virtual ~CClass() { } };
class CThing : public CClass { public:
    static void* operator new(std::size_t size, bool flag) { g_Flag = flag; return std::malloc(size); }
    static void operator delete(void* p, bool) { std::free(p); }
    static void operator delete(void* p) { std::free(p); } };
class CThingSwitch : public CThing { public: int State; CThingSwitch() : State(3) { ++g_Ctors; }
    static CClass* Alloc() { return new (false) CThingSwitch; } };
int main() {
    CClass* c = CThingSwitch::Alloc();
    if (!c || g_Ctors != 1 || g_Flag || ((CThingSwitch*)c)->State != 3) { std::printf("0052ace0_TEST FAIL\n"); return 1; }
    delete c;
    { std::printf("0052ace0_TEST PASS\n"); return 0; }
}