// NScript::CQ_TraderEscortScript::CDarkwoodTrader::`scalar deleting destructor'(unsigned int flags) -- the out-of-line implicit ~CDarkwoodTrader 0x00E01710, then free the entity when bit 0 is set
// Leaf: ??_GCDarkwoodTrader@CQ_TraderEscortScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E016F0, 30 bytes: push esi; mov esi,ecx; call ~CDarkwoodTrader 0x00E01710; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Identity (two sources): slot 0 of the RTTI-less entity vtable 0x012DF424, the vtable the landed ctor 0x00E01670
//   (??0CDarkwoodTrader@CQ_TraderEscortScript@NScript@@...) stores and whose slot 2 is the landed ?Init@CDarkwoodTrader@... 0x00E04BD0;
//   Ego_r.pdb ??_GCDarkwoodTrader@CQ_TraderEscortScript@NScript@@UAEPAXI@Z (0x00E43E80, right after ??1 0x00E43DF0; FableWin.pdb has the
//   /Od ??_E and ??1). Its only callee is the landed ??1CDarkwoodTrader@CQ_TraderEscortScript@NScript@@UAE@XZ 0x00E01710.
//   Name-fix NF163 (q4053): was a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor stand-in calling
//   @CLandscapeBackgroundPatch_dtor_00e01710@4 (catalog CVtable_012df424).
// Class: engine/CQ_TraderEscortScript.h (shared header: nested class CDarkwoodTrader, ParentClass reference; opt-in
//   FABLE_RETAIL_CQ_TRADERESCORTSCRIPT_DARKWOODTRADER_CTOR declares the PDB ctor). Only the ??_G is graded; CTimer::~CTimer is expanded
//   inline (engine/CTimer.h wave-8 practice: interface slot 88 DeregisterTimer through pDSTGame), as in the landed ~CDarkwoodTrader 0x00E01710.
void operator delete(void* p);                          // 0x00BFE9BC
#include <new>
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS)
#include "engine/CTimer.h"
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#define FABLE_RETAIL_CQ_TRADERESCORTSCRIPT_DARKWOODTRADER_CTOR
#include "engine/CQ_TraderEscortScript.h"
// The PDB ctor (retail out of line at 0x00E01670, owned by that landed row) is defined inline here, so this object emits it, if at all,
//   as a pick-any COMDAT.
inline NScript::CQ_TraderEscortScript::CDarkwoodTrader::CDarkwoodTrader(CQ_TraderEscortScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
namespace {
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, as 0x009566C0): a placement construction through the inline ctor references the
// entity vtable, which makes this object emit the vtable with its ??_G (the graded leaf) and the implicit ~CDarkwoodTrader as pick-any
// COMDATs. Internal to this TU; never called.
void ReferenceDarkwoodTraderVtable(void* where, NScript::CQ_TraderEscortScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    new (where) NScript::CQ_TraderEscortScript::CDarkwoodTrader(parent, data, me);
}
}
