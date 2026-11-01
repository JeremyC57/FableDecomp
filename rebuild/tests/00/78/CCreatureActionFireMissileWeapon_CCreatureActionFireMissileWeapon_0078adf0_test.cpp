// Behaviour test for the implicit CCreatureAction_FireMissileWeapon::~CCreatureAction_FireMissileWeapon 0x0078ADF0: release
// PSpecialShotDef (Delete at the last reference), then destroy PTarget, then ~CCreatureActionBase
#include <cstdio>
static int g_order[16];
static int g_count = 0;
class CDefPointeeBase {
public:
    CDefPointeeBase() : IVRefCount(0) {}
    virtual ~CDefPointeeBase() {}
    virtual void Delete() const { g_order[g_count++] = 1; }
    long Release() const { return --IVRefCount; }
    mutable unsigned long IVRefCount;
};
template <class T> class CDefPointer {
public:
    CDefPointer() : Object(0) {}
    ~CDefPointer() { Release(); }
    void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() { g_order[g_count++] = 2; }
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase() { g_order[g_count++] = 3; }
};
class CThing;
class CMeleeCombatAbilityDef;
class CCreatureAction_FireMissileWeapon : public CCreatureActionBase {
public:
    CIntelligentPointer<const CThing>          PTarget;
    float                                      ShotAccuracyPercentage;
    float                                      ShotDamageMultiplier;
    float                                      ShotBuildupTime;
    bool                                       Decapitate;
    long                                       DelayTimer;
    CDefPointer<const CMeleeCombatAbilityDef>  PSpecialShotDef;
};
int main()
{
    CDefPointeeBase def;
    def.IVRefCount = 1;
    {
        CCreatureAction_FireMissileWeapon action;
        action.PSpecialShotDef.Object = &def;
    }
    if (g_count != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || def.IVRefCount != 0) {
        std::printf("0078adf0_TEST FAIL\n");
        return 1;
    }
    {
        CCreatureAction_FireMissileWeapon noDef;
    }
    if (g_count != 5 || g_order[3] != 2 || g_order[4] != 3) { std::printf("0078adf0_TEST FAIL\n"); return 1; }
    def.IVRefCount = 2;
    CCreatureActionBase* p = new CCreatureAction_FireMissileWeapon;
    static_cast<CCreatureAction_FireMissileWeapon*>(p)->PSpecialShotDef.Object = &def;
    delete p;
    if (g_count != 7 || def.IVRefCount != 1) { std::printf("0078adf0_TEST FAIL\n"); return 1; }
    std::printf("0078adf0_TEST PASS\n");
    return 0;
}