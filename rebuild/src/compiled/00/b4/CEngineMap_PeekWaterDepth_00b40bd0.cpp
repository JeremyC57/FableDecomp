// CEngineMap::PeekWaterDepth(long x, long y) const -- the water depth of a landscape cell: the blend-weighted (1/255) sum of the water heights of the CLandscapeTheme records of the cell's first three engine theme layers, stopping at the first layer with no theme (0 when the first has none); the third layer's blend is what the first two leave of 255
// Leaf: ?PeekWaterDepth@CEngineMap@@QBEMJJ@Z
// Flags: /d2inlT576
// Inline-budget: /d2inlT576 -- the five expanded Peek sites (inline trace, c2.dll 0x1078D1AA: c1 308 each; scores PeekThemeId 403 at
//   f=1, PeekThemeBlend / PeekThemeId 460 at f=0.5, PeekThemeBlend / PeekThemeId 575 at f=0.25): retail expands all five
//   (0x00B40BD0..0x00B40CEF, 0x00B40D0F.., 0x00B40E71.., 0x00B40FA9.., 0x00B41133..), T<=575 calls the last two; no plain control
//   flow raises a straight-line site above f=1, where the default budget already calls it. The same TU's 0x00B3F5E0 matches at
//   T576 too (its own smallest is T444).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B40BD0, 1736 bytes (0x00B40BD0..0x00B41297, `ret 8` at 0x00B41285 and 0x00B41295; the
//   next function, CEngineMap::PeekHasWater 0x00B412A0, follows after int3 padding), thiscall, float in st(0).
// Identity (proven): Ego_r.pdb ?PeekWaterDepth@CEngineMap@@QBEMJJ@Z (const, two longs, float; donor order between
//   PeekInterpolatedHasWaterFast and the CEngineWorldMap members, as retail 0x00B40BD0 sits after PeekWaterType 0x00B40670); also FableWin.pdb; callers
//   0x00B416A0 (PeekHasWater), 0x00B418A4 (PeekInterpolatedWaterHeight), 0x00B41A96 (PeekWaterHeight) and 0x00B41D3C (the landed
//   CEngineMap::PeekInterpolatedWaterDepth 0x00B41C30, which declares it with this spelling).
// Callees: the header inline CEngineMap::PeekThemeId (x3) / PeekThemeBlend (x2), expanded (their out-of-line COMDAT copies
//   0x00B43720 / 0x00B43870 in this TU's inline run are called only for a cell held by the neighbouring map, the inline self
//   call); ?GetDefClassIndexFromGlobalIndex@CDefinitionManager@@QBEJJ@Z 0x009ACCA0 on g_pEngineDefinitionManager 0x01436E28;
//   the inline FableWin.pdb ?PeekTheme@CEngineLandscapeRenderer@@QBEABUCLandscapeTheme@@K@Z on g_LandscapeRenderer_01436EA8
//   (LandscapeThemes +0x4B8, 0x38-byte records, WaterHeight +0x34); the CIDrawMap virtuals DrawGetEngineThemeAt (slot 6,
//   `call [reg+18h]`) and DrawGetEngineBlendAt (slot 7, `call [reg+1Ch]`); g_pEngineWorldMap 0x01436E8C (GetMap(x, y)).
// Constants: 1/255 is the float pool __real@3b808081 (0x01231724), 0.0f __real@00000000 (0x0122DEDC).
// Shape (what made it match, 9 grades):
//   - the Peek bodies are the shared header's (hdr/engine/CEngineMap.h FABLE_RETAIL_CENGINEMAP_PEEK_BODIES, plain inline) and read
//     g_pEngineWorldMap at each use: a `worldMap` local copy kept the first copy's MapWidth CSE out of its spill slot (retail
//     0x00B40BF3 `mov [esp+38h],ecx`, reused at 0x00B40CC0) and shifted the x / y copies' registers;
//   - nested ifs with one `return depth` (retail's last layer stores depth and joins the `fld [esp+18h]` exit 0x00B4127A; the
//     `return 0.0f` block sits at the end, 0x00B41288);
//   - one `index` local per layer (retail keeps index0 / index1 in their own slots [esp+24h] / [esp+2Ch]);
//   - `(blend * WaterHeight) * (1.0f / 255.0f)`: the grouping's PAREN tuple puts the second layer's `fmul` before the two
//     copy stores (0x00B4111D..0x00B41127), as retail; the other two layers schedule the same either way.
#define FABLE_RETAIL_CENGINEMAP_PEEK
#define FABLE_RETAIL_CENGINEMAP_PEEKWATER
#define FABLE_RETAIL_CENGINEMAP_PEEK_BODIES
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP_DRAW
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_PEEKTHEME
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"
#include "engine/CDefinitionManager.h"
#include "engine/CEngineLandscapeRenderer.h"

extern CDefinitionManager* g_pEngineDefinitionManager;           // 0x01436E28 (landed spelling, CEngine::Initialise 0x00B26360)
extern CEngineLandscapeRenderer* g_LandscapeRenderer_01436EA8;   // 0x01436EA8 (landed spelling, CEngine::Initialise 0x00B26360)

float CEngineMap::PeekWaterDepth(long x, long y) const
{
    long theme0 = PeekThemeId(x, y, 0);
    if (theme0 > 0) {
        long index0 = g_pEngineDefinitionManager->GetDefClassIndexFromGlobalIndex(theme0);
        unsigned char blend0 = PeekThemeBlend(x, y, 0);
        float depth = (blend0 * g_LandscapeRenderer_01436EA8->PeekTheme(index0).WaterHeight) * (1.0f / 255.0f);
        long theme1 = PeekThemeId(x, y, 1);
        if (theme1 > 0) {
            long index1 = g_pEngineDefinitionManager->GetDefClassIndexFromGlobalIndex(theme1);
            unsigned char blend1 = PeekThemeBlend(x, y, 1);
            depth += (blend1 * g_LandscapeRenderer_01436EA8->PeekTheme(index1).WaterHeight) * (1.0f / 255.0f);
            long theme2 = PeekThemeId(x, y, 2);
            if (theme2 > 0) {
                long index2 = g_pEngineDefinitionManager->GetDefClassIndexFromGlobalIndex(theme2);
                unsigned char blend2 = 255 - blend1 - blend0;
                depth += (blend2 * g_LandscapeRenderer_01436EA8->PeekTheme(index2).WaterHeight) * (1.0f / 255.0f);
            }
        }
        return depth;
    }
    return 0.0f;
}