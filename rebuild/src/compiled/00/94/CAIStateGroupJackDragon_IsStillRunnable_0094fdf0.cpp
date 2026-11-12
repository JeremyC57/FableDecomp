// CAIStateGroup_JackDragon::IsStillRunnable() -- the dragon fight continues while the enemy lives and nothing stops the group.
// Leaf: ?IsStillRunnable@CAIStateGroup_JackDragon@@AAE_NXZ
// Naming (agentNF31, 2026-11-12): access only, public -> private: Ego_r.pdb egor_signatures 0x009747B0 + FableWin.pdb
//   _fablewin_api 0x017C1E60 / 0x02BD9900 ?IsStillRunnable@CAIStateGroup_JackDragon@@AAE_NXZ; engine/CAIStateGroup_JackDragon.h
//   declares it private and the landed caller IntoHover 0x00951D60 binds this spelling.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094FDF0, length 55 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice, on PEnemy at +0x14), CAIStateGroupBase::
// IsStopping @ 0x008FD010.
//
// Shape: two early `return false` guards on PEnemy (null, then the dead bit +0x91 & 1 -- the landed
// IS_THING_DEAD tests, but as early returns so the false tail sits BETWEEN the guards and the final call), then
// `return IsStopping() ? false : true;` -- the ternary keeps the negation byte-sized (`neg al ; sbb al,al ;
// inc al`); `!IsStopping()` widens it to eax and the nested-if form puts the false tail last.
// The engine header keeps PEnemy as an opaque byte array, so the layout is modelled locally with its name.
#include "engine/CThing.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


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
protected:
    bool IsStopping() const;                          // 0x008FD010 (Ego_r.pdb protected const)
public:
};

struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CIntelligentPointer<CThing> PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
private:
    bool IsStillRunnable();
};

bool CAIStateGroup_JackDragon::IsStillRunnable()
{
    if (PEnemy.Get() == 0)
        return false;
    if (PEnemy.Get()->IsDead())
        return false;
    return IsStopping() ? false : true;
}