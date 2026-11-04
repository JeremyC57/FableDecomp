// CTCForcePushPower::~CTCForcePushPower() -- force-push spell component dtor (no user code): ~PItemRemoved, the def pointer's release, then ~CTCBase
// Leaf: ??1CTCForcePushPower@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A5460.
// Identity: RTTI vtable 0x0126C9BC (CTCForcePushPower); Ego_r.pdb PDef +0xc, PItemRemoved +0x14.
// Shape: empty user body; ~PItemRemoved, the def pointer release, ~CTCBase.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CSpecialAbilitiesForcePushDef;

class CTCForcePushPower : public CTCBase {
public:
    CTCForcePushPower(CThing&);
    virtual ~CTCForcePushPower();
    CDefPointer<const CSpecialAbilitiesForcePushDef> PDef;                 // +0x0c
    long                                     SpellLevel;                   // +0x10
    CIntelligentPointer<CThing>              PItemRemoved;                 // +0x14
    bool                                     Casted;                       // +0x1c
    long                                     CurTimeDelayBetweenCastsFrames; // +0x20
    bool                                     IgnoreFriendlies;             // +0x24
};
FABLE_STATIC_ASSERT(offsetof(CTCForcePushPower, PItemRemoved) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CTCForcePushPower) == 0x28);

CTCForcePushPower::~CTCForcePushPower()
{
}