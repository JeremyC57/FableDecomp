// Behaviour test for CTCTrespassable::CTCTrespassable(CThing*) @ 0x007BAD20: both flags cleared.
#include <cstdio>
#include <new>
class CThing { public: int id; };
static CThing* g_baseParent = 0;
struct CTCBase_Calls {
    virtual void v00() { }
    unsigned char _body[0x8];
    CTCBase_Calls(CThing* parent) { g_baseParent = parent; }
};
class CTCTrespassable : public CTCBase_Calls { public:
    bool Trespassable;
    bool IsBeingTrespassedIn;
    virtual void v00() { }
    CTCTrespassable(CThing* parent);
};
CTCTrespassable::CTCTrespassable(CThing* parent) : CTCBase_Calls(parent), Trespassable(false), IsBeingTrespassedIn(false) { }
int main()
{
    union { double d; void* p; unsigned char raw[sizeof(CTCTrespassable)]; } u;
    for (unsigned i = 0; i < sizeof(u.raw); ++i) u.raw[i] = 0xAA;
    CThing thing;
    CTCTrespassable* t = new (u.raw) CTCTrespassable(&thing);
    if (g_baseParent != &thing || t->Trespassable || t->IsBeingTrespassedIn) { std::printf("007bad20_TEST FAIL\n"); return 1; }
    std::printf("007bad20_TEST PASS\n");
    return 0;
}