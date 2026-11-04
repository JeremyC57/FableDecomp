// NScript::CQ_BountyHuntScript::CQ_BountyHuntScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: script base, owner/interface pair, own vtable, a registered timer at +0x80, three zeroed words at +0x88..+0x90
// Leaf: ??0CQ_BountyHuntScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe @ 0x00D12CB0, 74 bytes (+ int3 padding), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (manifest label "CHeroMorphDef" is propagated, see GOTCHAS),
// CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the interface global @ 0x0143E8F8.
// Label note: the catalog/manifest name "CV_AmbushScamScript::CV_AmbushScamScript" is propagated over several
// quest-script constructors; this one stores vtable 0x012C9E68 (no RTTI locator: quest-script module), so it is
// named CVtable_012C9E68 like the landed family (0x00E68F70).
// Shape (landed 0x00E68F70 model): the two arguments are stored BEFORE the vtable by an inlined
// `__declspec(novtable)` intermediate base (interface at +0x40, owner at +0x44); the timer member's inline
// constructor registers the timer (CTimer() : Index(RegisterTimer())); the three zero words are init-list members.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
// Naming wave 35: identity level-script ctor: called by the Alloc 00d1cba0 on the new block, stores script vtable 012c9e68; PDB ??0CQ_BountyHuntScript@NScript@@QAE@PAVCScriptDataBase@@...; outer CQ_BountyHuntScript proven by its Main 00d13580 entity registrations; was CVtable_012C9E68::CVtable_012C9E68 (wave3/labels_alloc.tsv); PDB ??0CQ_BountyHuntScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
//   Was `??0CVtable_012C9E68@@QAE@PAXPAVCGameScriptInterface@@@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
//   argument-carrying base model unchanged (data = the first argument, stored at +0x44; the interface at +0x40); same bytes.
class CScriptDataBase;
class CGameScriptInterfaceBase;
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

namespace NScript {
class CQ_BountyHuntScript : public CScriptObject {
public:            // vtable 0x012C9E68
    unsigned char _pad48[0x80 - 0x48];
    CTimer m_timer;                                  // +0x80
    long _pad84;                                     // +0x84
    long m_value88;                                  // +0x88
    long m_value8c;                                  // +0x8c
    long m_value90;                                  // +0x90
    virtual void v00();
    CQ_BountyHuntScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_BountyHuntScript();    // ??1CQ_BountyHuntScript@NScript@@UAE@XZ
};
}
using NScript::CQ_BountyHuntScript;

NScript::CQ_BountyHuntScript::CQ_BountyHuntScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (CGameScriptInterface*)&game), m_value88(0), m_value8c(0), m_value90(0)
{
}