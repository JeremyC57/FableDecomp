#ifndef FABLE_ENGINE_COPINIONDEEDREACTION_H
#define FABLE_ENGINE_COPINIONDEEDREACTION_H
// New header (p25O, row 0x00960120 CAIStateGroup_DeedReactionRoot::Update). COpinionDeedReaction: Ego_r.pdb TPI
// (struct_layouts_egor.tsv, 144 bytes, no STL member so the offsets are retail's): Animations CCharString[18] +0x00, DialogueTags
// ulong[18] +0x48. Indexed by EOpinionAttitudeType. CTCHeroOpinionDeedLog holds 92 of them at +0xE8 (retail
// `lea edx,[ebx+ecx*4+0E8h]` / `mov edi,[ebx+ecx*4+130h]`, 0x00960266 / 0x00960363). Needs fable_string.h first.
#include "rebuild_abi.h"
class COpinionDeedReaction {
public:
    CCharString Animations[18];                                          // +0x00
    unsigned long DialogueTags[18];                                      // +0x48
};
FABLE_STATIC_ASSERT(sizeof(COpinionDeedReaction) == 0x90);
#endif
