// NScript::CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test checks an unrelated local struct; CGameScriptInterface re-modelled with 87 placeholder slots)
// Leaf: ??0CQ_NewOakValeIntroScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// NScript::CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(void* data, CGameScriptInterface* game) -- quest-script ctor:
//   script base and interface/data pair, vtable 0x012D7A28, eight empty villager-speech vectors and the two timers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAAC00, `ret 8`. Identity: its factory 0x00DBEF70 (`push 10Ch`) is registered at
//   0x00CD6E59 under "S_QNOVI" / "Q_NewOakValeIntro". Callees: CScriptBase::CScriptBase 0x00CB8110,
//   pDSTGame->RegisterTimer (slot 87) twice.
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Layout: PDB (Ego_r) names; retail = PDB - 0x14 (script base), STLport vectors 12 bytes (PDB 16): the flag/counter block
//   +0x48..+0x9b (VictimShake .. lastVillagerSpeechIdx; opaque here -- the PDB places two C3DVectors at odd offsets,
//   which this ctor does not touch), VillagerSpeech* vectors +0x9c..+0xfb, WhichBadDeedsPerformed +0xfc,
//   TalkIntermittentTimer +0x104, WatchTimer +0x108; size 0x10c. Model shared with the landed quest-script ctors.
//   Local model: engine/CGameScriptInterface.h is data-only (no virtual slots).
// CGameScriptInterface: the first 88 virtual slots of vtable 0x01260F0C, named from the slot labels (rtti-vtables.tsv +
//   manifest); only the slot position matters here, so the signatures are simplified (void, no parameters) except
//   RegisterTimer (slot 87, returns the new timer index). Overloaded names carry a _2 suffix.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
// Naming wave 35: identity level-script ctor: called by the Alloc 00dbef70 on the new block, stores script vtable 012d7a28; PDB ??0CQ_NewOakValeIntroScript@NScript@@QAE@PAVCScriptDataBase@@...; outer CQ_NewOakValeIntroScript proven by its Main 00dabac0 entity registrations; was CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript (namespace spelling) (wave3/labels_alloc.tsv); PDB ??0CQ_NewOakValeIntroScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
//   Was `??0CQ_NewOakValeIntroScript@NScript@@QAE@PAXPAVCGameScriptInterface@@@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
//   argument-carrying base model unchanged (data = the first argument, stored at +0x44; the interface at +0x40); same bytes.
class CScriptDataBase;
class CGameScriptInterfaceBase;
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
class CCharString { public: void* PStringData; };
template<class T> struct CRetailVector { T* m_first; T* m_last; T* m_end; CRetailVector() : m_first(0), m_last(0), m_end(0) { } };   // STLport vector (12 bytes)
namespace NScript {
class CQ_NewOakValeIntroScript : public CQuestScriptObject { public:
    unsigned char VictimShake_to_lastVillagerSpeechIdx[0x54];   // +0x48..+0x9b
    CRetailVector<CCharString> VillagerSpeechMaleGood;          // +0x9c
    CRetailVector<CCharString> VillagerSpeechMaleBad;           // +0xa8
    CRetailVector<CCharString> VillagerSpeechMaleBoth;          // +0xb4
    CRetailVector<CCharString> VillagerSpeechMaleNeither;       // +0xc0
    CRetailVector<CCharString> VillagerSpeechFemaleGood;        // +0xcc
    CRetailVector<CCharString> VillagerSpeechFemaleBad;         // +0xd8
    CRetailVector<CCharString> VillagerSpeechFemaleBoth;        // +0xe4
    CRetailVector<CCharString> VillagerSpeechFemaleNeither;     // +0xf0
    bool WhichBadDeedsPerformed[5];                             // +0xfc
    CTimer TalkIntermittentTimer;                               // +0x104
    CTimer WatchTimer;                                          // +0x108
    virtual ~CQ_NewOakValeIntroScript();
    CQ_NewOakValeIntroScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
NScript::CQ_NewOakValeIntroScript::CQ_NewOakValeIntroScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CQuestScriptObject((void*)data, (CGameScriptInterface*)&game)
{
}
}