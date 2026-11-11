// CEngineLandscapeRenderer::BuildBlendingTables() -- build the five 128x128 foreground blend tables: for every texel take the unit hemisphere normal over (x, y) in [-1, 1) and fill the texel of each mapping direction's table with that direction's blend weight scaled to 0..255
// Leaf: ?BuildBlendingTables@CEngineLandscapeRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B67270, 518 bytes to `ret` at 0x00B67475 (+10 int3; the catalog's 528 includes them), thiscall,
//   ebp frame (0xB4 locals: the inline-asm helper's frame constants force it).
// Identity: Ego_r.pdb ?BuildBlendingTables@CEngineLandscapeRenderer@@AAEXXZ (private; donor neighbours CalcForegroundFadeFactors /
//   DeleteMap landed beside it at 0x00B671E0 / 0x00B67180); the five CTexture members it fills are Ego_r ForegroundBlendTables[5]
//   (retail +0x5BC, engine/CEngineLandscapeRenderer.h).
// Retail-asm: 0x00B673C6..0x00B673E0 `fld [ebp-10h] ; fsub [ebp-4Ch] ; fistp dword ptr [ebp-18h] ; fild [ebp-18h] ; fadd [ebp-48h] ;
//   fstp [ebp-30h] ; mov eax,[ebp-10h] ; cmp eax,[ebp-30h] ; jne ; inc dword ptr [ebp-18h]` -- the engine's inline-assembly
//   GFFloatToLongFloor (engine/GFFloatToLongFloor.h, shared retail-asm helper with its FABLE_PORTABLE twin; 0.5f / 1.0f frame
//   constants stored once at 0x00B6732E / 0x00B67335).
// Callees: CPixelFormat::Initialise(8, 8, 8, 8, false) 0x009E3790 (the inline four-long constructor), CPixelFormat::GetByteColourDepth
//   0x009E3800, CTexture::Initialise 0x009FA280 (128x128, one level, usage 0, SURFACE_POOL_MANAGED), CTexture::Lock 0x009FA450 /
//   Unlock 0x009F9DE0, C3DVector::AccurateNormalise 0x00A14440, CEngineLandscapeMeshBuilder::GetMappingDirectionBlend 0x00BE2B70
//   (MeshBuilder +0x10). Constants: 1/64 0x0125B24C, 255.0f 0x01230014, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Spelling: the texel index is y * pitch + x * bytes_per_pixel + b (VC7.1 strength-reduces it into the two induction slots that
//   reuse the dead this / texture-pointer slots, ebp-8 / ebp-4); normal.Y is the inner-loop expression (VC7.1 hoists it to ebp-44h
//   but keeps Y*Y in the loop: a named outer float made it hoist Y*Y and grow the frame); PMem is read before Pitch (the pitch store
//   stays in the first loop).
// Headers: FABLE_HDR_OVERLAY=$A/hdr (CEngineLandscapeRenderer.h, CEngineLandscapeMeshBuilder.h, CTexture.h, C3DVector.h opt-ins).
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_BLENDTABLES
#define FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGBLEND
#define FABLE_RETAIL_CTEXTURE_LOCK
#define FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
#include <math.h>
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/C3DVector.h"
#include "engine/GFFloatToLongFloor.h"

enum { BLEND_TABLE_SIZE = 128, NUMBER_OF_BLEND_TABLES = 5 };

void CEngineLandscapeRenderer::BuildBlendingTables()
{
    CPixelFormat format(8, 8, 8, 8);
    long bytes_per_pixel = format.GetByteColourDepth();
    CSurfaceLockedRect locked_rects[NUMBER_OF_BLEND_TABLES];
    unsigned char* table_bits[NUMBER_OF_BLEND_TABLES];
    long pitch;
    for (long i = 0; i < NUMBER_OF_BLEND_TABLES; ++i) {
        ForegroundBlendTables[i].Initialise(C2DExtentsI(BLEND_TABLE_SIZE, BLEND_TABLE_SIZE), 1, format, 0, SURFACE_POOL_MANAGED, false, false);
        locked_rects[i] = ForegroundBlendTables[i].Lock(0, 0);
        table_bits[i] = locked_rects[i].PMem;
        pitch = locked_rects[i].Pitch;
    }
    for (long y = 0; y < BLEND_TABLE_SIZE; ++y) {
        for (long x = 0; x < BLEND_TABLE_SIZE; ++x) {
            C3DVector normal;
            normal.X = (x - BLEND_TABLE_SIZE / 2) * (1.0f / 64.0f);
            normal.Y = (y - BLEND_TABLE_SIZE / 2) * (1.0f / 64.0f);
            float z_squared = 1.0f - normal.X * normal.X - normal.Y * normal.Y;
            if (!(z_squared > 0.0f))
                z_squared = 0.0f;
            normal.Z = (float)sqrt(z_squared);
            normal.AccurateNormalise();
            for (long direction = 0; direction < NUMBER_OF_BLEND_TABLES; ++direction) {
                float blend = MeshBuilder.GetMappingDirectionBlend((LANDSCAPE_TEXTURE_MAPPING_DIRECTION)direction, normal) * 255.0f;
                long value = GFFloatToLongFloor(blend);
                for (long b = 0; b < bytes_per_pixel; ++b)
                    table_bits[direction][y * pitch + x * bytes_per_pixel + b] = (unsigned char)value;
            }
        }
    }
    for (long i = 0; i < NUMBER_OF_BLEND_TABLES; ++i)
        ForegroundBlendTables[i].Unlock(0);
}