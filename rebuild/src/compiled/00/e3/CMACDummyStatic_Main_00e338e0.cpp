// CM_AC_DummyStatic::Main() -- straw-dummy entity script: create OBJECT_STRAW_DUMMY_01 on Me at the "AC_DummyStatic" bone, hand the new dummy Me's data string and turn it to face the "AC_PlayerRing" thing
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E338E0, 319 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; Me's
//   CScriptThing slot 3 GetDataString (+0x0c), the dummy's impl slot 4 SetDataString (+0x10); CGameScriptInterface slots 72
//   GetThingWithScriptName (+0x120), 99 CreateObjectOnEntity (+0x18c), 475 EntitySetFacingAngleTowardsThing (+0x76c).
// Shape: the landed CQ_GuildTrainingDepartureScript CScriptThing model: both script things are named locals whose inline
//   destructors (vtable store, counted-pointer release, ~CBaseClass) run in reverse order at the end, the vtable constant
//   kept in esi; SetDataString is CScriptThing's INLINE virtual (forward to PImp when set), devirtualised and inlined on the
//   local while the call on the Me member still dispatches through the vtable.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline void CScriptThing::SetDataString(const CCharString& data) { if (PImp.Data) PImp.Data->SetDataString(data); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CM_AC_DummyStatic {
public:
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    void Main();
};

void CM_AC_DummyStatic::Main()
{
    CScriptThing dummy = Game->CreateObjectOnEntity("OBJECT_STRAW_DUMMY_01", Me, "AC_DummyStatic");
    dummy.SetDataString(Me.GetDataString());
    CScriptThing ring = Game->GetThingWithScriptName("AC_PlayerRing");
    Game->EntitySetFacingAngleTowardsThing(dummy, ring, false);
}