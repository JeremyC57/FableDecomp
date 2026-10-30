// CVtable_012E2428::CVtable_012E2428(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, owner/interface pair, own vtable, a registered timer at +0x7c, five empty CCharStrings at +0x80
// Leaf: ??0CVtable_012E2428
// Retail: Fable.exe @ 0x00E2AB60, 85 bytes (+ int3 padding), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (manifest label "CHeroMorphDef" is propagated, see GOTCHAS),
// CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the interface global @ 0x0143E8F8,
// CCharString::CCharString() @ 0x0099E4B0 (five times, in an inline counted loop).
// Label note: the catalog/manifest name "CV_AmbushScamScript::CV_AmbushScamScript" is propagated over several
// quest-script constructors; this one stores vtable 0x012E2428 (no RTTI locator: quest-script module), so it is
// named CVtable_012E2428 like the landed family (0x00E68F70).
// Shape (landed 0x00E68F70 model): the two arguments are stored BEFORE the vtable by an inlined
// `__declspec(novtable)` intermediate base (interface at +0x40, owner at +0x44); the timer member's inline
// constructor registers the timer (CTimer() : Index(RegisterTimer())); the CCharString[5] member array is built
// by an inline `dec ebx ; jne` loop (no EH in this module, so no `eh vector constructor iterator`).
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

struct CVtable_012E2428 : CScriptObject {            // vtable 0x012E2428
    unsigned char _pad48[0x7c - 0x48];
    CTimer m_timer;                                  // +0x7c
    CCharString m_texts[5];                          // +0x80
    virtual void v00();
    CVtable_012E2428(void* owner, CGameScriptInterface* game);
};

CVtable_012E2428::CVtable_012E2428(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}