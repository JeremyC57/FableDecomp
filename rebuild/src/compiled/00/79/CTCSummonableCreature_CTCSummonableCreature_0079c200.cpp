// CTCSummonableCreature::~CTCSummonableCreature() -- summoned-creature component dtor (no user code): ~Summoner, the def pointer's release, then ~CTCBase
// Leaf: ??1CTCSummonableCreature@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079C200.
// Identity: RTTI vtable 0x0126C41C (CTCSummonableCreature); Ego_r.pdb PDef +0xc, Summoner CIntelligentPointer<CThingCreatureBase> +0x11.
// Shape: empty user body; ~Summoner, the def pointer release, ~CTCBase.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CThingCreatureBase;
class CSummonableCreatureDef;

class CTCSummonableCreature : public CTCBase {
public:
    CTCSummonableCreature(CThing&);
    virtual ~CTCSummonableCreature();
    CDefPointer<const CSummonableCreatureDef>   PDef;                      // +0x0c
    bool                                        HasBeenSummoned;           // +0x10
    CIntelligentPointer<CThingCreatureBase>     Summoner;                  // +0x11
};
FABLE_STATIC_ASSERT(offsetof(CTCSummonableCreature, Summoner) == 0x11);

CTCSummonableCreature::~CTCSummonableCreature()
{
}