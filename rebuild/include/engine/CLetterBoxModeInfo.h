#ifndef FABLE_ENGINE_CLETTERBOXMODEINFO_H
#define FABLE_ENGINE_CLETTERBOXMODEINFO_H
// New shared header (wfA2, row 0x00435530 CDisplayEngine::DoRender). Ego_r.pdb CLetterBoxModeInfo (struct_layouts_egor.tsv, sizeof 0x20):
// a CFadeInFadeOutBase base subobject (0x1c bytes; the PDB lists it as base padding) followed by float Ratio +0x1C. Retail: CDisplayEngine::
// LetterBox +0x70; DoRender tests the base's Active byte +0x70 and calls CFadeInFadeOutBase::Update 0x00434870 with ecx = &LetterBox
// (0x004357BA..0x004357C8); GetLetterBoxRatio 0x004349F0 reads +0x8C (= +0x70 + Ratio +0x1C), GetLetterBoxAlpha 0x00434A00 forwards
// to GetFadeInOutAlpha on +0x70. No member functions of its own are modelled.
#include "rebuild_abi.h"
#include "engine/CFadeInFadeOutBase.h"
#pragma pack(push, 1)
class CLetterBoxModeInfo : public CFadeInFadeOutBase {
public:
    float Ratio;                                                         // +0x1c
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CLetterBoxModeInfo) == 0x20);
#endif // FABLE_ENGINE_CLETTERBOXMODEINFO_H
