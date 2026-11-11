// Byte-exact reconstruction of CGameScriptInterface::GetMinWorldFrameForMessages() const @ 0x006e7510
// Leaf: ?GetMinWorldFrameForMessages@CGameScriptInterface@@IBEJXZ
// Protected const member; the body ignores this and reads a global chain (the +0x18 frame word; its twin
// GetMaxWorldFrameForMessages 0x006E7530 reads +0x1c). Ego_r.pdb 0x00710770 / FableWin.pdb 0x024674E0 list Min before Max.
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).

struct L3 { char pad18[0x18]; long value; };          // +0x18
struct L2 { char pad2c[0x2c]; L3* p2c; };              // +0x2c
struct L1 { char pad08[0x08]; L2* p08; };              // +0x08
struct Root { char pad88[0x88]; L1* p88; };            // +0x88

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

long CGameScriptInterface::GetMinWorldFrameForMessages() const
{
    L1* a = reinterpret_cast<Root*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->p88;
    if (a) {
        L2* b = a->p08;
        if (b) {
            return b->p2c->value;
        }
    }
    return 0;
}