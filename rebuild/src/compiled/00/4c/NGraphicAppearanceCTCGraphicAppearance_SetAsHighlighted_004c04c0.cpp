// NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float, float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*) -- build a smooth-outline mesh effect (its texture alpha values from the thing's appearance def, else 0x80 / 0) and install it through SetPMeshEffect
// Leaf: ?SetAsHighlighted@CTCGraphicAppearance@NGraphicAppearance@@QAEXMMABVCRGBColour@@_N1W4EAppearanceMeshEffectPriority@2@PAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C04C0, 155 bytes (+5 int3 padding), `ret 1Ch`. FableWin.pdb
//   ?SetAsHighlighted@CTCGraphicAppearance@NGraphicAppearance@@QAEXMMABVCRGBColour@@_N1W4EAppearanceMeshEffectPriority@2@PAX@Z.
// Callees (PDB spellings): CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (on the base def read in place), operator new 0x00BFEA1A,
//   CEngineMeshEffectOutlineSmooth::CEngineMeshEffectOutlineSmooth(float, float, const CRGBColour&, unsigned char, long, long) 0x004C3000
//   (Ego_r.pdb / FableWin.pdb ??0CEngineMeshEffectOutlineSmooth@@QAE@MMABVCRGBColour@@EJJ@Z), CTCGraphicAppearance::SetPMeshEffect
//   0x004C0240, the def's virtual release.
// Layout: CThing PBaseDef +0x70; CAppearanceDef OutlineEffectAlphaRef +0x54 / OutlineEffectAlphaBias +0x55 (Ego_r.pdb, after the 0x20-byte
//   Animation at +0x34; `movzx esi,byte ptr [ebp+54h]`, `movzx edi,byte ptr [ebp+55h]`).
// Shape: the parent thing is read into a local first (the def staged in eax, `mov ecx,eax` after the out push); the effect handle is the adopting CIVCountedPointer(T*) built in the argument area (`push ecx ; mov ecx,esp ; mov [ecx],eax`).
// Name fix (agentNF11 2026-11-11): moved off the local CThing / CDefClassBase / CHighlightDef / CEngineMeshEffect stand-ins (and a
// same-TU stand-in GetSubDef body) onto the shared headers. 0x0042B0A2 binds GetSubDef<CAppearanceDef> (the two bytes are the appearance
// def's OutlineEffectAlphaRef / OutlineEffectAlphaBias; was a CHighlightDef stand-in), 0x004C3000 binds the PDB
// CEngineMeshEffectOutlineSmooth constructor (was ??0CEngineMeshEffect@@QAE@MMABVCRGBColour@@_NJJ@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CENGINEMESHEFFECT_OUTLINESMOOTH
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_FROM_RAW
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HIGHLIGHT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MESHEFFECT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAppearanceDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

namespace NGraphicAppearance {

void CTCGraphicAppearance::SetAsHighlighted(float widthWorldSpace, float widthScreenSpace, const CRGBColour& colour, bool p4, bool p5,
                                            EAppearanceMeshEffectPriority priority, void* owner)
{
    CThing* thing = ParentThing;
    const CThingBaseDef& base_def = thing->PeekBaseDef();
    CDefPointer<const CAppearanceDef> appearance;
    long alphaRef = 0x80;
    long alphaBias = 0;
    if (base_def.GetSubDef(&appearance)) {
        alphaRef = appearance->OutlineEffectAlphaRef;
        alphaBias = appearance->OutlineEffectAlphaBias;
    }
    SetPMeshEffect(CIVCountedPointer<CEngineMeshEffect>(new CEngineMeshEffectOutlineSmooth(widthWorldSpace, widthScreenSpace, colour, 0,
                                                                                          alphaRef, alphaBias)),
                   p4, p5, false, priority, owner);
}

}
