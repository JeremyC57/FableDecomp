// NScript::CPersonalScript_EmptyScript::CVillager::Init() -- looks up the "ScrumpyCrate" thing into the villager entity script's Crate handle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF49A0, 191 bytes (padded to 192), plain `ret`. Slot 3 of vtable 0x012F6514.
//   PDB: ?Init@CVillager@CPersonalScript_EmptyScript@NScript@@EAEXXZ is the only CVillager::Init in Ego_r.pdb; its layout
//   (Crate CScriptThing at +0x1c) matches the stores. The owning quest is not proven (several scripts nest a CVillager).
// Callees: CGameScriptInterface slot 72 GetThingWithScriptName(const CCharString&) (+0x120, through the entity script's
//   interface pointer at +4), CCharString::CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC. String "ScrumpyCrate" 0x012F5F24.
// Shape: as CV_MayorsInvitationScript::Init 0x00EB6BD0: implicit conversion temporary for the name, inline counted
//   operator= into the member, then the returned temporary's inline destructor (vtable re-store, release, zeroes,
//   base destructor; the zeroes become immediates once the callee-saved registers are popped).
// Layout (Ego_r.pdb CVillager, entity scripts match the PDB): interface +0x04, ParentClass +0x14, PMasterData +0x18,
//   Crate +0x1c (PImp Data +0x20, Info +0x24).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                     // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPersonalScript_EmptyScript { public:
    struct CVillager {
        virtual void v00();
        CGameScriptInterface* Game;                // +0x04
        unsigned char _base08[0x0c];               // +0x08 (entity script base)
        void* ParentClass;                         // +0x14
        void* PMasterData;                         // +0x18
        CScriptThing Crate;                        // +0x1c
        virtual void Init();
    };
};
void CPersonalScript_EmptyScript::CVillager::Init()
{
    Crate = Game->GetThingWithScriptName("ScrumpyCrate");
}
}