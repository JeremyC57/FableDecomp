// CTCInventoryAbilities::OnCreate() -- registers the abilities screen, points the inventory at its def, runs the base OnCreate and initialises abilities and expressions once (not while a hero state is about to load)
// Grade: DIFFER(172v170) -- scheduling only: retail loads the abilities def straight into edi (`mov edi,[ebp-4] ; push [edi+84h]`), ours into eax and copies it to edi after the push (named local, holder, index local and && forms tried; the landed CTCInventoryWeapons::OnCreate twin matches with the same source shape)
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F4A86, 170 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`). The twin of the landed CTCInventoryWeapons::OnCreate
// (0x005C3057) with the abilities def and the one-time initialisation.
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const @ 0x00437CD0 (through g_pPlayerGui @ 0x013B8790),
//          NHeroInformationScreens::CManager::RegisterScreen(const CBase*) @ 0x0066EB11, CDefClassBase::GetSubDef<T>
//          (CDefPointer<const T>*) @ 0x00485815, CTCInventoryBase::SetPDef(long) @ 0x005BDB4D, CTCInventoryBase::
//          OnCreate() @ 0x005C0121, CTCBase::GetWorld() const @ 0x00686A80, InitialiseAbilities() @ 0x005EEDE2,
//          InitialiseExpressions() @ 0x005F3E3E; the defs' slot 1 on release.
// Shape: the screen argument is the null-safe conversion to the NHeroInformationScreens::CBase secondary base at
// +0x130 (`neg ; sbb ; and`); the owner's def is add-ref'd and released by hand around GetSubDef (both null-tested,
// the out pointer zeroed between the load and the test); the abilities sub-def is read once into edi, used for
// SetPDef (+0x84) and released by hand at the end without a null test. The one-time init tests Initialised and the
// world's SerialisingAboutToLoadHeroState (+0x101) as two nested ifs.
// Layout: retail CTCInventoryBase 0x130 bytes, ParentThing +0x04; CTCInventoryAbilities::Initialised +0x141 (PDB
// +0x159); CThing PDef +0x70; retail CWorld SerialisingAboutToLoadHeroState +0x101 (PDB +0x10D).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long m_refCount;                                 // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CInventoryAbilitiesDef : CDefPointeeBase {
    unsigned char _pad08[0x84 - 0x08];
    long InventoryDefIndex;                          // +0x84
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CInventoryAbilitiesDef>* out);   // 0x00485815
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                 // +0x70
};
namespace NHeroInformationScreens {
struct CBase {                                       // vtable 0x0123A3E0
    virtual void v00();
    virtual void v01();
    virtual void v02();
};
struct CManager {
    void RegisterScreen(const CBase* screen);        // 0x0066EB11
};
}
struct CPlayerGui {
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

struct CWorld {
    unsigned char _pad00[0x101];
    bool SerialisingAboutToLoadHeroState;            // +0x101 (PDB +0x10D)
};

struct CTCInventoryBase_Abilities {                  // retail 0x130 bytes
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void OnCreate();                         // slot 4
    CThing* ParentThing;                             // +0x04
    unsigned char _pad08[0x130 - 0x08];
    void SetPDef(long globalDefIndex);               // 0x005BDB4D
    CWorld* GetWorld() const;                        // 0x00686A80 (CTCBase)
};

class CTCInventoryAbilities : public CTCInventoryBase_Abilities, public NHeroInformationScreens::CBase {
public:
    unsigned char _pad_0x134[0x0D];
    bool          Initialised;                       // +0x141
    void InitialiseAbilities();                      // 0x005EEDE2
    void InitialiseExpressions();                    // 0x005F3E3E
    virtual void OnCreate();
};

void CTCInventoryAbilities::OnCreate()
{
    g_pPlayerGui->GetPHeroInfoScreensManager()->RegisterScreen(this);
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<CInventoryAbilitiesDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    CInventoryAbilitiesDef* abilitiesDef = def.Object;
    SetPDef(abilitiesDef->InventoryDefIndex);
    CTCInventoryBase_Abilities::OnCreate();
    if (!Initialised) {
        if (!GetWorld()->SerialisingAboutToLoadHeroState) {
            InitialiseAbilities();
            InitialiseExpressions();
            Initialised = true;
        }
    }
    if (--abilitiesDef->m_refCount == 0)
        abilitiesDef->Destroy();
}
