// Behaviour test for CTCInventoryExperience::OnCreate @ 0x0057713F (model copy, stub manager/defs/vectors).
#include <cstdio>
#include <cstdlib>
struct CDefPointeeBase {
    virtual void v00() {}
    virtual void Destroy() { ++destroyed; }
    long m_refCount;
    static long destroyed;
    CDefPointeeBase() : m_refCount(1) {}
};
long CDefPointeeBase::destroyed = 0;
template <class T> struct CDefPointer { T* Object; T* operator->() const { return Object; } };
struct CCreatureDef : CDefPointeeBase { unsigned char _pad08[0x98 - 0x08]; long ExperienceScreenInventory; };
static CCreatureDef g_creatureDef;
struct CDefClassBase : CDefPointeeBase {
    long refCountSeenInGetSubDef;
    bool GetSubDef(CDefPointer<CCreatureDef>* out) { refCountSeenInGetSubDef = m_refCount; out->Object = &g_creatureDef; ++g_creatureDef.m_refCount; return true; }
};
struct CThing { unsigned char _pad00[0x70]; CDefPointer<CDefClassBase> PDef; };
namespace NHeroInformationScreens {
struct CBase { virtual void v00() {} virtual void v01() {} virtual void v02() {} };
struct CManager { const CBase* registered; void RegisterScreen(const CBase* screen) { registered = screen; } };
}
struct CPlayerGui { NHeroInformationScreens::CManager manager; NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const { return const_cast<NHeroInformationScreens::CManager*>(&manager); } };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
struct ExperienceSpendInfo { long currentSpend; long initialSpend; long levelUpSpend; };
struct SSpendInfoVector {
    ExperienceSpendInfo* _M_start; ExperienceSpendInfo* _M_finish; ExperienceSpendInfo* _M_end_of_storage;
    void resize(unsigned long count) { _M_start = (ExperienceSpendInfo*)std::malloc(count * sizeof(ExperienceSpendInfo)); _M_finish = _M_start + count; _M_end_of_storage = _M_finish; }
};
struct ExperienceSpendEvent { bool enable; bool initiate; long time; };
struct CTCInventoryBase_Experience {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void OnCreate() { ++baseCreates; }
    CThing* ParentThing;
    unsigned char _pad08[0x130 - 0x08];
    long pdef; long baseCreates;
    void SetPDef(long globalDefIndex) { pdef = globalDefIndex; }
};
class CTCInventoryExperience : public CTCInventoryBase_Experience, public NHeroInformationScreens::CBase {
public:
    unsigned char _pad_0x134[0x1C8 - 0x134];
    ExperienceSpendEvent ExperienceSpendAdd;
    ExperienceSpendEvent ExperienceSpendRemove;
    unsigned char _pad_0x1d8[0x1E0 - 0x1D8];
    SSpendInfoVector ExperienceSpendValuesPool; SSpendInfoVector ExperienceSpendValuesStats; SSpendInfoVector ExperienceSpendValuesAbility;
    long ExperienceSpendMagnitude;
    long indicatorResets;
    void ResetStatIndicatorValues() { ++indicatorResets; }
    virtual void OnCreate();
};
void CTCInventoryExperience::OnCreate()
{
    g_pPlayerGui->GetPHeroInfoScreensManager()->RegisterScreen(this);
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<CCreatureDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    CCreatureDef* creatureDef = def.Object;
    SetPDef(creatureDef->ExperienceScreenInventory);
    CTCInventoryBase_Experience::OnCreate();
    ResetStatIndicatorValues();
    ExperienceSpendMagnitude = 0;
    ExperienceSpendAdd.enable = false;
    ExperienceSpendAdd.initiate = false;
    ExperienceSpendAdd.time = 0;
    ExperienceSpendRemove.enable = false;
    ExperienceSpendRemove.initiate = false;
    ExperienceSpendRemove.time = 0;
    ExperienceSpendValuesStats.resize(10);
    ExperienceSpendValuesAbility.resize(20);
    ExperienceSpendValuesPool.resize(4);
    if (--creatureDef->m_refCount == 0)
        creatureDef->Destroy();
}
int main()
{
    CDefClassBase thingDef; thingDef.refCountSeenInGetSubDef = -1;
    g_creatureDef.ExperienceScreenInventory = 4242;
    CThing thing; thing.PDef.Object = &thingDef;
    CTCInventoryExperience inv;
    inv.ParentThing = &thing; inv.pdef = -1; inv.baseCreates = 0; inv.indicatorResets = 0;
    inv.ExperienceSpendMagnitude = 99; inv.ExperienceSpendAdd.enable = true; inv.ExperienceSpendAdd.initiate = true; inv.ExperienceSpendAdd.time = 5;
    inv.ExperienceSpendRemove.enable = true; inv.ExperienceSpendRemove.initiate = true; inv.ExperienceSpendRemove.time = 6;
    inv.ExperienceSpendValuesStats._M_start = 0; inv.ExperienceSpendValuesAbility._M_start = 0; inv.ExperienceSpendValuesPool._M_start = 0;
    inv.OnCreate();
    bool ok = true;
    if (g_gui.manager.registered != static_cast<NHeroInformationScreens::CBase*>(&inv)) ok = false;
    if (thingDef.refCountSeenInGetSubDef != 2 || thingDef.m_refCount != 1 || CDefPointeeBase::destroyed != 0) ok = false;   // add-ref around GetSubDef
    if (g_creatureDef.m_refCount != 1) ok = false;                       // the sub-def temporary released
    if (inv.pdef != 4242 || inv.baseCreates != 1 || inv.indicatorResets != 1) ok = false;
    if (inv.ExperienceSpendMagnitude != 0 || inv.ExperienceSpendAdd.enable || inv.ExperienceSpendAdd.initiate || inv.ExperienceSpendAdd.time != 0) ok = false;
    if (inv.ExperienceSpendRemove.enable || inv.ExperienceSpendRemove.initiate || inv.ExperienceSpendRemove.time != 0) ok = false;
    if (inv.ExperienceSpendValuesStats._M_finish - inv.ExperienceSpendValuesStats._M_start != 10) ok = false;
    if (inv.ExperienceSpendValuesAbility._M_finish - inv.ExperienceSpendValuesAbility._M_start != 20) ok = false;
    if (inv.ExperienceSpendValuesPool._M_finish - inv.ExperienceSpendValuesPool._M_start != 4) ok = false;
    // a sub-def whose last reference is the temporary is destroyed
    g_creatureDef.m_refCount = 0;
    inv.OnCreate();
    if (CDefPointeeBase::destroyed != 1) ok = false;
    if (!ok) { std::printf("0057713f_TEST FAIL\n"); return 1; }
    std::printf("0057713f_TEST PASS\n");
    return 0;
}