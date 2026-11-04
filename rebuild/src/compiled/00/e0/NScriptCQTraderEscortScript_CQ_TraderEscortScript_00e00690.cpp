// NScript::CQ_TraderEscortScript::CQ_TraderEscortScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: script base, owner/interface pair, own vtable, a registered timer at +0x48, an empty CCharString at +0x78
// Leaf: ??0CQ_TraderEscortScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
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
// Naming wave 35: identity out-of-line copy of the ctor the Alloc 00e0af60 inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores script vtable 012df16c, ret 8 (two args); PDB ??0CQ_TraderEscortScript@NScript@@QAE@PAVCScriptDataBase@@...; outer CQ_TraderEscortScript proven by its Main 00e00db0 entity registrations; was CVtable_012DF16C::CVtable_012DF16C (wave3/labels_alloc.tsv); PDB ??0CQ_TraderEscortScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
//   Was `??0CVtable_012DF16C@@QAE@PAXPAVCGameScriptInterface@@@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
//   argument-carrying base model unchanged (data = the first argument, stored at +0x44; the interface at +0x40); same bytes.
class CScriptDataBase;
class CGameScriptInterfaceBase;
#include "fable_string.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
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
class CQ_TraderEscortScript : public CScriptObject {
public:            // vtable 0x012DF16C
    CTimer m_timer;                                  // +0x48
    unsigned char _pad4c[0x78 - 0x4c];
    CCharString m_text;                              // +0x78
    virtual void v00();
    CQ_TraderEscortScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_TraderEscortScript();    // ??1CQ_TraderEscortScript@NScript@@UAE@XZ
};
}
using NScript::CQ_TraderEscortScript;

NScript::CQ_TraderEscortScript::CQ_TraderEscortScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (CGameScriptInterface*)&game), m_text()
{
}