// Behaviour test for CTCBed::CTCBed(CThing*) @ 0x00746820: Empty/Animating true, GettingIn/GettingOut false,
// BedSleepAnim -1, null def.
#include <cstdio>
#include <new>
struct CThing { int id; };
struct CBedDef;
static CThing* g_baseParent = 0;
struct CTCBase_Calls {
    virtual void v00() { }
    unsigned char _body[0x8];
    CTCBase_Calls(CThing* parent) { g_baseParent = parent; }
};
template<class T> struct CDefPointer { T* m_pDef; CDefPointer() : m_pDef(0) { } };
struct CTCBed : CTCBase_Calls {
    bool Empty; bool GettingIn; bool GettingOut; bool Animating;
    long BedSleepAnim;
    CDefPointer<const CBedDef> PDef;
    virtual void v00() { }
    CTCBed(CThing* parent);
};
CTCBed::CTCBed(CThing* parent)
    : CTCBase_Calls(parent), Empty(true), GettingIn(false), GettingOut(false), Animating(true), BedSleepAnim(-1), PDef() { }
int main()
{
    union { double d; void* p; unsigned char raw[sizeof(CTCBed)]; } u;
    for (unsigned i = 0; i < sizeof(u.raw); ++i) u.raw[i] = 0xAA;
    CThing thing;
    CTCBed* b = new (u.raw) CTCBed(&thing);
    if (g_baseParent != &thing || !b->Empty || b->GettingIn || b->GettingOut || !b->Animating || b->BedSleepAnim != -1
        || b->PDef.m_pDef != 0) { std::printf("00746820_TEST FAIL\n"); return 1; }
    std::printf("00746820_TEST PASS\n");
    return 0;
}