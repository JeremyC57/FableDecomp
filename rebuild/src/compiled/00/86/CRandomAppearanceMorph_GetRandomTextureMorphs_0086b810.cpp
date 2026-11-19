// CRandomAppearanceMorph::GetRandomTextureMorphs(unsigned long&, unsigned long, const CVectorMap<unsigned long, CTextureMorph>&, CArray<CMorphSet::CMorphSetTextureMorph>&) const -- for each original texture in the morph map, pick one of its morphs that is ungrouped or in the requested group at random and append (texture, new texture) to the result
// Leaf: ?GetRandomTextureMorphs@CRandomAppearanceMorph@@IBEXAAKKABV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@AAV?$CArray@VCMorphSetTextureMorph@CMorphSet@CRandomAppearanceMorph@@@@@Z
// Flags: /d2inlT49
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086B810, length 230 bytes, `ret 10h` (`this` unused).
// Identity: Ego_r.pdb / FableWin.pdb ?GetRandomTextureMorphs@CRandomAppearanceMorph@@IBEXAAKKABV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@AAV?$CArray@VCMorphSetTextureMorph@CMorphSet@CRandomAppearanceMorph@@@@@Z
//   (protected const); the landed callers 0x0086BE40 (same TU in retail, twice) and 0x0086BF70 bind that spelling.
// Callees: CVectorMap<unsigned long, CTextureMorph>::UpperBound 0x0086A8D0 (the end of one key's run), GFROR13 0x00497890 (inside the
//   inline GFRandom), the result vector's grow path 0x0086CA20 (STLport _M_insert_overflow, out of line in this TU).
// NF98 (round 8, q11251, coordinator rule: one definition): this row was a stand-in spelling over a local model (CArray / CVectorMap / a
//   global CMorphSet), while the TU of 0x0086BE40 defined the PDB spelling strongly (its e01 same-TU body). The body now lives once in
//   engine/CRandomAppearanceMorph.h (FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_GETRANDOMTEXTUREMORPHS_BODY, inline __declspec(noinline), moved
//   unchanged from 0x0086BE40); both rows include it, and this row makes VC7.1 emit it as a pick-any COMDAT through the reference below (the
//   nf88 pattern, 0x008C1600: the member is protected, so the reference goes through an anonymous derived class). Same TU settings as
//   0x0086BE40 (/d2inlT49, FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE), under which e01 measured this body RELOCATION_MATCH at 0x0086B810.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND
#define FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TEXTUREMORPH_CTOR
#define FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_GETRANDOMTEXTUREMORPHS_BODY
#include "engine/CRandomAppearanceMorph.h"
// The body is the one inline definition in engine/CRandomAppearanceMorph.h (FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_GETRANDOMTEXTUREMORPHS_BODY).

namespace {
// Fidelity: scaffolding, not retail: takes the address of the header's inline (noinline) GetRandomTextureMorphs so this object emits it out
// of line as a pick-any COMDAT (retail's 0x0086B810). Internal to this TU; the class is never instantiated and the function never called.
class CGetRandomTextureMorphsReference : public CRandomAppearanceMorph {
public:
    static void (CRandomAppearanceMorph::*Reference())(unsigned long&, unsigned long, const TTextureMorphMap&, CArray<CMorphSet::CMorphSetTextureMorph>&) const;
};
// Out of class, so it is an ordinary (not inline) function that VC7.1 compiles even though nothing calls it.
void (CRandomAppearanceMorph::*CGetRandomTextureMorphsReference::Reference())(unsigned long&, unsigned long, const TTextureMorphMap&, CArray<CMorphSet::CMorphSetTextureMorph>&) const
{
    return &CGetRandomTextureMorphsReference::GetRandomTextureMorphs;
}
}
