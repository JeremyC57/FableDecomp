// CTCInventoryAbilities::OnCreate() -- registers the abilities screen, points the inventory at its def, runs the base OnCreate and initialises abilities and expressions once (not while a hero state is about to load)
// Leaf: OnCreate
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F4A86, 170 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`). The twin of the landed CTCInventoryWeapons::OnCreate
// (0x005C3057) with the abilities def and the one-time initialisation.
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const @ 0x00437CD0 (through (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) @ 0x013B8790),
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
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    typename CDefNonConst<T>::Type* operator->() const { return Object; }
};
class CCreatureDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x84 - 0x08];
    long InventoryDefIndex;                          // +0x84
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00485815 GetSubDef<CCreatureDef> (Ego_r.pdb member template)
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
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
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};

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
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetPHeroInfoScreensManager()->RegisterScreen(this);
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<const CCreatureDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    CCreatureDef* abilitiesDef = const_cast<CCreatureDef*>(def.Object);
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