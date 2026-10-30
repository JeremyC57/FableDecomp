// CVtable_012DF16C::CVtable_012DF16C(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, owner/interface pair, own vtable, a registered timer at +0x48, an empty CCharString at +0x78
// Leaf: ??0CVtable_012DF16C
// Retail: Fable.exe @ 0x00E00690, 59 bytes (+ int3 padding), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (manifest label "CHeroMorphDef" is propagated, see GOTCHAS),
// CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the interface global @ 0x0143E8F8,
// CCharString::CCharString() @ 0x0099E4B0.
// Label note: the catalog/manifest name "CV_AmbushScamScript::CV_AmbushScamScript" is propagated over several
// quest-script constructors; this one stores vtable 0x012DF16C (no RTTI locator: quest-script module), so it is
// named CVtable_012DF16C like the landed family (0x00E68F70).
// Shape (landed 0x00E68F70 model): the two arguments are stored BEFORE the vtable by an inlined
// `__declspec(novtable)` intermediate base (interface at +0x40, owner at +0x44); the timer member's inline
// constructor registers the timer (CTimer() : Index(RegisterTimer())); the timer store is scheduled after the
// string's `lea ecx` and the string is default-constructed out of line.
#include "fable_string.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

struct CVtable_012DF16C : CScriptObject {            // vtable 0x012DF16C
    CTimer m_timer;                                  // +0x48
    unsigned char _pad4c[0x78 - 0x4c];
    CCharString m_text;                              // +0x78
    virtual void v00();
    CVtable_012DF16C(void* owner, CGameScriptInterface* game);
};

CVtable_012DF16C::CVtable_012DF16C(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game), m_text()
{
}