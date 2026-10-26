#include <cstdio>
class CThing { public: int id; };
class CBaseClass { public: virtual ~CBaseClass() { } };
struct CIVCountedPointer { void* Object; CIVCountedPointer() : Object(0) { } };
class CEnginePrimitiveHandle : public CBaseClass { public: CIVCountedPointer Data; CEnginePrimitiveHandle() { } };
struct CTCShotDefinitionBase {
    CThing* m_pParent;
    CTCShotDefinitionBase(CThing* parent) : m_pParent(parent) { }
    virtual ~CTCShotDefinitionBase() { }
};
struct CTCShotDefinitionProjectile : CTCShotDefinitionBase {
    CEnginePrimitiveHandle Line1;
    CTCShotDefinitionProjectile(CThing* parent) : CTCShotDefinitionBase(parent) { }
};
int main()
{
    CThing t;
    CTCShotDefinitionProjectile p(&t);
    if (p.m_pParent != &t || p.Line1.Data.Object != 0) { std::printf("004dc991_TEST FAIL\n"); return 1; }
    std::printf("004dc991_TEST PASS\n");
    return 0;
}