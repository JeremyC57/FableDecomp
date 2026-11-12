#ifndef FABLE_ENGINE_CTCTROPHY_H
#define FABLE_ENGINE_CTCTROPHY_H
// New shared header (w22 agent2007, rows 0x006DE8C0 CTCTrophy::OnShowTrophyPerformAction / 0x006DEEE0 CTCTrophy::GlobalUpdate): the
// trophy thing component (the hero shows a trophy to gather witnesses in the trophy minigame). RTTI .?AVCTCTrophy@@ : CTCBase
// (ghidra_out/rtti_bases.tsv); vtable 0x0125FC54 (rebuild/manifest/rtti-vtables.tsv): slot 0 ??_G 0x006DFB40, 1 OnSerialise 0x006DDBF0,
// 4 OnCreate 0x006DDA20, 8 HasFrameUpdate 0x006DFA40, 9 FrameUpdate 0x006DF480, 21 GetInterfaceType 0x006DFA10 (TCI 0x39),
// 22 GetClassTypeName 0x006DFA20, 23 IsComponentSerialisable 0x006DD750, 26 GlobalUpdate 0x006DEEE0.
// Methods and access: the Ego_r.pdb / FableWin.pdb `@CTCTrophy@@` set (private AAE: SendPauseEvent, StartDialogBox, GameDebugText,
// PlaySoundW, StartMinigame, TerminateMinigame, UpdateGainedWitnessSounds; public QAE: OnShowTrophyFinishAction, EndMinigame,
// OnTrophyUseAction, OnShowTrophyInitialise, OnShowTrophyPerformAction). InstructionsText: the public static CCharString array
// ?InstructionsText@CTCTrophy@@2PAVCCharString@@A (0x013BAB84; GlobalUpdate indexes it by CurrentInstructionsPage < 2).
// Members: Ego_r.pdb names in order at the retail offsets (the two STLport lists are 4 bytes, Ego_r 12): PDef +0x0C .. FrameForNext-
// GainedWitnessSound +0x30 (PDB offsets hold), AvailableWitnesses +0x34, WitnessesAchieved +0x38, WaitingForConfirmation +0x3C ..
// IsGlobalTC +0x40, CurrentInstructionsPage +0x44, Mountable +0x48; sizeof 0x4C (the landed FrameUpdate / ctor 0x006DDAE0 / dtor
// 0x006DDB80 use these offsets). Needs FABLE_RETAIL_CTCBASE, FABLE_RETAIL_DEF_CLASS_BASE (CDefPointer), FABLE_RETAIL_CINTELLIGENTPOINTER
// and stlport/fable_containers.h.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
class CThing;
class CThingAICreature;
class CThingCreatureBase;
class CTrophyDef;
class CPersistContext;

class CTCTrophy : public CTCBase {
public:
    CTCTrophy(CThing&);                                                  // 0x006DDAE0
    virtual ~CTCTrophy();                                                // 0x006DDB80
    static CTCBase* __fastcall Alloc(CThing&);
    virtual void OnSerialise(CPersistContext&, bool);                    //  1 0x006DDBF0
    virtual void OnCreate();                                             //  4 0x006DDA20
    virtual void FrameUpdate();                                          //  9 0x006DF480
    virtual ETCInterfaceType GetInterfaceType() const;                   // 21 0x006DFA10
    virtual CCharString GetClassTypeName() const;                        // 22 0x006DFA20
    virtual bool IsComponentSerialisable() const;                        // 23 0x006DD750
    virtual void GlobalUpdate(bool);                                     // 26 0x006DEEE0
    void OnShowTrophyInitialise(CThing&);                                // 0x006DE8B0
    void OnShowTrophyPerformAction(CThing&);                             // 0x006DE8C0
    void OnShowTrophyFinishAction(CThing&);
    void OnTrophyUseAction(CThingCreatureBase&);
    void EndMinigame();                                                  // 0x006DDA50
    static CCharString InstructionsText[];                               // 0x013BAB84
private:
    void SendPauseEvent(bool);                                           // 0x006DD760
    void StartDialogBox(CWideString, unsigned long, CWideString, CWideString, CWideString);   // 0x006DD7E0
    void GameDebugText(const CCharString&, float);
    unsigned long PlaySoundW(const CCharString&);                        // 0x006DDCD0
    void StartMinigame();                                                // 0x006DE080
    void TerminateMinigame();                                            // 0x006DE280
    void UpdateGainedWitnessSounds();                                    // 0x006DEE80
public:
    CDefPointer<const CTrophyDef> PDef;                                  // +0x0c
    long  NonMinigameDeedHandle;                                         // +0x10
    bool  MinigameInProgress;                                            // +0x14
    long  BestWitnessesAheadToDate;                                      // +0x18
    long  MinigameInitialFramesLeft;                                     // +0x1c
    long  MinigameCurrentFramesLeft;                                     // +0x20
    long  MinigameStartingWitnessCount;                                  // +0x24
    long  NumberOfWitnessesLeft;                                         // +0x28
    long  PendingGainedWitnessSounds;                                    // +0x2c
    long  FrameForNextGainedWitnessSound;                                // +0x30
    std::list<CIntelligentPointer<CThingAICreature> > AvailableWitnesses;   // +0x34
    std::list<CIntelligentPointer<CThingAICreature> > WitnessesAchieved;    // +0x38
    bool  WaitingForConfirmation;                                        // +0x3c
    bool  WaitingForScore;                                               // +0x3d
    bool  WaitingForTerminate;                                           // +0x3e
    bool  WaitingForNextInstructionsPage;                                // +0x3f
    bool  IsGlobalTC;                                                    // +0x40
    long  CurrentInstructionsPage;                                       // +0x44
    bool  Mountable;                                                     // +0x48
};
FABLE_STATIC_ASSERT(offsetof(CTCTrophy, PDef) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CTCTrophy, PendingGainedWitnessSounds) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCTrophy, AvailableWitnesses) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCTrophy, WitnessesAchieved) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCTrophy, WaitingForConfirmation) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCTrophy, CurrentInstructionsPage) == 0x44);
FABLE_STATIC_ASSERT(sizeof(CTCTrophy) == 0x4c);
#endif
