// IsPlayerThreateningEntity(const CScriptThing& thing) -- nonzero while the hero is wielding a weapon and has the given thing targeted
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?IsPlayerThreateningEntity@@
// Naming wave 30 (family cst): the local non-virtual CScriptThing stand-in (??1CScriptThing@@QAE@XZ) is replaced by engine/CScriptThing.h's retail
//   class: ~CScriptThing 0x004AA840 is virtual (RTTI CScriptThing : CBaseClass; Ego_r.pdb / FableWin.pdb ??1CScriptThing@@UAE@XZ)
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBEED2, 94 bytes, `__fastcall` global (thing in ecx), plain `ret`,
//   returns int (PDB ?IsPlayerThreateningEntity@@YIHABVCScriptThing@@@Z).
// Callees: CGameScriptInterface (global @ 0x0143E8F8) slot 195 IsPlayerWieldingWeapon (+0x30c, 0x00897480) and slot 71
//   GetHeroTargetedThing (+0x11c, 0x0089A490, CScriptThing by value through the hidden pointer), CScriptThing::operator==
//   @ 0x008AAE20, ~CScriptThing @ 0x004AA840.
// Shape: one `&&` returned as int: the right operand's CScriptThing temporary exists only on that path, so VC7.1 keeps
//   a "constructed" flag word in the frame (`and [ebp-4],0` up front, `mov [ebp-4],1` before the call, `test byte
//   [ebp-4],1` before the destructor); operator=='s argument is pushed before the getter's hidden pointer (right to
//   left: the argument of the outer call is evaluated first). Frame-pointer size TU.
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                         // : CBaseClass, vtable 0x01238C8C; virtual ~CScriptThing 0x004AA840
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)

int __fastcall IsPlayerThreateningEntity(const CScriptThing& thing)
{
    return pDSTGame->IsPlayerWieldingWeapon() && pDSTGame->GetHeroTargetedThing() == thing;
}