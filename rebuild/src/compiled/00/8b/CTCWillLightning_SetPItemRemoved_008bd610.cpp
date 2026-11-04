// CTCWillLightning::SetPItemRemoved(CIntelligentPointer<CThing> item) -- remember the item the lightning spell took from the caster's hands (the by-value pointer is assigned, then destroyed)
// Leaf: ?SetPItemRemoved@CTCWillLightning@@QAEXV?$CIntelligentPointer@VCThing@@@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BD610, `ret 0Ch` (by-value intelligent pointer).
// Identity: Ego_r.pdb ?SetPItemRemoved@CTCWillLightning@@QAEXV?$CIntelligentPointer@VCThing@@@@@Z; PItemRemoved +0x11.
// Callees: CIntelligentPointer<CThing>::operator= (FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN), ~CIntelligentPointer of the argument.
// Shape: out-of-line `PItemRemoved = item;`.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"

enum EWEAPON_STATE { };

class CTCWillLightning : public CTCBase {                    // RTTI CTCWillLightning : CTCBase (Ego_r.pdb 0xF0 bytes)
public:
    CTCWillLightning(CThing&);
    virtual ~CTCWillLightning();
    void SetPItemRemoved(CIntelligentPointer<CThing> item);
    EWEAPON_STATE                WeaponState;                // +0x0c
    bool                         LeftHandAnim;               // +0x10
    CIntelligentPointer<CThing>  PItemRemoved;               // +0x11
    long                         AIHoldingDownTimer;         // +0x1c
    // ... the lightning-beam members from +0x20 (Ego_r.pdb CTCWillLightning) are not needed here
};
FABLE_STATIC_ASSERT(offsetof(CTCWillLightning, PItemRemoved) == 0x11);

void CTCWillLightning::SetPItemRemoved(CIntelligentPointer<CThing> item)
{
    PItemRemoved = item;
}