// NScript::CQ_BreakSiegeScript::CKG_Sheriff::Init() -- the Knothole Glade sheriff script's set-up: clear ToldHeroToGatherGuards and attach its own thing (+8) to the "VILLAGE_KNOTHOLEGLADE" village through the script interface
// Leaf: ?Init@CKG_Sheriff@CQ_BreakSiegeScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D1DFF0, length 147 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("VILLAGE_KNOTHOLEGLADE" @ 0x012CAECC, -1), the script
// interface (+4) virtual slots 72 (+0x120, GetThingWithScriptName: CScriptThing by hidden pointer) and 492 (+0x7b0,
// EntityAttachToVillage), the CScriptThing temporary's inline destructor (vtable 0x01238C8C, counted pimpl release:
// destroy + operator delete @ 0x00BFE9BC, CBaseClass::~CBaseClass @ 0x0099A2E0), CCharString::~CCharString @ 0x0099EAE0.
// Model: exactly the landed sibling CGuardToHelpDefend::Init 0x00D1EAE0 (same village); Ego_r.pdb CKG_Sheriff has the
// interface at +4, its script thing at +8, ParentClass +0x14, PMasterData +0x18 and ToldHeroToGatherGuards +0x1c.
// Shape: the village name is a named local in its own block; the nested call fetches the interface vtable once (edi).
// Naming wave 35: identity reg 00d1d3c6 "KG_Sheriff" in 00d1d100; Alloc 00d1df20; vtable 012caf1c slot 2; PDB ?Init@CKG_Sheriff@CQ_BreakSiegeScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CKG_Sheriff::Init (wave3/labels_alloc.tsv); PDB ?Init@CKG_Sheriff@CQ_BreakSiegeScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CKG_Sheriff@@QAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
struct CScriptThingImp;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern const char g_knotholeGlade[];               // 0x012CAECC ("VILLAGE_KNOTHOLEGLADE")
namespace NScript { class CQ_BreakSiegeScript; } using NScript::CQ_BreakSiegeScript;
struct CQ_SunnyvaleMasterData;
namespace NScript {
class CQ_BreakSiegeScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CKG_Sheriff;
};
}
namespace NScript {
class CQ_BreakSiegeScript::CKG_Sheriff {
public:
    virtual void v00();
    CGameScriptInterface*   Interface;             // +0x04
    CScriptThing            Me;                    // +0x08
    CQ_BreakSiegeScript*    ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    bool                    ToldHeroToGatherGuards;   // +0x1c
private:
    virtual void Init();                          // PDB ?Init@CKG_Sheriff@CQ_BreakSiegeScript@NScript@@EAEXXZ
};
}

void NScript::CQ_BreakSiegeScript::CKG_Sheriff::Init()
{
    ToldHeroToGatherGuards = false;
    {
        CCharString villageName(g_knotholeGlade, -1);
        Interface->EntityAttachToVillage(Me, Interface->GetThingWithScriptName(villageName));
    }
}