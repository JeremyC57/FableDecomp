#ifndef FABLE_ENGINE_CTCQUESTCOMPLETIONUI_H
#define FABLE_ENGINE_CTCQUESTCOMPLETIONUI_H
// CTCQuestCompletionUI (new shared header, chunk 1010b, row 0x00896B60 CGameScriptInterface::KickOffQuestStartScreen): the hero's
// quest start / completion screen component. RTTI .?AVCTCQuestCompletionUI@@ : CTCBase : CBaseClassNonCopyable : CBaseClass
// (ghidra_out/rtti_bases.tsv); interface TCI_QUEST_COMPLETION_UI 252 (`cmp [eax],0FCh` 0x00896BA2; HasTC = bit 28 of the +0x3C
// word). Layout: Ego_r.pdb member order and types (struct_layouts_egor.tsv, 0xF0) with retail STLport container sizes (vector
// 0xC, list 4, map 0xC): Active +0x0C, Quitting +0x1C, QuittingFadeOut +0xC0 -- the three flags KickOffQuestStartScreen polls
// (0x00896C6D / 0x00896C74 / 0x00896C7B); size 0xD8 (the double LastRenderTime keeps 8-byte alignment). Members whose types
// need other headers stay opaque byte runs with the type in a comment.
// Methods (Ego_r.pdb / FableWin.pdb spellings): Activate(CompletionType, const CCharString&, bool, const CWideString&, bool)
// out of line at 0x005E4740, SaveInitialValues(const CCharString&) 0x005E4860; the FableWin-only (inline in the release build)
// accessors IsActive / IsQuitting / IsQuittingFadeOut read the flags (bodies per the bytes above). The CompletionType
// enumerators are not in either PDB name set (0 = the quest start screen: KickOffQuestStartScreen passes 0).
// Needs FABLE_RETAIL_CTCBASE (engine/CTCBase.h).
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
class CCharString;
class CWideString;
class CTCQuestCompletionUI : public CTCBase {
public:
#if defined(_MSC_VER)
    enum CompletionType;
#else
    enum CompletionType { };
#endif
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_QUEST_COMPLETION_UI; }
    void Activate(CompletionType completion, const CCharString& scriptName, bool b1, const CWideString& summary, bool b2);   // 0x005E4740
    void SaveInitialValues(const CCharString& scriptName);                                                                    // 0x005E4860
    bool IsActive() const { return Active; }                             // FableWin ?IsActive@CTCQuestCompletionUI@@QBE_NXZ
    bool IsQuitting() const { return Quitting; }                         // FableWin ?IsQuitting@CTCQuestCompletionUI@@QBE_NXZ
    bool IsQuittingFadeOut() const { return QuittingFadeOut; }           // FableWin ?IsQuittingFadeOut@CTCQuestCompletionUI@@QBE_NXZ
private:
    bool          Active;                                                // +0x0c
    bool          ReallyActive;                                          // +0x0d
    unsigned char _pad_0x0e[0x2];                                        // +0x0e
    unsigned char Components[0xc];                                       // +0x10 std::vector<NUISystem::CComponent*>
    bool          Quitting;                                              // +0x1c
    bool          WaitingForRegionLoad;                                  // +0x1d
    bool          WaitingForPlayerControl;                               // +0x1e
    bool          WaitingForBoastProcessing;                             // +0x1f
    long          Completion;                                            // +0x20 CompletionType
    unsigned long Frame;                                                 // +0x24
    long          FrameStartedBoastProcessing;                           // +0x28
    unsigned char PCardDef[0x4];                                         // +0x2c CDefPointer<const CQuestCardDef>
    unsigned char Boasts[0x4];                                           // +0x30 std::list<CQuestManager::CBoast>
    unsigned long RenderedObjectDef;                                     // +0x34
    unsigned char QuestSummary[0x4];                                     // +0x38 CWideString
    unsigned char SuccessSummary[0x4];                                   // +0x3c CWideString
    unsigned char QuestObjective[0x4];                                   // +0x40 CWideString
    unsigned char _pad_0x44[0x4];                                        // +0x44
    double        LastRenderTime;                                        // +0x48
    unsigned char ScriptName[0x4];                                       // +0x50 CCharString
    unsigned char FailureReason[0x4];                                    // +0x54 CWideString
    long          GoldReward;                                            // +0x58
    long          RenownReward;                                          // +0x5c
    long          TargetGoldGained;                                      // +0x60
    long          InitialGold;                                           // +0x64
    long          TargetExperienceGained;                                // +0x68
    long          InitialExperience;                                     // +0x6c
    long          TargetRenownGained;                                    // +0x70
    long          InitialRenown;                                         // +0x74
    float         ExperienceGainedDisplayed;                             // +0x78
    float         RenownGainedDisplayed;                                 // +0x7c
    float         GoldGainedDisplayed;                                   // +0x80
    unsigned char StartQuestExperience[0xc];                             // +0x84 std::map<long,unsigned long>
    unsigned char StartQuestGold[0xc];                                   // +0x90 std::map<long,unsigned long>
    unsigned char StartQuestRenown[0xc];                                 // +0x9c std::map<long,unsigned long>
    void*         PRenownText;                                           // +0xa8 CText*
    void*         PGoldText;                                             // +0xac CText*
    void*         PExperienceText;                                       // +0xb0 CText*
    float         AccumFadeTime;                                         // +0xb4
    void*         PFrame;                                                // +0xb8 NUISystem::CComponent*
    void*         PBlack;                                                // +0xbc NUISystem::CComponent*
    bool          QuittingFadeOut;                                       // +0xc0
    long          QuestCardDef;                                          // +0xc4
    unsigned long ExperienceCountingSound;                               // +0xc8
    unsigned long GoldCountingSound;                                     // +0xcc
    unsigned long RenownCountingSound;                                   // +0xd0
    bool          CanTeleportToGuildMember;                              // +0xd4
};
FABLE_STATIC_ASSERT(sizeof(CTCQuestCompletionUI) == 0xd8);
#endif // FABLE_ENGINE_CTCQUESTCOMPLETIONUI_H
