// CRandomAppearanceMorph::GetRandomMorphSet(unsigned long& seed, CMorphSet& morph_set, bool flag) const -- roll a complete random morph set:
//   skeletal morphs, body parts, then the general texture morphs
// Leaf: ?GetRandomMorphSet@CRandomAppearanceMorph@@QBEXAAKAAVCMorphSet@1@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086BF70, length 58 bytes (`ret 0Ch`). Called from 0x00728181
// (CTCRandomAppearanceMorph). Callees: GetRandomSkeletalMorph @ 0x0086B9D0, GetRandomBodyParts @ 0x0086BE40,
// GetRandomTextureMorphs @ 0x0086B810 (all protected const CRandomAppearanceMorph members, Ego_r.pdb / FableWin.pdb IBE).
// Identity: Ego_r.pdb (0x00857780) + FableWin.pdb ?GetRandomMorphSet@CRandomAppearanceMorph@@QBEXAAKAAVCMorphSet@1@_N@Z (public const).
// Shape: three member calls forwarding the seed and the set; the last passes group 0, the class's general texture-morph map
// (`lea ecx,[esi+28h]`) and the set's array (`add edi,18h` on the set register, no longer needed).
// Port (NF93, q11250 / q11251): was a local CRandomAppearanceMorph / CMorphSet / CVectorMap model with public non-const QAE callees and a
//   global CMorphSet; now on engine/CRandomAppearanceMorph.h (opt-ins _SKELETAL and the NF93 _GETRANDOMMORPHSET declaration).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_SKELETAL
#define FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_GETRANDOMMORPHSET
#include "engine/CRandomAppearanceMorph.h"

void CRandomAppearanceMorph::GetRandomMorphSet(unsigned long& seed, CMorphSet& morph_set, bool flag) const
{
    GetRandomSkeletalMorph(seed, morph_set);
    GetRandomBodyParts(seed, morph_set, flag);
    GetRandomTextureMorphs(seed, 0, GeneralTextureMorphs, morph_set.GeneralTextureMorphs);
}
