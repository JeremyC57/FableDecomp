// CTCNoise::GetCreator() const -- the thing that made this noise
// Leaf: ?GetCreator@CTCNoise@@QBEPBVCThing@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007498D0, 8 bytes: `add ecx,48h ; jmp 0x00A01B50` (Creator.GetPItem(), tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?GetCreator@CTCNoise@@QBEPBVCThing@@XZ, the class's only reader of its
//   CIntelligentPointer<const CThing> Creator (Ego_r struct_layouts +0x48, the offset this body forwards; SetCreator / GetCreator are
//   Ego_r 0x0075B900 / 0x0075B950); the sole caller CThingFilter_NoiseCameFromThing::operator() 0x006A8229 compares the result with a
//   thing. The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50.
// Header: engine/CTCNoise.h (new), engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CTCNoise.h"
const CThing* CTCNoise::GetCreator() const
{
    return Creator.Get();
}