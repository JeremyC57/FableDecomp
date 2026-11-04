// CTCDExperienceOrb::~CTCDExperienceOrb() -- experience-orb driver dtor: reset the experience def pointer, then the member def pointer and ~CTCBase
// Leaf: ??1CTCDExperienceOrb@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00779CD0.
// Identity: Ego_r.pdb ??1CTCDExperienceOrb@@UAE@XZ; FableWin.pdb ?Reset@?$CDefPointer@$$CBVCExperienceDef@@@@ shows the user body
//   resets the def pointer (Reset(0)); overlay FABLE_RETAIL_CDEFPOINTER_RESET gives CDefPointer::Reset its inline body.
// Shape: `PDef.Reset(0);` then the member def pointer and ~CTCDriverBase (5 grades: Release+zero also matched, three variants differed).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCDriverBase.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RESET
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"

class CExperienceDef : public CSubDefClassBase {             // RTTI CExperienceDef : CSubDefClassBase : CDefClassBase
};

class CTCDExperienceOrb : public CTCDriverBase {
public:
    CTCDExperienceOrb(CThing&);
    virtual ~CTCDExperienceOrb();
    CDefPointer<const CExperienceDef> PDef;                                // +0x0c
    long                              ExperiencePoints;                    // +0x10
    long                              CombinationImmunityTimer;            // +0x14
    long                              LifeTimer;                           // +0x18
    bool                              DecayOverTime;                       // +0x1c
    bool                              PickedUp;                            // +0x1d
};
FABLE_STATIC_ASSERT(sizeof(CTCDExperienceOrb) == 0x20);

CTCDExperienceOrb::~CTCDExperienceOrb()
{
    PDef.Reset(0);
}