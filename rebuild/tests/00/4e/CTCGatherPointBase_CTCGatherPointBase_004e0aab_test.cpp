// Behaviour test for CTCGatherPointBase::CTCGatherPointBase(CThing*) @ 0x004E0AAB: base gets the parent, the Users
// vector is empty and MaxOccupation is 1000.
#include <cstdio>
#include <new>
struct CThing { int id; };
static CThing* g_baseParent = 0;
struct CTCBase_Calls {
    virtual void v00() { }
    unsigned char _body[0x8];
    CTCBase_Calls(CThing* parent) { g_baseParent = parent; }
};
template<class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};
struct CIntelligentPointerThing { void* __vftable; void* PData; };
struct CTCGatherPointBase : CTCBase_Calls {
    CRetailVector<CIntelligentPointerThing> Users;
    long                                    MaxOccupation;
    virtual void v00() { }
    CTCGatherPointBase(CThing* parent);
};
CTCGatherPointBase::CTCGatherPointBase(CThing* parent) : CTCBase_Calls(parent), Users(), MaxOccupation(1000) { }
int main()
{
    union { double d; void* p; unsigned char raw[sizeof(CTCGatherPointBase)]; } u;
    for (unsigned i = 0; i < sizeof(u.raw); ++i) u.raw[i] = 0xAA;
    CThing thing;
    CTCGatherPointBase* g = new (u.raw) CTCGatherPointBase(&thing);
    if (g_baseParent != &thing || g->Users.m_pFirst != 0 || g->Users.m_pLast != 0 || g->Users.m_pEnd != 0
        || g->MaxOccupation != 1000) { std::printf("004e0aab_TEST FAIL\n"); return 1; }
    std::printf("004e0aab_TEST PASS\n");
    return 0;
}