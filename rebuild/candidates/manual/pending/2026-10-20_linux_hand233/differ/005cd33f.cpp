// CTCInventoryStats::OnCreate() -- register the stats screen with the hero-information screen manager, point the inventory at the owner's CCreatureDef StatsScreenInventory def, run the base OnCreate and cache the owner's hero-experience (TC 0x68) and hero-stats (TC 4) components
// Grade: DIFFER(246v244) -- register allocation only: retail keeps this in edi (all callee-saved pushes at entry) and releases the sub-def through ecx (mov ecx,[ebp-4] ; dec [ecx+4]); VC7.1 here puts this in esi with edi's push sunk, and the release loads eax then copies to ecx (the known DecRefToZero/Release eax gap). Tried: named/inline lookups, a self local, GetParent accessor, five release forms.
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CD33F, length 244 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) +
// ("s",on).
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const @ 0x00437CD0 on g_pPlayerGui (0x013B8790), NHeroInformationScreens::
// CManager::RegisterScreen(const CBase*) @ 0x0066EB11, CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*)
// @ 0x00485815, CTCInventoryBase::SetPDef(long) @ 0x005BDB4D, CTCInventoryBase::OnCreate() @ 0x005C0121, std::bitset::test
// @ 0x00410DE0 on the owner's UsingComponents (+0x20), the const component-map LowerBound @ 0x004365B0, the defs' slot 1 on
// release.
// Shape: the landed CTCInventoryExperience::OnCreate (0x0057713F) / CTCInventoryTrade::OnCreate (0x0059C8F2) def dance (the
// null-safe conversion to the CBase secondary base, the owner def add-ref'd and released by hand around GetSubDef, the sub-def
// released without a null test at the end); each component is the landed GetInterface bitset-test + const LowerBound shape
// with the owner re-read, the size-optimised `push 68h ; pop ebx` constant serving the test and the key.
// Layout: CThing PDef +0x70; CCreatureDef StatsScreenInventory retail +0x90 (PDB +0xA0); retail CTCInventoryBase 0x130 bytes
// with CBase at +0x130; CTCInventoryStats CreaturesKilled +0x134 (12-byte map), pMannequin +0x140, UseMannequin +0x144,
// lastCategoryIdx +0x148, pTCHeroExperience +0x14c, pTCHeroStats +0x150 (PDB minus 0x18).
#pragma optimize("y", off)
#pragma optimize("s", on)
__forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    void Release() { if (DecRefToZero(m_refCount)) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CCreatureDef : CDefPointeeBase {
    unsigned char _pad08[0x90 - 0x08];
    long StatsScreenInventory;                  // +0x90 (PDB +0xA0)
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CCreatureDef>* out);   // 0x00485815
};
struct CTCBase;
struct CPresenceBits {
    bool test(long bit) const;                  // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits UsingComponents;              // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    template <class T> __forceinline void GetInterface(long typeId, T** out) const
    {
        if (UsingComponents.test(typeId)) {
            long type = typeId;
            const CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || typeId < it->type)
                it = map->last;
            *out = (T*)it->component;
        }
    }
};
namespace NHeroInformationScreens {
struct CBase {                                  // vtable 0x0123A3E0 (its vptr is the +0x130 word)
    virtual void v00();
};
struct CManager {
    void RegisterScreen(const CBase* screen);   // 0x0066EB11
};
}
struct CPlayerGui {
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
struct CTCHeroExperience;
struct CTCHeroStats;
struct CTCInventoryBase_Stats {                 // retail 0x130 bytes
    virtual void OnCreate();                    // slot 4
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x130 - 0x08];
    void SetPDef(long globalDefIndex);          // 0x005BDB4D
};
class CTCInventoryStats : public CTCInventoryBase_Stats, public NHeroInformationScreens::CBase {
public:
    unsigned char      CreaturesKilled[0xc];    // +0x134 map<unsigned long, long>
    CThing*            pMannequin;              // +0x140
    bool               UseMannequin;            // +0x144
    long               lastCategoryIdx;         // +0x148
    CTCHeroExperience* pTCHeroExperience;       // +0x14c
    CTCHeroStats*      pTCHeroStats;            // +0x150
    virtual void OnCreate();
};

void CTCInventoryStats::OnCreate()
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
    SetPDef(def->StatsScreenInventory);
    CTCInventoryBase_Stats::OnCreate();
    ParentThing->GetInterface(0x68, &pTCHeroExperience);
    ParentThing->GetInterface(4, &pTCHeroStats);
    def.Object->Release();
}
