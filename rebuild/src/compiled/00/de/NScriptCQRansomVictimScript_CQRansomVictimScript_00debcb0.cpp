// NScript::CQ_RansomVictimScript::~CQ_RansomVictimScript() -- implicit quest-script destructor: destroy seh_Ransommer, seh_Brother, seh_Victim, seh_Father and seh_Guard, deregister NPCCommentTimer, destroy ActiveEnemyName, then ~CScriptBase
// Leaf: ??1CQ_RansomVictimScript@NScript@@UAE@XZ
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEBCB0, 344 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510; sole caller the
//   scalar deleting dtor 0x00DEBC90 (slot 0 of the RTTI-less quest vtable 0x012DCE70).
// Identity (proven): vtable 0x012DCE70 is stored only by the ctor 0x00DE0320, whose only caller is the Alloc 0x00DEBC60
//   (`push 0BCh ; call new ; ... call 0x00DE0320`); CScriptInfoManager::RegisterScripts stores that Alloc at 0x00CD73B6 right
//   after `push "S_QRV"` (0x012F7854); refs/script_recovery/conversion_queue.tsv agrees (Q_RansomVictim / S_QRV / 0x00DEBC60).
//   No vtable store in the body = compiler-generated dtor. Catalog label right (class needs the NScript namespace).
// Layout: Ego_r.pdb CQ_RansomVictimScript (0xD0) at retail offsets (Ego_r - 0x14, no STLport container): twelve bools
//   +0x48..+0x53, HitChief +0x54, HitEvilBrotherAndProtection +0x58, SomeoneDealingWithBeingHit +0x5c, KidnappersKilled +0x60,
//   ActiveEnemyName +0x64, NPCCommentTimer +0x68, seh_Guard +0x6c, seh_Father +0x7c, seh_Victim +0x8c, seh_Brother +0x9c,
//   seh_Ransommer +0xac (0xBC = the Alloc size).
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunk 665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// Callees: the seh_* implicit dtors inline (PImp release: DeleteFunc slot, operator delete 0x00BFE9BC; vtable 0x0126008C store,
//   ~CBaseObject 0x0099A430), CTimer::~CTimer inline (engine/CTimer.h: pDSTGame 0x0143E8F8 slot 88
//   DeregisterTimer), ~CCharString 0x0099EAE0, ~CScriptBase 0x00CBD510.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00DE0320), which emits the vtable, ??_G and this implicit dtor.
void operator delete(void* p);                          // 0x00BFE9BC
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
#include "engine/CTimer.h"
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptBase.h"
class CCharStringData;
class CCharString { public:
    ~CCharString();                                                      // 0x0099EAE0
    CCharStringData* PStringData;
};
class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                                      // +0x40
    T*                        MasterScriptData;                          // +0x44
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_RansomVictimScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_RansomVictimScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    bool                                       QuestStartScreened;       // +0x48
    bool                                       MissionSucceeded;         // +0x49
    bool                                       MissionFailed;            // +0x4a
    bool                                       RescuedVictim;            // +0x4b
    bool                                       RansommerDied;            // +0x4c
    bool                                       RansomHelperStarted;      // +0x4d
    bool                                       RansomHelperSpoken;       // +0x4e
    bool                                       ExtraAssistanceGiven;     // +0x4f
    bool                                       RansomHelpersAttacked;    // +0x50
    bool                                       ToldGuardToWaitOutside;   // +0x51
    bool                                       EnteredNewRegion;         // +0x52
    bool                                       ThanksCutsceneStarting;   // +0x53
    long                                       HitChief;                 // +0x54
    long                                       HitEvilBrotherAndProtection;  // +0x58
    bool                                       SomeoneDealingWithBeingHit;   // +0x5c
    long                                       KidnappersKilled;         // +0x60
    CCharString                                ActiveEnemyName;          // +0x64
    CTimer                                     NPCCommentTimer;          // +0x68
    CScriptGameResourceObjectScriptedThingBase seh_Guard;                // +0x6c
    CScriptGameResourceObjectScriptedThingBase seh_Father;               // +0x7c
    CScriptGameResourceObjectScriptedThingBase seh_Victim;               // +0x8c
    CScriptGameResourceObjectScriptedThingBase seh_Brother;              // +0x9c
    CScriptGameResourceObjectScriptedThingBase seh_Ransommer;            // +0xac
};
CQ_RansomVictimScript::CQ_RansomVictimScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00DE0320
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_RansomVictimScript) == 0xBC);