// CTCCreatureOpinionOfHero::OnKill() -- kill the unknown-threat opinion source thing if it is still alive
// Leaf: OnKill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681B30, length 52 bytes. CTCCreatureOpinionOfHero vtable 0x0125BC3C slot 14
// (the CTCBase OnKill slot).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on PUnknownThreatOpinionSource, +0x344),
// CThing::Kill(bool) @ 0x004C9B80.
// Shape: the landed IS_THING_ALIVE idiom (0x008CAD10): the member address held in esi, GetPItem re-fetched for the null
// test, the Dead bit (+0x91 bit 0) and the Kill target; the `push 1` precedes the third GetPItem (arguments first).
// Model: CTCCreatureOpinionOfHero with its PDB member names at the RETAIL offsets. engine/CTCCreatureOpinionOfHero.h (PDB,
// sizeof 0x65c) does not fit the bytes past +0x24: the retail STLport lists are 4 bytes (PDB 12), so PendingDeeds sits at
// +0x54, KnownDeeds[92] at +0x1c8, HeroOpinionEnemy at +0x342 and PUnknownThreatOpinionSource at +0x344 (as the landed
// SetHeroOpinionEnemy 0x006815D0 also shows), hence a local model.
struct CThing {                             // only the dead bit and the call are modelled
    unsigned char _pad[0x91];
    unsigned char Dead : 1;                 // +0x91 bit 0
    void Kill(bool checkLocked);            // 0x004C9B80
};

struct CBaseIntelligentPointer {
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

// IS_THING_ALIVE(pointer): the item is set and its Dead bit is clear (GetPItem re-fetched, as the landed idiom 0x008CAD10).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->Dead)
            return true;
    }
    return false;
}

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
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
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
    virtual void OnKill();
};

void CTCCreatureOpinionOfHero::OnKill()
{
    if (IS_THING_ALIVE(PUnknownThreatOpinionSource))
        PUnknownThreatOpinionSource.GetPItem()->Kill(true);
}
