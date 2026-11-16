// CGraphicInfo::GetVolumeTexture(CVolumeTexture**, unsigned long) const -- forward to the first frame: `mov ecx,[ecx+28h] ; jmp 0x009FF710`
// Leaf: ?GetVolumeTexture@CGraphicInfo@@QBE_NPAPAVCVolumeTexture@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FFB20.
// NF39 (2026-11-16): moved off its T::Run stand-in onto the shared engine/CGraphicInfo.h (opt-in FABLE_RETAIL_CGRAPHICINFO_GETVOLUMETEXTURE,
//   which names 0x009FFB20; Frames +0x28): Ego_r.pdb 0x00B5D250 ?GetVolumeTexture@CGraphicInfo@@QBE_NPAPAVCVolumeTexture@@K@Z; the callee is
//   Ego_r.pdb 0x00B5CAA0 / FableWin.pdb ?GetVolumeTexture@CGraphicInfoFrame@@QBE_NPAPAVCVolumeTexture@@K@Z (overlay opt-in
//   FABLE_RETAIL_CGRAPHICINFOFRAME_GETVOLUMETEXTURE in engine/CGraphicInfoFrame.h). Its landed caller binds this symbol. Defines from the
//   landed 0x00B54890 (the same CGraphicInfo opt-in).
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGRAPHICINFO_GETVOLUMETEXTURE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETVOLUMETEXTURE
#include "engine/CGraphicInfo.h"

bool CGraphicInfo::GetVolumeTexture(CVolumeTexture** texture, unsigned long level) const
{
    return Frames->GetVolumeTexture(texture, level);
}
