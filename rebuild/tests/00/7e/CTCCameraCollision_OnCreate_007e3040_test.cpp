// Behaviour test for 007e3040: body copied from src/007e3040.cpp (CTCCameraCollision::vfunc_4 / OnCreate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
template <class T> class CDefPointer;
class CCameraCollisionDef;
static int g_destroyed = 0;
class CDefClassBase
{ public:
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
    bool GetSubDef(CDefPointer<CCameraCollisionDef>* out) const;
};
template <class T>
class CDefPointer
{ public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        T* object = Object;
        if (object != 0)
        {
            if (--object->m_refCount == 0)
                object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};
struct CThingDef : CDefClassBase {};
class CCameraCollisionDef : public CDefClassBase { public:};
static CCameraCollisionDef* g_camDef = 0; static int g_getSubDef = 0; static const CDefClassBase* g_subDefOn = 0;
bool CDefClassBase::GetSubDef(CDefPointer<CCameraCollisionDef>* out) const
{
    ++g_getSubDef; g_subDefOn = this;
    if (!g_camDef) return false;
    ++g_camDef->m_refCount; out->Object = g_camDef; return true;
}
static CThingDef* g_thingDef = 0; static int g_getDef = 0; static long g_getDefIndex = -1;
class CDefinitionManager
{ public:
    bool GetDef(long globalIndex, CDefPointer<CThingDef>* out)
    {
        ++g_getDef; g_getDefIndex = globalIndex;
        if (!g_thingDef) return false;
        ++g_thingDef->m_refCount; out->Object = g_thingDef; return true;
    }
};
static CDefinitionManager g_manager;
class CThing
{ public:
    unsigned char  _pad_0x00[0x8c];
    unsigned short DefGlobalIndex;
};
static int g_baseOnCreate = 0;
#pragma pack(push, 1)
class CTCBase
{ public:
    void*   __vftable;
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    unsigned char _pad_0x0a[0x2];
    void OnCreate() { ++g_baseOnCreate; }
    CDefinitionManager* GetDefinitionManager() const { return &g_manager; }
};
class CTCCameraCollision : public CTCBase
{ public:
    CDefPointer<CCameraCollisionDef> CameraCollisionDef;
    void OnCreate();
};
#pragma pack(pop)

void CTCCameraCollision::OnCreate()
{
    CTCBase::OnCreate();
    CDefPointer<CThingDef> thingDef;
    GetDefinitionManager()->GetDef(ParentThing->DefGlobalIndex, &thingDef);
    thingDef->GetSubDef(&CameraCollisionDef);
}
int main()
{
    static CThing thing; static CThingDef thingDef; static CCameraCollisionDef camDef; thingDef.m_refCount = 1; camDef.m_refCount = 1; g_thingDef = &thingDef; g_camDef = &camDef;
    thing.DefGlobalIndex = 0x2bc;
    {
        CTCCameraCollision c; c.ParentThing = &thing; c.CameraCollisionDef.Object = 0;
        c.OnCreate();
        CHECK(g_baseOnCreate == 1 && g_getDef == 1 && g_getDefIndex == 0x2bc); CHECK(g_getSubDef == 1 && g_subDefOn == &thingDef);
        CHECK(c.CameraCollisionDef.Object == &camDef && camDef.m_refCount == 2); CHECK(thingDef.m_refCount == 1 && g_destroyed == 0);
    }
    CHECK(camDef.m_refCount == 1);
    // a thing def held only by the lookup is destroyed once the sub-def is taken
    thingDef.m_refCount = 0;
    { CTCCameraCollision c; c.ParentThing = &thing; c.CameraCollisionDef.Object = 0; c.OnCreate(); CHECK(g_destroyed == 1 && c.CameraCollisionDef.Object == &camDef); }
    if (fails == 0) printf("007e3040_TEST PASS\n"); else printf("007e3040_TEST FAIL\n");
    return fails;
}