// CTCCreatureOpinionOfHero::IsWillingToContinueFollowingHero() const -- decided by the creature's attitude to the player
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006818F0, length 46 bytes + embedded jump table (2 targets) and byte index
// table (17 entries) through 0x00681938. Callers 0x006D4FA3, 0x006D5499.
// Callees: the world accessor @ 0x00686A80, COpinionReactionManager::GetThingAttitudeToPlayer(const CThing*) @ 0x00701840.
// Shape: the switch over attitudes 0..16 is `cmp eax,10h ; ja` + a byte-indexed two-entry table: attitudes 0, 1, 2, 5, 6,
// 9..16 keep following; 3, 4, 7, 8 and anything above 16 stop (the default). ParentThing is read into a local before the
// accessor call and the manager is a local too, so the call precedes the argument push.
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

class CTCBase { protected:
    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};
class CTCCreatureOpinionOfHero : public CTCBase { public:
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
    bool IsWillingToContinueFollowingHero() const;
};

bool CTCCreatureOpinionOfHero::IsWillingToContinueFollowingHero() const
{
    CThing* thing = ParentThing;
    COpinionReactionManager* manager = PeekWorld().POpinionReactionManager;
    switch (manager->GetThingAttitudeToPlayer(thing)) {
    case 0:
    case 1:
    case 2:
    case 5:
    case 6:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        return true;
    default:
        return false;
    }
}