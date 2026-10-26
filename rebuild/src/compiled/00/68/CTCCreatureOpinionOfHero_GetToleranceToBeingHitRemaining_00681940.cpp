// CTCCreatureOpinionOfHero::GetToleranceToBeingHitRemaining() const -- the override when set (>= 0), else the reaction manager's
// tolerance for this thing plus the personality's ToleranceToBeingHit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681940, length 59 bytes (+5 int3).
// Callees: the world accessor @ 0x00686A80 (called with this in ecx), COpinionReactionManager::GetToleranceToBeingHit
// (const CThing*) @ 0x00701940 (float). Constant 0.0f @ 0x0122DEDC.
// Shape: `override >= 0.0f` is `fcomp ; test ah,1 ; jne` with the override re-loaded for the return; ParentThing is read
// into a local before the accessor call (edi) and the manager is a local (the call precedes the push); the personality def (+0x20) is added only when present (`fadd [esi+50h]`).
class CThing;

class COpinionReactionManager { public:
    long GetThingAttitudeToPlayer(const CThing* thing) const;       // 0x00701840
    float GetToleranceToBeingHit(const CThing* thing) const;        // 0x00701940
};

class CWorld { public:
    unsigned char _pad00[0x10];
    void* DefinitionManager;                // +0x10
    unsigned char _pad14[0x74 - 0x14];
    COpinionReactionManager* POpinionReactionManager;   // +0x74 CCountedPointer<COpinionReactionManager> data
};

// Model: CTCCreatureOpinionOfHero with its PDB member names at the RETAIL offsets. engine/CTCCreatureOpinionOfHero.h (PDB,
// sizeof 0x65c) does not fit the bytes past +0x24: the retail STLport lists are 4 bytes (PDB 12), so PendingDeeds sits at
// +0x54, KnownDeeds[92] at +0x1c8, HeroOpinionEnemy at +0x342 and PUnknownThreatOpinionSource at +0x344 (as the landed
// SetHeroOpinionEnemy 0x006815D0 also shows), hence a local model.
class CThing;

struct CBaseIntelligentPointer {
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
};

struct SLongNode {                          // STLport _List_node<long>
    SLongNode* Next;                        // +0x00
    SLongNode* Prev;                        // +0x04
    long Data;                              // +0x08
};

struct CLongList {                          // STLport list<long> (4 bytes)
    SLongNode* Header;                      // +0x00
};

class COpinionPersonalityTraits { public:          // 36 bytes
    float BaseOffset;                       // +0x00
    float HeroStatScaling;                  // +0x04
    float VillageOpinionScaling;            // +0x08
    float TimeScalingRunIn;                 // +0x0c
    float TimeScalingRunOut;                // +0x10
    float PeakScalingIfPos;                 // +0x14
    float PeakScalingIfNeg;                 // +0x18
    float PersistScalingIfPos;              // +0x1c
    float PersistScalingIfNeg;              // +0x20
};

class COpinionPersonalityDef { public:             // retail offsets (the def root is 4 bytes shorter than the PDB's)
    unsigned char _pad00[0x3c];
    COpinionPersonalityTraits* PersonalityTraits;   // +0x3c
    unsigned char _pad40[0x10];
    float ToleranceToBeingHit;              // +0x50
};

class CDefPointeeBase;

class CTCCreatureOpinionOfHero { public:
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    long LastOpinionReactionFrame;          // +0x0c
    float NumberOfTimesHit;                 // +0x10
    float ToleranceToBeingHitOverride;      // +0x14
    long FrameToDecayNumberOfTimesHit;      // +0x18
    CDefPointeeBase* PDef;                  // +0x1c CDefPointer<COpinionOfHeroDef const>
    COpinionPersonalityDef* PPersonalityDef;        // +0x20 CDefPointer<COpinionPersonalityDef const>
    unsigned char Masks[0x30];              // +0x24 AttitudeMask, ReactionMask, DeedMask, RelativeOpinion (retail sizes)
    CLongList PendingDeeds;                 // +0x54
    long LastFrameAppliedDeedEffect[92];    // +0x58
    CLongList KnownDeeds[92];               // +0x1c8
    CLongList TransientOffsets;             // +0x338
    long ForcedAttitude;                    // +0x33c
    bool InteractedFlag;                    // +0x340
    bool GreetedFlag;                       // +0x341
    bool HeroOpinionEnemy;                  // +0x342
    bool InFindAGuardAndYellForHelp;        // +0x343
    CBaseIntelligentPointer PUnknownThreatOpinionSource;   // +0x344
    long FrameLastUpdatedUnknownThreat;     // +0x34c
    CWorld* GetWorld_00686a80() const;               // 0x00686A80
    float GetToleranceToBeingHitRemaining() const;
};

float CTCCreatureOpinionOfHero::GetToleranceToBeingHitRemaining() const
{
    if (ToleranceToBeingHitOverride >= 0.0f)
        return ToleranceToBeingHitOverride;
    CThing* thing = ParentThing;
    COpinionReactionManager* manager = GetWorld_00686a80()->POpinionReactionManager;
    float tolerance = manager->GetToleranceToBeingHit(thing);
    if (PPersonalityDef)
        tolerance += PPersonalityDef->ToleranceToBeingHit;
    return tolerance;
}