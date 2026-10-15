#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 0067f810: the prototype whose GetType matches is cloned into PPrimitive (true); no match leaves it alone (false).
struct CPhysicalPrimitiveBase {
    long m_type;
    long m_cloneOf;
    CPhysicalPrimitiveBase(long type) : m_type(type), m_cloneOf(0) {}
    virtual CPhysicalPrimitiveBase* Clone() const { CPhysicalPrimitiveBase* p = new CPhysicalPrimitiveBase(m_type); p->m_cloneOf = m_type; return p; }
    virtual long GetType() const { return m_type; }
    virtual ~CPhysicalPrimitiveBase() {}
};
template <class T> struct CCountedPointer {
    T* m_pObject;
    void* m_pCount;
    void Reset(T* object) { delete m_pObject; m_pObject = object; }
};
struct CPrototypeVector {
    CCountedPointer<CPhysicalPrimitiveBase>* m_pFirst;
    CCountedPointer<CPhysicalPrimitiveBase>* m_pLast;
    CCountedPointer<CPhysicalPrimitiveBase>* m_pEnd;
};
static CPrototypeVector g_PrimitivePrototypes;
struct CPhysicalPrimitiveInit {
    CCountedPointer<CPhysicalPrimitiveBase> PPrimitive;
    bool SetPPrimitiveFromType(long type);
};
bool CPhysicalPrimitiveInit::SetPPrimitiveFromType(long type)
{
    for (CCountedPointer<CPhysicalPrimitiveBase>* it = g_PrimitivePrototypes.m_pFirst;
         it != g_PrimitivePrototypes.m_pLast; ++it) {
        CPhysicalPrimitiveBase* prototype = it->m_pObject;
        if (prototype->GetType() == type) {
            PPrimitive.Reset(prototype->Clone());
            return true;
        }
    }
    return false;
}
int main()
{
    CPhysicalPrimitiveBase a(1), b(4), c(9);
    CCountedPointer<CPhysicalPrimitiveBase> protos[3];
    protos[0].m_pObject = &a; protos[1].m_pObject = &b; protos[2].m_pObject = &c;
    g_PrimitivePrototypes.m_pFirst = protos; g_PrimitivePrototypes.m_pLast = protos + 3; g_PrimitivePrototypes.m_pEnd = protos + 3;
    CPhysicalPrimitiveInit init; init.PPrimitive.m_pObject = 0; init.PPrimitive.m_pCount = 0;
    if (!init.SetPPrimitiveFromType(4) || init.PPrimitive.m_pObject == 0 || init.PPrimitive.m_pObject->m_cloneOf != 4) { std::printf("0067f810_TEST FAIL\n"); return 1; }
    CPhysicalPrimitiveBase* kept = init.PPrimitive.m_pObject;
    if (init.SetPPrimitiveFromType(5) || init.PPrimitive.m_pObject != kept) { std::printf("0067f810_TEST FAIL\n"); return 1; }
    g_PrimitivePrototypes.m_pLast = protos;
    if (init.SetPPrimitiveFromType(1)) { std::printf("0067f810_TEST FAIL\n"); return 1; }
    init.PPrimitive.Reset(0);
    std::printf("0067f810_TEST PASS\n");
    return 0;
}
