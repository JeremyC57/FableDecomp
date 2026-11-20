// CShaderRenderManager::SetVertexShaderConstantLayout(long index) -- select vertex constant layout `index` (-1: the null layout); when a
// layout was current, mark the shader state the new layout's constant ranges invalidate (world matrix, fog transform, ambient, fog colour,
// camera position, lights, light globals, combined projection), recount / reallocate the lights and reset the light buffer and globals.
// Leaf: ?SetVertexShaderConstantLayout@CShaderRenderManager@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098B5E0.
// NF134 (2026-11-20, q4650): re-authored on engine/CShaderRenderManager.h (Ego_r.pdb 0x00AE7D60 + FableWin.pdb 0x03030910
//   ?SetVertexShaderConstantLayout@CShaderRenderManager@@QAEXJ@Z; members CurrentVSConstantLayout +0x4, CurrentVSConstantLayoutIndex +0x8,
//   VertexShaderConstantLayouts +0xC, NullConstantLayout +0x18, Lights +0x164, LightBuffer +0x168, LightCount +0x16C, AllocatedLights +0x170,
//   AllocatedLightSize +0x174, LightGlobals +0x194, UpdateFlags +0x3D8) and engine/CVertexShaderConstantLayout.h (friend access to the
//   CShaderConstantRange members). Was the row-local CShaderRenderManagerSetLayoutView / CVertexShaderConstantLayoutView /
//   CShaderConstantRangeView / C4DVectorView model.
// Callees: ??9CShaderConstantRange@@QBE_NABV0@@Z 0x00987D70 (FableWin.pdb 0x03031230; the light-globals test is the one comparison
//   VC7.1 kept out of line), ?AllocateLights@CShaderRenderManager@@AAEXKK@Z 0x0098B420 (Ego_r.pdb 0x00AE7900 + FableWin.pdb 0x03038AA0),
//   std::vector<C4DVector>::resize 0x0098DA90 (STLport, out of line), ?GFClear4DVectors@NVectorMath@@YAXPAVC4DVector@@KABV2@@Z 0x00A5C300,
//   ~CBaseClass 0x0099A2E0 (the implicit ~CShaderConstantRange of the GetLightArray() temporary; vftable ??_7CShaderConstantRange@@6B@
//   0x0129A04C stored by its implicit copy).
#define FABLE_RETAIL_CSHADERCONSTANTRANGE_IMPLICIT_COPY
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT_GETLIGHTARRAY_INLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "engine/CShaderRenderManager.h"
#include "engine/NVectorMath.h"

void CShaderRenderManager::SetVertexShaderConstantLayout(long index)
{
    if (index == -1)
    {
        CurrentVSConstantLayoutIndex = -1;
        CurrentVSConstantLayout = &NullConstantLayout;
        return;
    }

    CVertexShaderConstantLayout* new_layout =
        const_cast<CVertexShaderConstantLayout*>(VertexShaderConstantLayouts[index]);

    if (CurrentVSConstantLayout != 0)
    {
        UpdateFlags |= 4;

        if (CurrentVSConstantLayout->WorldMatrix.Offset != new_layout->WorldMatrix.Offset ||
            CurrentVSConstantLayout->WorldMatrix.Count != new_layout->WorldMatrix.Count)
        {
            UpdateFlags |= 0x10;
        }

        if (CurrentVSConstantLayout->FogTransform.Offset != new_layout->FogTransform.Offset ||
            CurrentVSConstantLayout->FogTransform.Count != new_layout->FogTransform.Count)
        {
            UpdateFlags |= 0x6000;
        }

        if (CurrentVSConstantLayout->Ambient.Offset != new_layout->Ambient.Offset ||
            CurrentVSConstantLayout->Ambient.Count != new_layout->Ambient.Count)
        {
            UpdateFlags |= 4;
        }

        if (CurrentVSConstantLayout->FogColour.Offset != new_layout->FogColour.Offset ||
            CurrentVSConstantLayout->FogColour.Count != new_layout->FogColour.Count)
        {
            UpdateFlags |= 0x20000;
        }

        if (CurrentVSConstantLayout->CameraPos.Offset != new_layout->CameraPos.Offset ||
            CurrentVSConstantLayout->CameraPos.Count != new_layout->CameraPos.Count)
        {
            UpdateFlags |= 8;
        }

        const unsigned long light_size = new_layout->LightSize;
        if (CurrentVSConstantLayout->LightSize != light_size ||
            CurrentVSConstantLayout->LightArray.Offset != new_layout->LightArray.Offset ||
            CurrentVSConstantLayout->LightArray.Count != new_layout->LightArray.Count ||
            CurrentVSConstantLayout->LightGlobals != new_layout->LightGlobals)
        {
            if (static_cast<long>(light_size) > 0)
            {
                LightCount = new_layout->GetLightArray().Count / light_size;

                if (LightCount > AllocatedLights || light_size > AllocatedLightSize)
                {
                    AllocateLights(LightCount, light_size);
                }

                for (unsigned long light = 0; light < LightCount; ++light)
                {
                    Lights[light].UpdateFlag = true;
                }

                NVectorMath::GFClear4DVectors(LightBuffer, LightCount * light_size, C4DVector(0.0f, 0.0f, 0.0f, 1.0f));
            }
            UpdateFlags |= 0x700;
        }

        const CShaderConstantRange& current_light_globals = CurrentVSConstantLayout->LightGlobals;
        if (current_light_globals.Offset != new_layout->LightGlobals.Offset ||
            current_light_globals.Count != new_layout->LightGlobals.Count)
        {
            unsigned long count = new_layout->LightGlobals.Count;
            if (count <= LightGlobals.size())
            {
                count = LightGlobals.size();
            }

            LightGlobals.resize(count, C4DVector(0.0f, 0.0f, 0.0f, 1.0f));
            UpdateFlags |= 0x400;
        }

        const CShaderConstantRange& current_combined_projection_matrix = CurrentVSConstantLayout->CombinedProjectionMatrix;
        if (current_combined_projection_matrix.Offset != new_layout->CombinedProjectionMatrix.Offset ||
            current_combined_projection_matrix.Count != new_layout->CombinedProjectionMatrix.Count)
        {
            UpdateFlags |= 0x80;
        }
    }

    CurrentVSConstantLayout = new_layout;
    CurrentVSConstantLayoutIndex = index;
}
