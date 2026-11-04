// CTCPerceiveThing::GetPerceivedThing() const -- the thing this perception component currently perceives
// Leaf: ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007446A0, 8 bytes: `add ecx,0Ch ; jmp 0x00A01B50` (PerceivedThing.GetPItem(), tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ (the class's only accessor of
//   its CIntelligentPointer<const CThing> PerceivedThing, Ego_r struct_layouts +0x0C, the offset this body forwards); its 72 callers are
//   perception users (CTCCreatureOpinionOfHero::FrameUpdate / InformOfDeed, CThingFilter_IsPerceivingThing / IsAwareOfThing, CTCWife,
//   CTCCombat::IsToBlock ...) that use the result as a thing. The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50 (through CIntelligentPointer<const CThing>::Get()).
// Header: engine/CTCPerceiveThing.h (FABLE_RETAIL_CTCPERCEIVETHING), engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPERCEIVETHING
#include "engine/CTCPerceiveThing.h"
const CThing* CTCPerceiveThing::GetPerceivedThing() const
{
    return PerceivedThing.Get();
}