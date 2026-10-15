// Behaviour test for CTCDHeroSuit::CTCDHeroSuit(CThing*) @ 0x004E079C: the base gets the parent, PDef and the
// SuitParts vector come out null on a garbage-filled buffer.
#include <cstdio>
#include <new>
struct CThing { int id; };
struct CHeroSuitDef;
static CThing* g_baseParent = 0;
struct CTCDriverBase_Calls {
    virtual void v00() { }
    unsigned char _body[0x8];
    CTCDriverBase_Calls(CThing* parent) { g_baseParent = parent; }
};
template<class T> struct CDefPointer { T* m_pDef; CDefPointer() : m_pDef(0) { } };
template<class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};
struct SSuitPart { long Part; bool Worn; };
struct CTCDHeroSuit : CTCDriverBase_Calls {
    CDefPointer<const CHeroSuitDef> PDef;
    CRetailVector<SSuitPart>        SuitParts;
    virtual void v00() { }
    CTCDHeroSuit(CThing* parent);
};
CTCDHeroSuit::CTCDHeroSuit(CThing* parent) : CTCDriverBase_Calls(parent), PDef(), SuitParts() { }
int main()
{
    union { double d; void* p; unsigned char raw[sizeof(CTCDHeroSuit)]; } u;
    for (unsigned i = 0; i < sizeof(u.raw); ++i) u.raw[i] = 0xAA;
    CThing thing;
    CTCDHeroSuit* s = new (u.raw) CTCDHeroSuit(&thing);
    if (g_baseParent != &thing || s->PDef.m_pDef != 0 || s->SuitParts.m_pFirst != 0 || s->SuitParts.m_pLast != 0
        || s->SuitParts.m_pEnd != 0) { std::printf("004e079c_TEST FAIL\n"); return 1; }
    std::printf("004e079c_TEST PASS\n");
    return 0;
}