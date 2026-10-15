// CTCCreatureOpinionOfHero::IsWillingToAttackHeroOnOpinion() const -- hero enemies always; otherwise by the attitude to the player
// Grade: DIFFER(84v84) -- only the switch normalisation: retail `sub eax,2` (83 e8 02), the lane `add eax,-2` (83 c0 fe) for
// long, unsigned long, enum and inline-helper forms; both retail `sub` sites in the exe are in this opinion TU (0x0068180E,
// 0x0068186E) while the four other byte-indexed switches use `add` -- a compiler-build gap.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006817F0, length 59 bytes + embedded jump table (2 targets) and byte index
// table (16 entries) through 0x00681843. Caller 0x00864AE6.
// Callees: the world accessor @ 0x00686A80 (a `this`-ignoring member: [[0x013B8A1C]+0x24]), COpinionReactionManager::
// GetThingAttitudeToPlayer(const CThing*) @ 0x00701840 (CWorld +0x74).
// Shape: HeroOpinionEnemy (+0x342) short-circuits to the shared `mov al,1` exit. ParentThing is read into a local before
// the accessor call (esi) and the manager is a local too, so the call precedes the argument push. The switch over attitudes 2..17 lowers to `sub eax,2 ; cmp eax,0Fh ; ja` + a byte-indexed
// two-entry table: attitudes 2, 4, 5, 6, 12..17 refuse; 3, 7..11 and everything outside the range accept.
struct CThing;

struct COpinionReactionManager {
    long GetThingAttitudeToPlayer(const CThing* thing) const;       // 0x00701840
    float GetToleranceToBeingHit(const CThing* thing) const;        // 0x00701940
};

struct CWorld {
    unsigned char _pad00[0x10];
    void* DefinitionManager;                // +0x10
    unsigned char _pad14[0x74 - 0x14];
    COpinionReactionManager* POpinionReactionManager;   // +0x74 CCountedPointer<COpinionReactionManager> data
};

// Model: CTCCreatureOpinionOfHero with its PDB member names at the RETAIL offsets. engine/CTCCreatureOpinionOfHero.h (PDB,
// sizeof 0x65c) does not fit the bytes past +0x24: the retail STLport lists are 4 bytes (PDB 12), so PendingDeeds sits at
// +0x54, KnownDeeds[92] at +0x1c8, HeroOpinionEnemy at +0x342 and PUnknownThreatOpinionSource at +0x344 (as the landed
// SetHeroOpinionEnemy 0x006815D0 also shows), hence a local model.
struct CThing;

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

struct COpinionPersonalityTraits {          // 36 bytes
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

struct COpinionPersonalityDef {             // retail offsets (the def root is 4 bytes shorter than the PDB's)
    unsigned char _pad00[0x3c];
    COpinionPersonalityTraits* PersonalityTraits;   // +0x3c
    unsigned char _pad40[0x10];
    float ToleranceToBeingHit;              // +0x50
};

struct CDefPointeeBase;

struct CTCCreatureOpinionOfHero {
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
    CWorld* GetWorld() const;               // 0x00686A80
    bool IsWillingToAttackHeroOnOpinion() const;
};

bool CTCCreatureOpinionOfHero::IsWillingToAttackHeroOnOpinion() const
{
    if (HeroOpinionEnemy)
        return true;
    CThing* thing = ParentThing;
    COpinionReactionManager* manager = GetWorld()->POpinionReactionManager;
    switch (manager->GetThingAttitudeToPlayer(thing)) {
    case 2:
    case 4:
    case 5:
    case 6:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
        return false;
    default:
        return true;
    }
}
