// NScript::CQ_OpeningGraveyardSecretPassageScript::CQ_OpeningGraveyardSecretPassageScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base and interface/data pair, vtable 0x012DA11C, the four hero_object strings
// Leaf: ??0CQ_OpeningGraveyardSecretPassageScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC2570, length 59 bytes (+ int3 padding), `ret 8`.
// Identity: its only caller is the factory 0x00DCBFD0 (`push 74h ; call operator new`), registered at 0x00CD7156 under
//   "S_QOGSP"; the vtable 0x012DA11C holds Init 0x00DC25B0 and Main 0x00DC26B0 (manifest "CQ_SecretPassageScript::Init" /
//   "CGlobal_WatchForHeroDeathScript::Main" are propagated). Ego_r.pdb CQ_OpeningGraveyardSecretPassageScript is 0x88 bytes,
//   retail 0x74 = PDB - 0x14 like every quest script (the script base is 0x14 bytes smaller in retail).
// Label correction: the manifest's `_Pair_base<CCharString,C3DDummyObject>` is propagated.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CCharString::CCharString() @ 0x0099E4B0 (x4, hero_object).
// Layout (retail = PDB - 0x14): Game +0x40, Data +0x44 (the landed argument-carrying intermediate base), bools +0x48..+0x50,
//   SkeletonStuff[4] +0x51, MissionSucceeded/MissionFailed/DoorClosed +0x55..+0x57, hero_object[4] +0x58,
//   LieutenantAround +0x68, FriendliesKilled +0x6c, VisitorsAttacked +0x70.
// Shape: as the landed quest-script ctors (CQ_HangingTreeEvilScript 0x00D68F00): the two arguments are stored by the inlined
//   `__declspec(novtable)` intermediate base before the derived vtable store; the CCharString array has a constructor and no
//   destructor in this TU, so it is built with an inline counted loop (`mov ebx,4 ; ... ; dec ebx ; jne`); the plain bools and
//   the long are left uninitialised.
class CGameScriptInterface;

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base (landed model)
    CGameScriptInterface* Game;                         // +0x40 (second argument)
    void*                 Data;                         // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};

class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
};

namespace NScript {
class CQ_OpeningGraveyardSecretPassageScript : public CQuestScriptObject { public:   // retail 0x74 bytes, vtable 0x012DA11C
    bool finished_quest;                                // +0x48
    bool DoorChange;                                    // +0x49
    bool ZombieCamPlayed;                               // +0x4a
    bool skeleton_visited;                              // +0x4b
    bool doors_open;                                    // +0x4c
    bool GravekeeperIntroduced;                         // +0x4d
    bool GravekeeperCutsceneStart;                      // +0x4e
    bool UndeadCutscenePlayed;                          // +0x4f
    bool UndeadCutsceneStarted;                         // +0x50
    bool SkeletonStuff[4];                              // +0x51
    bool MissionSucceeded;                              // +0x55
    bool MissionFailed;                                 // +0x56
    bool DoorClosed;                                    // +0x57
    CCharString hero_object[4];                         // +0x58
    bool LieutenantAround;                              // +0x68
    long FriendliesKilled;                              // +0x6c
    bool VisitorsAttacked;                              // +0x70
    virtual ~CQ_OpeningGraveyardSecretPassageScript();
    CQ_OpeningGraveyardSecretPassageScript(void* data, CGameScriptInterface* game);
};

CQ_OpeningGraveyardSecretPassageScript::CQ_OpeningGraveyardSecretPassageScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
}