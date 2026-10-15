// COpinionSourceDef::operator=(const COpinionSourceDef&) -- memberwise copy: CParentDefClassBase part, the produced-opinion map, five awareness flags, the reaction-flag map, five witness flags, the faction restriction, the opinion set and the 79 per-reaction flags; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045EE9B, length 146 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (`push 4Fh ; pop ecx` loop count).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, map<EOpinion,float>::operator= @ 0x0045F03A,
//          map<EOpinionReactionType,bool>::operator= @ 0x0045DD2C, COpinionSet (vector<float>) operator= @ 0x0045EF2D.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// COpinionSourceDef's vtable 0x01233FF4 is the 5-byte `jmp 0x0045EE9B` right before this body; PDB members
// +0x40..+0x88 sit at retail +0x3c..+0x78 (retail base 0x39 bytes, containers 12 bytes instead of 16).
// Shape: memberwise assignment in declaration order; the bool[79] array is copied by the element loop the
// compiler writes for an array member (`sub edi,esi` turns the source into a this-relative offset).
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct COpinionMap {                    // retail map<EOpinion,float> (12 bytes)
    void* m_allocator; void* m_head; unsigned long m_size;
    COpinionMap& operator=(const COpinionMap& other);       // 0x0045F03A
};

struct CReactionFlagMap {               // retail map<EOpinionReactionType,bool> (12 bytes)
    void* m_allocator; void* m_head; unsigned long m_size;
    CReactionFlagMap& operator=(const CReactionFlagMap& other);   // 0x0045DD2C
};

struct COpinionSet {                    // retail 12 bytes: a vector<float> of opinion values
    float* m_first; float* m_last; float* m_end;
    COpinionSet& operator=(const COpinionSet& other);       // 0x0045EF2D
};

struct COpinionSourceDef : CParentDefClassBase {    // vtable 0x01233FF4
    COpinionMap ProducedOpinion;                // +0x3c
    bool AlwaysAwareOf;                         // +0x48
    bool UnknownThreatSource;                   // +0x49
    bool ReactNonstop;                          // +0x4a
    bool ReactToSelfPermitted;                  // +0x4b
    bool ReactionFlagDefault;                   // +0x4c
    CReactionFlagMap ReactionFlag;              // +0x50
    bool UseNavLineOfSight;                     // +0x5c
    bool WitnessesWillNotAttack;                // +0x5d
    bool WitnessesWillOverrideFactionEnemy;     // +0x5e
    bool BypassesPersonality;                   // +0x5f
    bool RelativeEffect;                        // +0x60
    long RestrictToFactionAndAllies;            // +0x64
    bool RestrictToFactionIsReversed;           // +0x68
    COpinionSet OpinionProduced;                // +0x6c
    bool ReactionFlags[79];                     // +0x78
    COpinionSourceDef& operator=(const COpinionSourceDef& other);
};

COpinionSourceDef& COpinionSourceDef::operator=(const COpinionSourceDef& other)
{
    CParentDefClassBase::operator=(other);
    ProducedOpinion = other.ProducedOpinion;
    AlwaysAwareOf = other.AlwaysAwareOf;
    UnknownThreatSource = other.UnknownThreatSource;
    ReactNonstop = other.ReactNonstop;
    ReactToSelfPermitted = other.ReactToSelfPermitted;
    ReactionFlagDefault = other.ReactionFlagDefault;
    ReactionFlag = other.ReactionFlag;
    UseNavLineOfSight = other.UseNavLineOfSight;
    WitnessesWillNotAttack = other.WitnessesWillNotAttack;
    WitnessesWillOverrideFactionEnemy = other.WitnessesWillOverrideFactionEnemy;
    BypassesPersonality = other.BypassesPersonality;
    RelativeEffect = other.RelativeEffect;
    RestrictToFactionAndAllies = other.RestrictToFactionAndAllies;
    RestrictToFactionIsReversed = other.RestrictToFactionIsReversed;
    OpinionProduced = other.OpinionProduced;
    for (long i = 0; i < 79; ++i)
        ReactionFlags[i] = other.ReactionFlags[i];
    return *this;
}