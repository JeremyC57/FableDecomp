// CTCCreatureOpinionOfHero::TransformForcedOpinion(long opinionType, float value) -- apply the personality's offset/scale for that opinion
// Grade: DIFFER(40v43) -- retail tests the personality def through eax (`mov eax,[ecx+20h] ; test eax,eax`) and re-reads it
// into ecx for the computation (traits in edx); the lane CSEs the two reads into ecx (plain member, inline def member,
// early-return and CDefPointer IsNull/operator-> forms all identical).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006817C0, length 42 bytes (+6 int3). One of two byte-identical bodies (0x00681790,
// 0x006817C0; callers 0x00700B75, 0x00700D67).
// Shape: with no personality def the value is returned unchanged; otherwise the def's PersonalityTraits array (+0x3c,
// 36-byte COpinionPersonalityTraits records: `lea eax,[eax+eax*8]` then *4) gives BaseOffset + value * HeroStatScaling.
// The def pointer is tested and then re-read for the computation.
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
    float TransformOpinion(long opinionType, float value) const
    {
        const COpinionPersonalityTraits& traits = PersonalityTraits[opinionType];
        return traits.BaseOffset + value * traits.HeroStatScaling;
    }
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
    float TransformForcedOpinion(long opinionType, float value) const;
};

float CTCCreatureOpinionOfHero::TransformForcedOpinion(long opinionType, float value) const
{
    if (PPersonalityDef)
        return PPersonalityDef->TransformOpinion(opinionType, value);
    return value;
}
