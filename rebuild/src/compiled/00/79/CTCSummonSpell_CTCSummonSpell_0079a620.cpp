// CTCSummonSpell::~CTCSummonSpell() -- summon-spell component dtor (no user code): ~PItemRemoved, ~Minion, the def pointer's release, then ~CTCBase
// Leaf: ??1CTCSummonSpell@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079A620.
// Identity: RTTI vtable 0x0126C2B4 (CTCSummonSpell); Ego_r.pdb members PDef CDefPointer +0xc, Minion +0x11, PItemRemoved +0x2c.
// Shape: empty user body: ~PItemRemoved, ~Minion (CIntelligentPointer), the def pointer release, ~CTCBase.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CThingAICreature;
class CSpecialAbilitiesSummonSpellDef;

class CTCSummonSpell : public CTCBase {
public:
    CTCSummonSpell(CThing&);
    virtual ~CTCSummonSpell();
    CDefPointer<const CSpecialAbilitiesSummonSpellDef> PDef;              // +0x0c
    bool                                     JustReleased;                 // +0x10
    CIntelligentPointer<CThingAICreature>    Minion;                       // +0x11
    long                                     FadeInTimer;                  // +0x1c
    long                                     CurFadeInTimer;               // +0x20
    long                                     SpellLevel;                   // +0x24
    long                                     CurTimer;                     // +0x28
    CIntelligentPointer<CThing>              PItemRemoved;                 // +0x2c
    bool                                     CutSceneCancelled;            // +0x34
};
FABLE_STATIC_ASSERT(offsetof(CTCSummonSpell, Minion) == 0x11);
FABLE_STATIC_ASSERT(offsetof(CTCSummonSpell, PItemRemoved) == 0x2c);

CTCSummonSpell::~CTCSummonSpell()
{
}