// Behaviour test for 004df14b: body copied from src/004df14b.cpp, callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
template <class P> struct CAllocProxy { P p; CAllocProxy(P v) : p(v) { } operator P() const { return p; } CAllocProxy& operator=(P v) { p = v; return *this; } };
static int g_baseCtor = 0, g_handleCtor = 0; static const void* g_baseParent = 0;
struct CThing; struct CEnginePersistentPrimitiveBase;
struct CTCDriverBase_Calls { virtual void v00() {} unsigned char _body[0x8]; CTCDriverBase_Calls(CThing* parent) { ++g_baseCtor; g_baseParent = parent; } };
struct C3DVector { float X, Y, Z; C3DVector() { } };
template<class T> struct CRetailVector { T* m_pFirst; T* m_pLast; CAllocProxy<T*> m_pEnd; CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
struct CIntelligentPointerThing { void* __vftable; void* PData; };
struct CBaseClass { virtual void v00() {} CBaseClass() { ++g_handleCtor; } };
struct CIVCountedPointer { CEnginePersistentPrimitiveBase* Object; __forceinline CIVCountedPointer() : Object(0) { } };
struct CEnginePrimitiveHandle : CBaseClass { CIVCountedPointer Data; virtual void v00() {} __forceinline CEnginePrimitiveHandle() { } };
struct CTCDPhysicalObstruction : CTCDriverBase_Calls {
    C3DVector StartPos; C3DVector EndPos; long LifeTimer; float Radius; CRetailVector<CIntelligentPointerThing> ThingsToIgnoreObstruction;
    CEnginePrimitiveHandle line1; CEnginePrimitiveHandle line2; CEnginePrimitiveHandle line3;
    virtual void v00() {}
    CTCDPhysicalObstruction(CThing* parent);
};
CTCDPhysicalObstruction::CTCDPhysicalObstruction(CThing* parent)
    : CTCDriverBase_Calls(parent)
{
}
int main()
{
    unsigned char raw[sizeof(CTCDPhysicalObstruction)]; memset(raw, 0xAA, sizeof(raw)); int parent = 0;
    CTCDPhysicalObstruction* tc = new (raw) CTCDPhysicalObstruction((CThing*)&parent);
    CHECK(g_baseCtor == 1 && g_baseParent == &parent && g_handleCtor == 3);
    CHECK(tc->ThingsToIgnoreObstruction.m_pFirst == 0 && tc->ThingsToIgnoreObstruction.m_pLast == 0 && tc->ThingsToIgnoreObstruction.m_pEnd.p == 0);
    CHECK(tc->line1.Data.Object == 0 && tc->line2.Data.Object == 0 && tc->line3.Data.Object == 0);
    CHECK(tc->LifeTimer == (long)0xAAAAAAAA && *(unsigned long*)&tc->StartPos.X == 0xAAAAAAAA);
    if (fails == 0) printf("004df14b_TEST PASS\n"); else printf("004df14b_TEST FAIL\n");
    return fails;
}