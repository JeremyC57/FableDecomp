// NScript::CIsThingConscious::~CIsThingConscious() -- this class's vtable (0x012C2FE8), the CScriptThing member at +4 destroyed inline (its vtable 0x01238C8C, the counted block released: destroy and delete at zero, both words cleared, CBaseClass::~CBaseClass @ 0x0099A2E0), then the predicate base's vtable (0x012C2FF4)
// Leaf: ??1CIsThingConscious@NScript@@UAE@XZ
// Name fix (agentNF24, land1011de): was the stand-in CScriptThingFunctor_00ef7ab0::~CScriptThingFunctor_00ef7ab0 over a
//   CThingFunctorBase_012c2ff4 base. Vtable 0x012C2FE8 is NScript::CIsThingConscious's (engine/CIsThingConscious.h: Clone 0x00CDDAE0
//   stores it; the landed 0x00D408B0 binds ??_7CIsThingConscious@NScript@@6B@ there), and the header names this address its destructor;
//   Ego_r.pdb + FableWin.pdb ??1CIsThingConscious@NScript@@UAE@XZ. The final store is NScript::CPredicateBase's vtable 0x012C2FF4
//   (agentFAM08: slot 2 the landed ??_GCPredicateBase@NScript@@UAEPAXI@Z 0x00CDDB40). Same shape as ~CIsThingAlive 0x00E241B0.
// Model: the shared engine/CIsThingConscious.h, CPredicateBase.h and CScriptThing.h; CScriptThing's destructor is defined inline after
//   the include (retail expands it here, as the landed 0x00EF84D0 does).
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() {}
#include "engine/CIsThingConscious.h"

NScript::CIsThingConscious::~CIsThingConscious()
{
}
