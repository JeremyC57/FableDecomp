// Leaf: ?RenderForeground@CEngineLandscapePatch@@
// Grade: DIFFER(316v4880) -- residue: retail inline asm (the shared 19-instruction SSE AABB distance block at 0x00BF47A6) + the landed source is a 59-line paraphrase calling invented helpers
// CEngineLandscapePatch::RenderForeground() @ 0x00BF4570, real span 4880 bytes (code 4860 + a 5-entry jump table at
//   0x00BF586C; catalog 4431). This file is the LANDED source unchanged; it builds 316 bytes. Retail inlines the whole render
//   path (1334 instructions, 86 calls). Blocked twice: (1) the same inline-asm AABB squared-distance block as
//   CEngineLandscapeMap::UpdateForeground 0x00BDC510 (ecx/edx reloaded from [esp+30h]/[esp+10h] right after being stored,
//   result via movss [esp+28h] / fld), over the retail_asm cap of 8; (2) a 4.9 KB render function needs a full fresh
//   reconstruction (out of scope for this pass).
#include "candidates/landscape_patch_render_foreground_compiled.h"

void CEngineLandscapePatch::RenderForeground()
{
    if ((Flags & 4) == 0)
        return;

    BeginLandscapeForegroundRenderState();
    if (!ClipLandscapeForegroundPatch(this))
    {
        RestoreLandscapeForegroundRenderState();
        return;
    }

    int diffusePass = 0;
    int projectedPass = 0;
    SetupLandscapeForegroundLighting(this, &diffusePass, &projectedPass);
    SetLandscapeForegroundWorldAndConstants(this);

    int currentPass = -1;
    int currentVariant = -1;
    for (CForegroundRenderEntry* entry = Foreground; entry != 0; entry = entry->Next)
    {
        UpdateLandscapeForegroundGlobalLighting(entry->GlobalLightScale);
        int pass = entry->ProjectedFrame == 0 ? projectedPass : diffusePass;
        if (pass != currentPass)
        {
            ApplyLandscapeForegroundShaderPass(pass);
            currentPass = pass;
        }
        if (entry->ShaderVariantIndex != currentVariant)
        {
            ApplyLandscapeForegroundShaderVariant(entry->ShaderVariantIndex, this);
            currentVariant = entry->ShaderVariantIndex;
        }

        bool bindProjected = pass == 4 && entry->ProjectedFrame != 0;
        if (bindProjected && !PrepareLandscapeForegroundTexture(
                entry->ProjectedFrame, entry->ProjectedLodIndex, true))
        {
            Flags = static_cast<fable_u8>(Flags & 0xFB);
            break;
        }
        if (entry->DiffuseFrame == 0 || !PrepareLandscapeForegroundTexture(
                entry->DiffuseFrame, entry->DiffuseLodIndex, false))
        {
            Flags = static_cast<fable_u8>(Flags & 0xFB);
            break;
        }

        BindLandscapeForegroundEntry(entry, bindProjected);
        DrawLandscapeForegroundEntry(entry->BaseVertexIndex, entry->PrimitiveCount);
    }

    DisableLandscapeForegroundPointLights();
    UnbindLandscapeForegroundResources();
    RenderLandscapeForegroundWater(WaterPatch);
    RestoreLandscapeForegroundRenderState();
}
