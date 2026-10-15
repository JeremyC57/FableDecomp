#include <cstdio>

struct CAGraphicAppearanceComponent;
static CAGraphicAppearanceComponent* g_registered = 0; static int g_count = 0;
struct CTCGraphicAppearance {
    void RegisterComponent(CAGraphicAppearanceComponent* c) { g_registered = c; ++g_count; }
};
struct CThing { CTCGraphicAppearance* PTCGraphicAppearance; };
struct CTCBase {
    virtual ~CTCBase() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* owner) : ParentThing(owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
struct CAGraphicAppearanceComponent { virtual ~CAGraphicAppearanceComponent() { } };
struct CPrimitive;
struct CRetailVector_CPrimitivePtr {
    CPrimitive** m_pFirst; CPrimitive** m_pLast; CPrimitive** m_pEnd;
    CRetailVector_CPrimitivePtr() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};
struct CMultiStaticMeshDef;
struct CDefPointer_CMultiStaticMeshDef { const CMultiStaticMeshDef* m_pDef; CDefPointer_CMultiStaticMeshDef() : m_pDef(0) { } };
struct CTCGraphicAppearanceMultipleStaticMeshes : CTCBase, CAGraphicAppearanceComponent {
    CRetailVector_CPrimitivePtr     PrimHandles;
    CDefPointer_CMultiStaticMeshDef PDef;
    CTCGraphicAppearanceMultipleStaticMeshes(CThing* owner)
        : CTCBase(owner), CAGraphicAppearanceComponent(), PrimHandles(), PDef()
    {
        ParentThing->PTCGraphicAppearance->RegisterComponent(this);
    }
};

int main()
{
    CTCGraphicAppearance app; CThing owner; owner.PTCGraphicAppearance = &app;
    CTCGraphicAppearanceMultipleStaticMeshes m(&owner);
    CAGraphicAppearanceComponent* asComponent = &m;
    if (g_count != 1 || g_registered != asComponent || m.ParentThing != &owner || m.PrimHandles.m_pFirst ||
        m.PrimHandles.m_pLast || m.PrimHandles.m_pEnd || m.PDef.m_pDef)
        { std::printf("007e14c0_TEST FAIL\n"); return 1; }
    std::printf("007e14c0_TEST PASS\n");
    return 0;
}
