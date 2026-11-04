// NScript::CQ_HeroSoulsArenaScript::CQ_HeroSoulsArenaScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: script base, owner/interface pair, own vtable, a registered timer at +0x58, three zeroed words at +0x5c..+0x64
// Leaf: ??0CQ_HeroSoulsArenaScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe @ 0x00D7C700, 62 bytes (+ int3 padding), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (manifest label "CHeroMorphDef" is propagated, see GOTCHAS),
// CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the interface global @ 0x0143E8F8.
// Label note: the catalog/manifest name "CV_AmbushScamScript::CV_AmbushScamScript" is propagated over several
// quest-script constructors; this one stores vtable 0x012D3994 (no RTTI locator: quest-script module), so it is
// named CVtable_012D3994 like the landed family (0x00E68F70).
// Shape (landed 0x00E68F70 model): the two arguments are stored BEFORE the vtable by an inlined
// `__declspec(novtable)` intermediate base (interface at +0x40, owner at +0x44); the timer member's inline
// constructor registers the timer (CTimer() : Index(RegisterTimer())); the three zero words are init-list members.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
// Naming wave 35: identity out-of-line copy of the ctor the Alloc 00d80ba0 inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores script vtable 012d3994, ret 8 (two args); PDB ??0CQ_HeroSoulsArenaScript@NScript@@QAE@PAVCScriptDataBase@@...; outer CQ_HeroSoulsArenaScript proven by its Main 00d7c890 entity registrations; was CVtable_012D3994::CVtable_012D3994 (wave3/labels_alloc.tsv); PDB ??0CQ_HeroSoulsArenaScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
//   Was `??0CVtable_012D3994@@QAE@PAXPAVCGameScriptInterface@@@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
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
class CQ_HeroSoulsArenaScript : public CScriptObject {
public:            // vtable 0x012D3994
    unsigned char _pad48[0x58 - 0x48];
    CTimer m_timer;                                  // +0x58
    long m_value5c;                                  // +0x5c
    long m_value60;                                  // +0x60
    long m_value64;                                  // +0x64
    virtual void v00();
    CQ_HeroSoulsArenaScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_HeroSoulsArenaScript();    // ??1CQ_HeroSoulsArenaScript@NScript@@UAE@XZ
};
}
using NScript::CQ_HeroSoulsArenaScript;

NScript::CQ_HeroSoulsArenaScript::CQ_HeroSoulsArenaScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (CGameScriptInterface*)&game), m_value5c(0), m_value60(0), m_value64(0)
{
}