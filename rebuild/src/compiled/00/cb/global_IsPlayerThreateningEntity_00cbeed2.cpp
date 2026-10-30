// IsPlayerThreateningEntity(const CScriptThing& thing) -- nonzero while the hero is wielding a weapon and has the given thing targeted
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ?IsPlayerThreateningEntity@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBEED2, 94 bytes, `__fastcall` global (thing in ecx), plain `ret`,
//   returns int (PDB ?IsPlayerThreateningEntity@@YIHABVCScriptThing@@@Z).
// Callees: CGameScriptInterface (global @ 0x0143E8F8) slot 195 IsPlayerWieldingWeapon (+0x30c, 0x00897480) and slot 71
//   GetHeroTargetedThing (+0x11c, 0x0089A490, CScriptThing by value through the hidden pointer), CScriptThing::operator==
//   @ 0x008AAE20, ~CScriptThing @ 0x004AA840.
// Shape: one `&&` returned as int: the right operand's CScriptThing temporary exists only on that path, so VC7.1 keeps
//   a "constructed" flag word in the frame (`and [ebp-4],0` up front, `mov [ebp-4],1` before the call, `test byte
//   [ebp-4],1` before the destructor); operator=='s argument is pushed before the getter's hidden pointer (right to
//   left: the argument of the outer call is evaluated first). Frame-pointer size TU.
class CScriptThing { public:
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing();                                    // 0x004AA840
    bool operator==(const CScriptThing& other) const;   // 0x008AAE20
    void* m_vtable;
    void* m_pImpData;
    void* m_pImpInfo;
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

int __fastcall IsPlayerThreateningEntity(const CScriptThing& thing)
{
    return g_pGameScriptInterface->IsPlayerWieldingWeapon() && g_pGameScriptInterface->GetHeroTargetedThing() == thing;
}