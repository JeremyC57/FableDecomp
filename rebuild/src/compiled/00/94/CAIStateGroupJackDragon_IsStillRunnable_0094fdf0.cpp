// CAIStateGroup_JackDragon::IsStillRunnable() -- the dragon fight continues while the enemy lives and nothing stops the group.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094FDF0, length 55 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice, on PEnemy at +0x14), CAIStateGroupBase::
// IsStopping @ 0x008FD010.
//
// Shape: two early `return false` guards on PEnemy (null, then the dead bit +0x91 & 1 -- the landed
// IS_THING_DEAD tests, but as early returns so the false tail sits BETWEEN the guards and the final call), then
// `return IsStopping() ? false : true;` -- the ternary keeps the negation byte-sized (`neg al ; sbb al,al ;
// inc al`); `!IsStopping()` widens it to eax and the nested-if form puts the false tail last.
// The engine header keeps PEnemy as an opaque byte array, so the layout is modelled locally with its name.
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem();                         // 0x00A01B50
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    bool IsStopping();                          // 0x008FD010
};

struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool IsStillRunnable();
};

bool CAIStateGroup_JackDragon::IsStillRunnable()
{
    if (PEnemy.GetPItem() == 0)
        return false;
    if (PEnemy.GetPItem()->IsDead())
        return false;
    return IsStopping() ? false : true;
}