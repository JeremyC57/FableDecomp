// 00ba9cb0 CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap
// Leaf: ?GetAppropriateMipmap@CEnginePrimitiveManagerMeshBase@@SIKABVC3DVector@@ABVCMatrix3x4@@ABVC3DPrimitive2@@MPBVC3DMeshMaterial@@PBVCGraphicDataBank@@@Z
// Retail-asm-helper: GFFloatToLongFloor
// Retail-asm: 0x00BA9DE2..0x00BA9DFC uses fistp dword for a rounded conversion and compares the next integral float.
// Flags: /d2inlT250
// Inline-budget: retail expands CGraphicDataBank::PeekGraphic at the width lookup. Plain /O2 emits a call.
#define FABLE_RETAIL_MESH_MIPMAP_PASS30
#define FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_GETWIDTH
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MIPMAPSIZEMULTIPLIER
#define FABLE_RETAIL_C3DPRIMITIVE2_HIERARCHY
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED
#include "rebuild_abi.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFGetTopBitInByte.h"

extern const float DELTA_NOTIONAL_ZERO;

unsigned long FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(
    const C3DVector& cam_pos, const CMatrix3x4& object_transform, const C3DPrimitive2& prim,
    float object_scale, const C3DMeshMaterial* material, const CGraphicDataBank* graphic_bank)
{
    C3DBoundingSphere bsphere = prim.GetBoundingSphere();
    object_transform.TransformPoint(bsphere.Center);
    float distance = (cam_pos - bsphere.Center).GetMagnitude();
    unsigned long mipmap_level = 0;
    float min_distance = distance - bsphere.GetRadius() * object_scale;
    float dtexdpos = prim.GetAverageTextureStretch();
    float divisor = min_distance * dtexdpos;
    if (divisor > DELTA_NOTIONAL_ZERO) {
        long diffuse_index = material->GetDiffuseMapBankIndex();
        if (diffuse_index > 0) {
            unsigned long texture_size = graphic_bank->PeekGraphic(diffuse_index)->GetWidth();
            float ideal_size = CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() * object_scale / divisor;
            long ratio = GFFloatToLongFloor(texture_size / ideal_size);
            if (ratio < 1)
                ratio = 1;
            else if (ratio > 255)
                ratio = 255;
            mipmap_level = GFGetTopBitInByte((unsigned char)ratio);
        }
    }
    return mipmap_level;
}