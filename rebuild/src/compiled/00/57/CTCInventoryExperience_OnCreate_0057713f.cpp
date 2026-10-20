// CTCInventoryExperience::OnCreate() -- registers the experience screen with the hero-information screen manager, points the inventory at the owner's CCreatureDef ExperienceScreenInventory def, runs the base OnCreate, resets the stat indicators and the spend state and sizes the three spend-record vectors (10 stats, 20 abilities, 4 pools)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057713F, length 215 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const @ 0x00437CD0 on g_pPlayerGui (0x013B8790), NHeroInformationScreens::CManager::
// RegisterScreen(const CBase*) @ 0x0066EB11, CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*) @ 0x00485815 (the
// body looks up "CCreatureDef"; the landed Trade/Weapons OnCreate twins call it the inventory sub-def), CTCInventoryBase::SetPDef(long)
// @ 0x005BDB4D, CTCInventoryBase::OnCreate() @ 0x005C0121, CTCInventoryExperience::ResetStatIndicatorValues() @ 0x0056E5AB,
// vector<ExperienceSpendInfo>::resize(n) @ 0x00579F8A (manifest "_Umove<EWeaponClass*>" is propagated; called with the count on the
// three 12-byte-element vectors), the defs' virtual slot 1 on release.
// Twin of the landed CTCInventoryTrade::OnCreate 0x0059C8F2 / CTCInventoryWeapons::OnCreate 0x005C3057 (same def dance: the owner's
// def pointer in a local, the out slot zeroed, add-ref/release by hand around GetSubDef, the sub-def released without a null test).
// Layout: CThing PDef +0x70; retail CTCInventoryBase 0x130 bytes with the NHeroInformationScreens::CBase secondary base at +0x130;
// CCreatureDef::ExperienceScreenInventory at retail +0x98 (PDB +0xA8); CTCInventoryExperience ExperienceSpendAdd +0x1C8 and
// ExperienceSpendRemove +0x1D0 ({bool enable, bool initiate, long time}), the three vectors at +0x1E0/+0x1EC/+0x1F8 and
// ExperienceSpendMagnitude at +0x204 (PDB offsets minus 0x44: 16-byte debug containers).
// Shape: the zero stores (one zero register, ebx) are sunk below the first resize call's argument push and `lea ecx`; they are written
// in retail's store order (magnitude first, then the two events).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CCreatureDef : CDefPointeeBase {
    unsigned char _pad08[0x98 - 0x08];
    long ExperienceScreenInventory;             // +0x98 (PDB +0xA8)
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CCreatureDef>* out);   // 0x00485815
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;            // +0x70
};
namespace NHeroInformationScreens {
struct CBase {                                  // vtable 0x0123A3E0
    virtual void v00();
    virtual void v01();
    virtual void v02();
};
struct CManager {
    void RegisterScreen(const CBase* screen);   // 0x0066EB11
};
}
struct CPlayerGui {
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
struct ExperienceSpendInfo {
    long currentSpend;      // +0x0
    long initialSpend;      // +0x4
    long levelUpSpend;      // +0x8
};
struct SSpendInfoVector {
    ExperienceSpendInfo* _M_start;
    ExperienceSpendInfo* _M_finish;
    ExperienceSpendInfo* _M_end_of_storage;
    void resize(unsigned long count);           // 0x00579F8A
};
struct ExperienceSpendEvent {                   // 8 bytes
    bool enable;                                // +0
    bool initiate;                              // +1
    long time;                                  // +4
};
struct CTCInventoryBase_Experience {            // retail 0x130 bytes
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void OnCreate();                    // slot 4
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x130 - 0x08];
    void SetPDef(long globalDefIndex);          // 0x005BDB4D
};
class CTCInventoryExperience : public CTCInventoryBase_Experience, public NHeroInformationScreens::CBase {
public:
    unsigned char _pad_0x134[0x1C8 - 0x134];
    ExperienceSpendEvent ExperienceSpendAdd;        // +0x1C8
    ExperienceSpendEvent ExperienceSpendRemove;     // +0x1D0
    unsigned char _pad_0x1d8[0x1E0 - 0x1D8];
    SSpendInfoVector ExperienceSpendValuesPool;     // +0x1E0
    SSpendInfoVector ExperienceSpendValuesStats;    // +0x1EC
    SSpendInfoVector ExperienceSpendValuesAbility;  // +0x1F8
    long ExperienceSpendMagnitude;                  // +0x204
    void ResetStatIndicatorValues();                // 0x0056E5AB
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