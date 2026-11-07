#ifndef FABLE_ENGINE_CV_TALENTLESSBARDSCRIPT_H
#define FABLE_ENGINE_CV_TALENTLESSBARDSCRIPT_H
// Retail opt-in (chunk 845): NScript::CV_TalentlessBardScript, new header (no includer before chunk 845; the landed sibling rows
// keep their local models).
// Layout: Ego_r.pdb CV_TalentlessBardScript (struct_layouts_egor.tsv, 260 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48), a further - 4 after the STLport vector SongItems (0xC; Dinkumware 0x10): bHurtBard +0x48,
// bBard_finished +0x49, lNumber_of_things_sung_about +0x4c, bRewardGiven +0x50, stBard +0x54, BardIntroComplete +0x60 (the byte
// CTB_Bard::Main 0x00ED8680 reads and sets through ParentClass; the landed WatchForAttack 0x00ED9790 model has AttackedByHero
// +0x61), AttackedByHero +0x61, BardFollowing +0x62, BardWanderTime +0x64, SongItems +0x68, QuestInfo[12] +0x74 (8-byte QuestData
// name / tag), SongThingAvailable +0xd4 (vector<bool>, opaque 0x14: header debt), RandomSpeechTimer +0xe8; sizeof 0xec.
// Retail methods (Ego_r.pdb ?<Name>@CV_TalentlessBardScript@NScript@@QAEXVCScriptThing@@@Z): WatchForResetWanderCentre 0x00ED94D0,
// WatchForAttack 0x00ED9790 (the CSpawnedFunc1<CV_TalentlessBardScript,CScriptThing> threads CTB_Bard::Main spawns, vtable 0x012F37D0).
// Nested CTB_Bard (Ego_r.pdb 44 bytes: CEntityScriptBase 0x14, ParentClass +0x14, PMasterData +0x18, first_time_talked_to +0x1c,
// given_gold +0x1d, lastRandomSpeech1/2/3 +0x20/+0x24/+0x28; sizeof 0x2c); vtable 0x012F3498: 0 scalar deleting dtor 0x00ED7DB0,
// 1 Main 0x00ED8680, 2 Init 0x00ED7E00, 3 GetParentScript 0x00ED7DA0; GetRandomSpeech 0x00ED99D0 (Ego_r.pdb
// ?GetRandomSpeech@CTB_Bard@CV_TalentlessBardScript@NScript@@AAE?AVCCharString@@XZ).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
#include "stlport/fable_containers.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TalentlessBardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    struct QuestData {
        CCharString name;                                                // +0x00
        CCharString tag;                                                 // +0x04
    };
    void WatchForResetWanderCentre(CScriptThing);                        // 0x00ED94D0
    void WatchForAttack(CScriptThing);                                   // 0x00ED9790
    bool bHurtBard;                                                      // +0x48
    bool bBard_finished;                                                 // +0x49
    unsigned long lNumber_of_things_sung_about;                          // +0x4c
    bool bRewardGiven;                                                   // +0x50
    CScriptThing stBard;                                                 // +0x54
    bool BardIntroComplete;                                              // +0x60
    bool AttackedByHero;                                                 // +0x61
    bool BardFollowing;                                                  // +0x62
    CTimer BardWanderTime;                                               // +0x64
    std::vector<CCharString> SongItems;                                  // +0x68
    QuestData QuestInfo[12];                                             // +0x74
    unsigned char SongThingAvailable[0x14];                              // +0xd4 vector<bool> (opaque, header debt)
    CTimer RandomSpeechTimer;                                            // +0xe8
    class CTB_Bard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_TalentlessBardScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00ED7DA0
    private:
        virtual void Main();                                             // slot 1 0x00ED8680
        virtual void Init();                                             // slot 2 0x00ED7E00
        CCharString GetRandomSpeech();                                   // 0x00ED99D0
    protected:
        CV_TalentlessBardScript* ParentClass;                            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
        bool first_time_talked_to;                                       // +0x1c
        bool given_gold;                                                 // +0x1d
        CCharString lastRandomSpeech1;                                   // +0x20
        CCharString lastRandomSpeech2;                                   // +0x24
        CCharString lastRandomSpeech3;                                   // +0x28
    };
};
} // namespace NScript
FABLE_STATIC_ASSERT(sizeof(NScript::CV_TalentlessBardScript) == 0xec);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_TalentlessBardScript::CTB_Bard) == 0x2c);
#endif // FABLE_RETAIL_NSCRIPT
#endif
