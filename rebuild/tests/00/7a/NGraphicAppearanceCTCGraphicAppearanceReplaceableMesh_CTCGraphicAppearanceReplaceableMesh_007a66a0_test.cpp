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
struct CReplaceableMeshDef;
struct CDefPointer_CReplaceableMeshDef { const CReplaceableMeshDef* m_pDef; CDefPointer_CReplaceableMeshDef() : m_pDef(0) { } };
struct CTCGraphicAppearanceReplaceableMesh : CTCBase, CAGraphicAppearanceComponent {
    bool                            Replaced;
    unsigned long                   MeshIndex;
    CPrimitive*                     PrimHandle;
    CDefPointer_CReplaceableMeshDef PDef;
    CTCGraphicAppearanceReplaceableMesh(CThing* owner)
        : CTCBase(owner), CAGraphicAppearanceComponent(), Replaced(false), MeshIndex(0), PrimHandle(0), PDef()
    {
        ParentThing->PTCGraphicAppearance->RegisterComponent(this);
    }
};

int main()
{
    CTCGraphicAppearance app; CThing owner; owner.PTCGraphicAppearance = &app;
    CTCGraphicAppearanceReplaceableMesh m(&owner);
    CAGraphicAppearanceComponent* asComponent = &m;
    if (g_count != 1 || g_registered != asComponent || m.ParentThing != &owner || m.Replaced || m.MeshIndex ||
        m.PrimHandle || m.PDef.m_pDef)
        { std::printf("007a66a0_TEST FAIL\n"); return 1; }
    std::printf("007a66a0_TEST PASS\n");
    return 0;
}