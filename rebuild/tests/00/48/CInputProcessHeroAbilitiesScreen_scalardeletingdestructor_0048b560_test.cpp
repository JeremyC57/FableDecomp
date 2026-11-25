// Behaviour model of CInputProcessHeroAbilitiesScreen::`scalar deleting destructor' (row 0x0048B560, ??_G): delete through a base pointer runs the destructor
// chain once and frees the object; an explicit virtual destructor call (flags 0) runs it without freeing. Self-contained model of the
// same class shape (the row object is the owner TU's; NF419 r1: leaf bytes and relocations graded against retail).
#include <cstdio>
#include <cstdlib>
static int g_dtors = 0, g_frees = 0; static void* g_freed = 0;
void* operator new(unsigned int n) { return std::malloc(n ? n : 1); }
void operator delete(void* p) { ++g_frees; g_freed = p; std::free(p); }
class Base { public: int baseState; Base() : baseState(1) { } virtual ~Base() { ++g_dtors; } };
class Derived : public Base { public: int ownState[3]; };
int main()
{
    Derived* p = new Derived;
    Base* q = p;
    delete q;
    if (g_dtors != 1 || g_frees != 1 || g_freed != (void*)p) { std::printf("0048b560 FAIL delete\n"); return 1; }
    Derived* r = new Derived;
    r->~Derived();
    if (g_dtors != 2 || g_frees != 1) { std::printf("0048b560 FAIL no-free path\n"); return 1; }
    std::free(r);
    std::printf("VDD_PARITY_OK\n");
    return 0;
}
