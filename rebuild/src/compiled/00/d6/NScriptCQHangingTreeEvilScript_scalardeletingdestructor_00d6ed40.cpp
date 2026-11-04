// NScript::CQ_HangingTreeEvilScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCQ_HangingTreeEvilScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D6ED40, 30 bytes, thiscall, `ret 4`; calls 00d6ed60=NScript::CQ_HangingTreeEvilScript::~CQ_HangingTreeEvilScript ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): outer script vtable 012d1f98 slot 0 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00d69480 (slot 2) registers 8 entity scripts whose PDB Alloc outer is CQ_HangingTreeEvilScript; PDB ??_G/??1CQ_HangingTreeEvilScript@NScript@@; refs=1; was CVtable_012d1f98::`scalar deleting destructor'.
//   ??_GCQ_HangingTreeEvilScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: Ego_r.pdb / FableWin.pdb ??_ECQ_HangingTreeEvilScript@NScript@@UAEPAXI@Z; Ego_r.pdb / FableWin.pdb ??1CQ_HangingTreeEvilScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012d1f98, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CQ_HangingTreeEvilScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z (the row's Leaf ??0CQ_HangingTreeEvilScript, completed from the PDB name set) of the landed row 0x00D68F00 (rebuild/src/compiled/00/d6/NScriptCQHangingTreeEvilScript_CQHangingTreeEvilScript_00d68f00.cpp), its own source unchanged. The constructor body is not part of this graded body.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base (landed model)
    CGameScriptInterface* Game;                         // +0x40 (second argument)
    void*                 Data;                         // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
class CCharString { public: void* PStringData; CCharString(); };   // default ctor 0x0099E4B0
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() : CBaseClass(), PImp() { }
template<class T> struct CRetailVector { T* m_first; T* m_last; T* m_end; CRetailVector() : m_first(0), m_last(0), m_end(0) { } };   // STLport vector (12 bytes)
namespace NScript {
class CQ_HangingTreeEvilScript : public CQuestScriptObject {
public:
    bool IntroCutscenePlayed, BanditsStarted, BanditKilled, PartySuccessfullySetUp, MissionSucceeded, MissionFailed,
         BanditsLeftRegion, SpawnHere, AtWayPoint, SwitchingRegions, RemoveBandits, AllEnemiesDead, QuestStartScreenShown,
         BanditYelledRecently, ExecutionerInterrupted, AudienceLeggit, ShouldFollowHero;   // +0x48..+0x58
    CCharString ReasonForFailing;                       // +0x5c
    long HTE_State;                                     // +0x60
    long CurrentBandits;                                // +0x64
    long BanditsAtNextRegion;                           // +0x68
    long TimerGUIIndex;                                 // +0x6c
    long CarryOverGuards;                               // +0x70
    CScriptThing Executioner;                           // +0x74
    CScriptThing Prisoner;                              // +0x80
    CScriptThing PartySpawnPoint;                       // +0x8c
    CScriptThing BanditExitPoint;                       // +0x98
    CTimer MissionTimer;                                // +0xa4
    CCharString CurrentLevel;                           // +0xa8
    CRetailVector<CScriptThing> CurrentEnemyVector;     // +0xac
    CRetailVector<CScriptThing> CurrentBanditVector;    // +0xb8
    CRetailVector<CCharString> AreasSpawnedIn;          // +0xc4
    virtual ~CQ_HangingTreeEvilScript();
    CQ_HangingTreeEvilScript(void* data, CGameScriptInterface* game);
};
CQ_HangingTreeEvilScript::CQ_HangingTreeEvilScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
}