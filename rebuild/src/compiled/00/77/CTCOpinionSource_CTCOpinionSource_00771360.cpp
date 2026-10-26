// CTCOpinionSource::CTCOpinionSource(CThing*) -- CTCBase(owner), vtable, empty def stack, every flag false, no opinion set, no virtual deed (ID -1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771360, length 91 bytes (vtable 0x0126A81C).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800.
// Retail CTCOpinionSource (PDB, the 16-byte DefStack vector is 12 bytes in retail): DefStack +0x0c, UseNavLineOfSight +0x18,
// WitnessesWillNotAttack +0x19, WitnessesWillOverrideFactionEnemy +0x1a, BypassesPersonality +0x1b, RelativeEffect +0x1c,
// RestrictToFactionAndAllies +0x20, RestrictToFactionIsReversed +0x24, ReactionFlags[79] +0x25 (left uninitialised),
// ReactToSelfPermitted +0x74, AlwaysAwareOf +0x75, UnknownThreatSource +0x76, ReactNonstopFlag +0x77, POpinionProduced
// (CCountedPointer, two words) +0x78, VirtualDeed +0x80, VirtualDeedID +0x84, FrameToClearVirtualDeed +0x88 (uninitialised).
// Shape: the vector's inline constructor comes first, so every store follows the vtable store, in declaration order,
// through one zero register.
class CThing;

struct CLongVector {                                // std::vector<long>
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    CLongVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
};

struct CFactionDefPointer {                         // CDefPointer<const CFactionDef>
    const void* m_pDef;
    CFactionDefPointer() : m_pDef(0) {}
};

struct COpinionSetPointer {                         // CCountedPointer<COpinionSet>
    void* m_pObject;
    void* m_pBlock;
    COpinionSetPointer() : m_pObject(0), m_pBlock(0) {}
};

struct CTCBase_00771360 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
    CTCBase_00771360(CThing* owner);                // 0x00686800
};

class CTCOpinionSource : public CTCBase_00771360 { public:
    CLongVector        DefStack;                    // +0x0c
    bool               UseNavLineOfSight;           // +0x18
    bool               WitnessesWillNotAttack;      // +0x19
    bool               WitnessesWillOverrideFactionEnemy;   // +0x1a
    bool               BypassesPersonality;         // +0x1b
    bool               RelativeEffect;              // +0x1c
    CFactionDefPointer RestrictToFactionAndAllies;  // +0x20
    bool               RestrictToFactionIsReversed; // +0x24
    bool               ReactionFlags[79];           // +0x25
    bool               ReactToSelfPermitted;        // +0x74
    bool               AlwaysAwareOf;               // +0x75
    bool               UnknownThreatSource;         // +0x76
    bool               ReactNonstopFlag;            // +0x77
    COpinionSetPointer POpinionProduced;            // +0x78
    long               VirtualDeed;                 // +0x80 EOpinionDeedType
    long               VirtualDeedID;               // +0x84
    unsigned long      FrameToClearVirtualDeed;     // +0x88
    CTCOpinionSource(CThing* owner);
    virtual void v00();
};

CTCOpinionSource::CTCOpinionSource(CThing* owner)
    : CTCBase_00771360(owner), UseNavLineOfSight(false), WitnessesWillNotAttack(false),
      WitnessesWillOverrideFactionEnemy(false), BypassesPersonality(false), RelativeEffect(false),
      RestrictToFactionIsReversed(false), ReactToSelfPermitted(false), AlwaysAwareOf(false),
      UnknownThreatSource(false), ReactNonstopFlag(false), VirtualDeed(0), VirtualDeedID(-1)
{
}