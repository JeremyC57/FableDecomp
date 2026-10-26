// Behaviour test for 005da6e0: body copied from src/005da6e0.cpp (CTCWeapon::OnCreate), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CWeaponDef { public:
    unsigned char _pad_0x00[0xc5];
    bool          ProjectileWeaponLoaded;
};
struct CWeaponDefPointer {
    CWeaponDef* Object;
    CWeaponDef* operator->() const { return Object; }
};
static CWeaponDef* g_weaponDef = 0; static int g_getSubDef = 0; static int g_destroyed = 0;
class CDefClassBase { public:
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;
    bool GetSubDef(CWeaponDefPointer* out);
};
bool CDefClassBase::GetSubDef(CWeaponDefPointer* out)
{
    ++g_getSubDef;
    if (!g_weaponDef) return false;
    out->Object = g_weaponDef; return true;
}
struct CDefClassPointer {
    CDefClassBase* Object;
    CDefClassPointer() : Object(0) {}
};
struct CDefClassReference {
    CDefClassBase* Object;
    CDefClassReference(CDefClassBase* object) : Object(object) {}
    ~CDefClassReference()
    {
        if (Object && --Object->m_count == 0)
            Object->Destroy();
    }
    CDefClassBase* operator->() const { return Object; }
};
static CDefClassBase* g_classDef = 0; static long g_askedIndex = -1; static int g_getDef = 0;
class CDefinitionManager { public:
    bool GetDef(long index, CDefClassPointer* out);
};
bool CDefinitionManager::GetDef(long index, CDefClassPointer* out)
{
    ++g_getDef; g_askedIndex = index;
    if (!g_classDef) return false;
    ++g_classDef->m_count; out->Object = g_classDef; return true;
}
static CDefinitionManager g_manager;
class CThing { public:
    unsigned char  _pad_0x00[0x8c];
    unsigned short DefClassIndex;
};
class CTCWeapon { public:
    virtual void v00() {}
    CThing*           ParentThing;
    unsigned char     _pad_0x08[0x4];
    CWeaponDefPointer WeaponDef;
    bool              ProjectileWeaponLoaded;
    CDefinitionManager* GetDefinitionManager() { return &g_manager; }
    void OnCreate();
};
void CTCWeapon::OnCreate()
{
    unsigned short index = ParentThing->DefClassIndex;
    if (index) {
        CDefClassPointer out;
        GetDefinitionManager()->GetDef(index, &out);
        CDefClassReference defClass(out.Object);
        defClass->GetSubDef(&WeaponDef);
        ProjectileWeaponLoaded = WeaponDef->ProjectileWeaponLoaded;
    }
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CWeaponDef weaponDef;
    memset(&thing, 0, sizeof(thing)); thing.DefClassIndex = 23; classDef.m_count = 1; weaponDef.ProjectileWeaponLoaded = true; g_classDef = &classDef; g_weaponDef = &weaponDef;
    CTCWeapon tc; tc.ParentThing = &thing; tc.WeaponDef.Object = 0; tc.ProjectileWeaponLoaded = false;
    tc.OnCreate();
    CHECK(g_getDef == 1 && g_askedIndex == 23 && g_getSubDef == 1); CHECK(tc.WeaponDef.Object == &weaponDef); CHECK(tc.ProjectileWeaponLoaded); CHECK(classDef.m_count == 1 && g_destroyed == 0);
    weaponDef.ProjectileWeaponLoaded = false; classDef.m_count = 0; tc.OnCreate(); CHECK(!tc.ProjectileWeaponLoaded && g_destroyed == 1);
    thing.DefClassIndex = 0; tc.WeaponDef.Object = 0; tc.OnCreate(); CHECK(g_getDef == 2 && tc.WeaponDef.Object == 0);
    if (fails == 0) printf("005da6e0_TEST PASS\n"); else printf("005da6e0_TEST FAIL\n");
    return fails;
}