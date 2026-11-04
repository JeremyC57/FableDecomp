// NScript::CQ_TraderEscortScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCQ_TraderEscortScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E0AFB0, 48 bytes, thiscall, `ret 4`; calls 0099eae0=CCharString::~CCharString ; 00cd4470=CTimer::~CTimer ; 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): outer script vtable 012df16c slot 0 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e00db0 (slot 2) registers 14 entity scripts whose PDB Alloc outer is CQ_TraderEscortScript; PDB ??_G/??1CQ_TraderEscortScript@NScript@@; refs=1; was CVtable_012df16c::`scalar deleting destructor'.
//   ??_GCQ_TraderEscortScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: Ego_r.pdb / FableWin.pdb ??_GCQ_TraderEscortScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CQ_TraderEscortScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012df16c, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CQ_TraderEscortScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00E00690 (rebuild/src/compiled/00/e0/NScriptCQTraderEscortScript_CQ_TraderEscortScript_00e00690.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
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
};
}
using NScript::CQ_TraderEscortScript;

NScript::CQ_TraderEscortScript::CQ_TraderEscortScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (CGameScriptInterface*)&game), m_text()
{
}