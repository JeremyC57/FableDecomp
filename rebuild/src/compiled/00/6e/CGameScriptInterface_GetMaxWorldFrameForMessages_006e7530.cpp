// Byte-exact reconstruction of CGameScriptInterface::GetMaxWorldFrameForMessages() const @ 0x006e7530
// Leaf: ?GetMaxWorldFrameForMessages@CGameScriptInterface@@IBEJXZ
// Protected const member (both PDBs IBE); reads a global singleton, walks a pointer chain, returns a long (+0x1c).

struct D { long dummy[7]; long frame; };      // +0x1c
struct C { char pad[0x2c]; D* pD; };           // +0x2c
struct B { char pad0[8]; C* pC; };             // +0x08
struct A { char pad0[0x88]; B* pB; };          // +0x88

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

A* g_singleton = 0;   // at 0x13b89fc

long CGameScriptInterface::GetMaxWorldFrameForMessages() const
{
    B* b = g_singleton->pB;         // [0x13b89fc] -> [+0x88]
    if (b) {
        C* c = b->pC;               // [+8]
        if (c) {
            D* d = c->pD;           // [+0x2c]
            return d->frame;        // [+0x1c]
        }
    }
    return 0;
}