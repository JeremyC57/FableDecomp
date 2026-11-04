// CTCPerceiveThing::UpdateDistanceFromPerceivedThing() -- DistanceFromPerceivedThingSquared = |owner position - perceived thing position|^2
// Leaf: ?UpdateDistanceFromPerceivedThing@CTCPerceiveThing@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007447E0, 77 bytes. Ego_r.pdb ?UpdateDistanceFromPerceivedThing@CTCPerceiveThing@@AAEXXZ (private).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (on PerceivedThing +0x0c), CThing::GetPos @ 0x004C73D0, twice.
// Layout: engine/CTCPerceiveThing.h (field4 +0x04 = the owner thing, PerceivedThing +0x0c, DistanceFromPerceivedThingSquared +0x14).
// Shape: the inline C3DVector difference (owner first) and the FABLE_RETAIL_C3DVECTOR parenthesised GetSquaredMagnitude
//   (X*X + Z*Z + Y*Y kept as one unit: the unparenthesised default schedules one of the three temporary pops after `pop ebx`;
//   the retail one sits between `pop esi` and `pop ebx`). The perceived thing and the owner are named locals in that order; the right operand's GetPos is called first.
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#define CTCPerceiveThing DonorCTCPerceiveThing
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing
class CTCPerceiveThing : public DonorCTCPerceiveThing {
    CThing* GetThing() const { return reinterpret_cast<CThing*>(field4); }
    const CIntelligentPointer<CThing>& GetPerceivedThing() const { return *reinterpret_cast<const CIntelligentPointer<CThing>*>(PerceivedThing); }
    void UpdateDistanceFromPerceivedThing();
};

void CTCPerceiveThing::UpdateDistanceFromPerceivedThing()
{
    CThing* perceived = GetPerceivedThing().Get();
    CThing* owner = GetThing();
    DistanceFromPerceivedThingSquared = (owner->GetPos() - perceived->GetPos()).GetSquaredMagnitude();
}