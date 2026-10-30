// NScript::CHeroBoastsScript::BoastRansomVictimKidnappersKilled(CCharString quest) -- boast thread: until the script ends, wait on quest `quest`; a failed quest fails boast 0x27, a completed one completes it when the master flag RansomVictimKidnappersKilled is set and fails it otherwise (each outcome after a final termination check), else a script frame
// Leaf: ?BoastRansomVictimKidnappersKilled@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE68A0, 196 bytes (208 with int3 padding), thiscall `ret 4` (the quest name by
//   value, destroyed on every exit).
// Identity (proven): NScript::CHeroBoastsScript::BoastRansomVictimKidnappersKilled; evidence: CHeroBoastsScript's spawn of this body (0x00CE3A95:
//   `mov [edi+34h], 0x00CE68A0` in the CSpawnedFunc2 construction) passes the literal "BoastRansomVictimKidnappersKilled"; Ego_r.pdb has
//   ?BoastRansomVictimKidnappersKilled@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z, and the master flag the body reads (+0x12B) is
//   Ego_r.pdb NScript::CQ_SunnyvaleMasterData::RansomVictimKidnappersKilled (offsets identical in retail).
// Label: the manifest's "WatchForRegionLoads" is propagated across family H034 (seven boast threads).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940 (Ego_r.pdb ?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ),
//   CCharString::~CCharString 0x0099EAE0, and the script interface virtuals (retail vtable 0x01260F0C, slot names from
//   ghidra_out/cgsi_slots_aligned.tsv): NewScriptFrame (7), MsgOnQuestCompletedBeforeScreenShown (28),
//   MsgOnQuestFailedBeforeScreenShown (29), SetBoastAsFailed (305), SetBoastAsCompleted (306).
// Layout: Ego_r.pdb CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {CGameScriptInterfaceBase* Game,
//   CQ_SunnyvaleMasterData* MasterScriptData} after CScriptBase, retail +0x40 / +0x44 (CScriptBase is 0x40 bytes in retail:
//   the PActiveThread IsActiveThreadTerminating reads is +0x2C). Local models -- Review at landing.
// Shape: completed-and-flag is one condition, so the completed query is made a second time on the fall-through; the two
//   "fail the boast" exits share one block (retail tail-merges them).
class CCharString {
public:
    void* PData;
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};

// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"

class CScriptBase {
public:
    unsigned char _pad00[0x40];                         // vptr, Threads, EntityScripts, EntityScriptBindings, LastWorldUpdateFrame,
                                                        // PActiveThread (+0x2C), RuntimeScriptedEntityPersist
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

namespace NScript {
class CQ_SunnyvaleMasterData {
public:
    unsigned char _pad000[0x12B];
    bool          RansomVictimKidnappersKilled;                             // +0x12B
};
}

template <class TMasterData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TMasterData*              MasterScriptData;         // +0x44
};

namespace NScript {
class CHeroBoastsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void BoastRansomVictimKidnappersKilled(CCharString quest);
};
}

void NScript::CHeroBoastsScript::BoastRansomVictimKidnappersKilled(CCharString quest)
{
    while (!IsActiveThreadTerminating()) {
        if (Game->MsgOnQuestFailedBeforeScreenShown(quest)) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetBoastAsFailed(0x27, quest);
            return;
        }
        if (Game->MsgOnQuestCompletedBeforeScreenShown(quest) && MasterScriptData->RansomVictimKidnappersKilled) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetBoastAsCompleted(0x27, quest);
            return;
        }
        if (Game->MsgOnQuestCompletedBeforeScreenShown(quest)) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetBoastAsFailed(0x27, quest);
            return;
        }
        Game->NewScriptFrame();
    }
}