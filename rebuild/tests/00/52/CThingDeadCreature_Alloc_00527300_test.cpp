// Behaviour test for 00527300 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
#include <cstdlib>
static int g_Ctors = 0; static bool g_Flag = true;
class CClass { public: virtual ~CClass() { } };
class CThing : public CClass { public:
    static void* operator new(std::size_t size, bool flag) { g_Flag = flag; return std::malloc(size); }
    static void operator delete(void* p, bool) { std::free(p); }
    static void operator delete(void* p) { std::free(p); } };
class CThingDeadCreature : public CThing { public: int Body[8]; CThingDeadCreature() { ++g_Ctors; Body[0] = 1; }
    static CClass* Alloc() { return new (false) CThingDeadCreature; } };
int main() {
    CClass* c = CThingDeadCreature::Alloc();
    if (!c || g_Ctors != 1 || g_Flag || ((CThingDeadCreature*)c)->Body[0] != 1) { std::printf("00527300_TEST FAIL\n"); return 1; }
    delete c;
    { std::printf("00527300_TEST PASS\n"); return 0; }
}