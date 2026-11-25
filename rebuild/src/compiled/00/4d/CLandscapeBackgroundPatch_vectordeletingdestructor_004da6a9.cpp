// std::pair<long, CIntelligentPointer<CThing> >::`scalar deleting destructor'(unsigned int flags) -- the element type of the
//   CArray<pair<long, CIntelligentPointer<CThing> > > vectors: ~pair (the CIntelligentPointer<CThing> at +4, by ~CBaseIntelligentPointer
//   0x00A01C10), then operator delete when bit 0 is set; returns this
// Leaf: ??_G?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA6A9, 31 bytes, thiscall, `ret 4`; size-optimised TU. Two references, both `push 0 ; call`:
//   the 12-byte-stride __destroy_aux 0x004DD79F (at 0x004DD7AB) and the unreferenced __fastcall _Destroy copy 0x004DBEDF (at 0x004DBEE1).
//   Compiler-generated: STLport _construct.h `_Destroy(p) { p->~T(); }` is emitted by VC7.1 as a call of this ??_G with 0.
// Name (NF409): (1) the binding table already maps this spelling here (witness 0x004DD79F, whose object emits it and calls it per
//   element); (2) FableWin.pdb's name set holds the non-const element type: CArray / vector / _Destroy / allocator of
//   pair<long, CIntelligentPointer<CThing> > (??1?$CArray@U?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@@@QAE@XZ and siblings), and the
//   retail body destroys a CIntelligentPointer (vptr + PData) at +4 after a 4-byte key, 12-byte elements. STL instances have no PDB
//   ??_G (both PDBs are Dinkumware): the spelling is the repo STLport's (descriptive-by-library).
// NF409: was the strong free stand-in ?vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z (catalog
//   ScalarDeletingDtor_004da6a9 / manifest CLandscapeBackgroundPatch::`vector_deleting_destructor', propagated) on a row-local model.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern): the deleting destructor is a compiler-emitted COMDAT; the anonymous-namespace
//   helper below destroys a pair through a typedef so this object emits it pick-any, as 0x004DD79F's does. Internal to this TU; never
//   called. `#pragma optimize("s",on)` is the row's own mode.
#pragma optimize("s",on)
#include "stlport/fable_containers.h"
class CThing;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

namespace {
typedef std::pair<long, CIntelligentPointer<CThing> > CThingIndexPair;
void EmitThingIndexPairDeletingDestructor(CThingIndexPair* element)
{
    element->~CThingIndexPair();
}
}
