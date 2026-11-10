#ifndef FABLE_ENGINE_CAPPEARANCEDEF_H
#define FABLE_ENGINE_CAPPEARANCEDEF_H
// CAppearanceDef (new shared header, chunk 1006, row 0x005B3E4E CTCInventoryClothing::GlobalUpdate; the generated flat model is in
// engine/_quarantine/). Retail class: RTTI .?AVCAppearanceDef@@ : CSubDefClassBase (ghidra_out/rtti_bases.tsv). Ego_r.pdb members
// (packed after the 0x25-byte base): Graphic +0x25 (CEngineGraphic, opaque here), Animation +0x34 (CAnimationSet: the row adds 34h
// to the def pointer before GetAnim, 0x005B3EC0); the members after Animation are not modelled. Reached through
// CDefClassBase::GetSubDef<CAppearanceDef> (out of line in retail at 0x0042B0A2). Needs FABLE_RETAIL_DEF_CLASS_BASE.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CAnimationSet.h"
#ifdef FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
#include "engine/CEngineGraphic.h"
enum ELightingChannel;
#endif
#pragma pack(push, 1)
class CAppearanceDef : public CSubDefClassBase {
public:
    // Ego_r.pdb / FableWin.pdb ?StatGetTypeName@CAppearanceDef@@SI?AVCCharString@@XZ (inline: the class-name key GetSubDef looks up).
    static CCharString __fastcall StatGetTypeName() { return CCharString("CAppearanceDef", -1); }
#ifdef FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
    CEngineGraphic Graphic;
    unsigned char _pad_0x33;
#else
    unsigned char Graphic[0xf];                              // +0x25 CEngineGraphic
#endif
    CAnimationSet Animation;                                 // +0x34
#ifdef FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
    unsigned char OutlineEffectAlphaRef,OutlineEffectAlphaBias;
    bool OverrideLightingChannel;
    unsigned char _pad_0x57;
    ELightingChannel LightingChannel;
    CEngineGraphic FadeOutWhenCloseGraphic;
    bool HasFadeOutWhenCloseGraphic;
    unsigned char _pad_0x6b;
    float FadeOutGraphicRenderFadeDistance;
    float FadeOutGraphicNearRenderFadeDistance;
    float FadeOutGraphicNearRenderClipDistance;
#endif
};
#pragma pack(pop)
#ifdef FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
FABLE_STATIC_ASSERT(sizeof(CAppearanceDef)==120);
#endif
#endif // FABLE_ENGINE_CAPPEARANCEDEF_H
