#ifndef FABLE_ENGINE_CTCFADEOUTANDIN_H
#define FABLE_ENGINE_CTCFADEOUTANDIN_H
// New header (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad). Retail opt-in only: FABLE_RETAIL_CTCFADEOUTANDIN.
// Class: Ego_r.pdb CTCFadeOutAndIn : CTCBase (struct_layouts_egor.tsv: FadingOut +0x0C, FadingIn +0x0D, Alpha +0x10, FramesLeftForFade +0x14,
// FadingThings vector<CIntelligentPointer<CThing> > +0x18); interface TCI_FADE_OUT_AND_IN (249: UpdateRegionLoad's PeekTC key 0xF9 at 0x004A4025).
// Methods: Ego_r.pdb ?FadeOut@CTCFadeOutAndIn@@QAEXM_N@Z (0x006C3B70, called 0x004A3BD3), ?AddThingToFade@CTCFadeOutAndIn@@QAEXAAVCThing@@@Z
// (0x006C3DF0, called 0x004A3D84), ?IsFading@CTCFadeOutAndIn@@QBE_NXZ (inline body: COMDAT 0x0049D310 `FadingIn || FadingOut`, called).
#ifdef FABLE_RETAIL_CTCFADEOUTANDIN
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThing;
class CTCFadeOutAndIn : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_FADE_OUT_AND_IN; }
    void FadeOut(float time, bool);                                      // 0x006C3B70
    void FadeIn(float time, bool);                                       // 0x006C3BD0
    void AddThingToFade(CThing&);                                        // 0x006C3DF0
    bool IsFading() const { return FadingIn || FadingOut; }              // COMDAT 0x0049D310
protected:
    bool FadingOut;                                                      // +0x0c
    bool FadingIn;                                                       // +0x0d
    float Alpha;                                                         // +0x10
    long FramesLeftForFade;                                              // +0x14
    std::vector<CIntelligentPointer<CThing> > FadingThings;              // +0x18
};
#endif
#endif // FABLE_ENGINE_CTCFADEOUTANDIN_H
