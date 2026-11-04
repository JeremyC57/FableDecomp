// CTCWillEnflame::~CTCWillEnflame() -- enflame-spell component dtor (no user code): ~PTarget, ~PItemRemoved, the def pointer's release, then ~CTCBase
// Leaf: ??1CTCWillEnflame@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A4310.
// Identity: RTTI vtable 0x0126C824 (CTCWillEnflame); Ego_r.pdb members PDef +0x10, PItemRemoved +0x18, PTarget +0x21.
// Shape: empty user body; members in reverse order, then ~CTCBase.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CSpecialAbilitiesEnflameDef;

class CTCWillEnflame : public CTCBase {
public:
    CTCWillEnflame(CThing&);
    virtual ~CTCWillEnflame();
    bool                                     EnflameActionInitialised;     // +0x0c
    bool                                     Released;                     // +0x0d
    CDefPointer<const CSpecialAbilitiesEnflameDef> PDef;                   // +0x10
    long                                     SpellLevel;                   // +0x14
    CIntelligentPointer<CThing>              PItemRemoved;                 // +0x18
    bool                                     IsZTargetting;                // +0x20
    CIntelligentPointer<CThing>              PTarget;                      // +0x21
};
FABLE_STATIC_ASSERT(offsetof(CTCWillEnflame, PDef) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CTCWillEnflame, PTarget) == 0x21);

CTCWillEnflame::~CTCWillEnflame()
{
}