// CTCInventoryAbilities::OnCreate() -- registers the abilities screen, points the inventory at its def, runs the base OnCreate and initialises abilities and expressions once (not while a hero state is about to load)
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F4A86, 170 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`). The twin of the landed CTCInventoryWeapons::OnCreate
// (0x005C3057) with the abilities def and the one-time initialisation.
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const @ 0x00437CD0 (through g_pPlayerGui @ 0x013B8790),
//          NHeroInformationScreens::CManager::RegisterScreen(const CBase*) @ 0x0066EB11, CDefClassBase::GetSubDef<T>
//          (CDefPointer<const T>*) @ 0x00485815, CTCInventoryBase::SetPDef(long) @ 0x005BDB4D, CTCInventoryBase::
//          OnCreate() @ 0x005C0121, CTCBase::PeekWorld() const @ 0x00686A80, InitialiseAbilities() @ 0x005EEDE2,
//          InitialiseExpressions() @ 0x005F3E3E; the defs' slot 1 on release.
// Shape: the screen argument is the null-safe conversion to the NHeroInformationScreens::CBase secondary base at
// +0x130 (`neg ; sbb ; and`); the owner's def is add-ref'd and released by hand around GetSubDef (both null-tested,
// the out pointer zeroed between the load and the test); the abilities sub-def is read once into edi, used for
// SetPDef (+0x84) and released by hand at the end without a null test. The one-time init tests Initialised and the
// world's SerialisingAboutToLoadHeroState (+0x101) as two nested ifs.
// Layout: retail CTCInventoryBase 0x130 bytes, ParentThing +0x04; CTCInventoryAbilities::Initialised +0x141 (PDB
// +0x159); CThing PDef +0x70; retail CWorld SerialisingAboutToLoadHeroState +0x101 (PDB +0x10D).
// Layout (supersedes any local-model note above): engine/CWorld.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld
#pragma optimize("y", off)
#pragma optimize("s", on)

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long m_refCount;                                 // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
struct CInventoryAbilitiesDef : CDefPointeeBase {
    unsigned char _pad08[0x84 - 0x08];
    long InventoryDefIndex;                          // +0x84
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CInventoryAbilitiesDef>* out);   // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CInventoryAbilitiesDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                 // +0x70
};
namespace NHeroInformationScreens {
class CBase { public:                                       // vtable 0x0123A3E0
    virtual void v00();
    virtual void v01();
    virtual void v02();
};
class CManager { public:
    void RegisterScreen(const CBase* screen);        // 0x0066EB11
};
}
class CPlayerGui { public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                     // 0x013B8790

class CWorld : public CWorldDonor {
 public:
};

class CTCBase
{
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void OnCreate();                         // slot 4
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

struct CTCInventoryBase_Abilities : public CTCBase {
    virtual void OnCreate();
                  // retail 0x130 bytes
    CThing* ParentThing;                             // +0x04
    unsigned char _pad08[0x130 - 0x08];
    void SetPDef(long globalDefIndex);               // 0x005BDB4D
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
        if (!PeekWorld().SerialisingAboutToLoadHeroState) {
            InitialiseAbilities();
            InitialiseExpressions();
            Initialised = true;
        }
    }
    if (--abilitiesDef->m_refCount == 0)
        abilitiesDef->Destroy();
}