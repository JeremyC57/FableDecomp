// CEngineMap::PeekHasWaterFast(const long mapx, const long mapy) const -- true when any of the cell's first three theme layers (stopping at the first empty one) has a water type
// Leaf: ?PeekHasWaterFast@CEngineMap@@QBE_NJJ@Z
// Flags: /d2inlT1267
// Inline-budget: /d2inlT1267 -- retail expands PeekThemeId at all three layer sites (0x00B40260.., 0x00B403C6.., 0x00B40513..; the
//   out-of-line copy 0x00B43720 is called only for the neighbouring map). With the default threshold none expands; T576 (the twin
//   PeekWaterDepth 0x00B40BD0's value) expands the two later sites but not the first; bisected: T1266 leaves the first site a call,
//   T1267 is the smallest that expands all three (no c2 inline trace tool in this lane; the first site is the one not yet compiled
//   when this function is costed in a one-function TU).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B40260, 1037 bytes, thiscall `ret 8`, bool in al.
// Ego_d.pdb: parameters `const long mapx, const long mapy`, locals t0 (function scope), { t1 { t2 } } -- the nesting below; the text
//   follows the landed twin CEngineMap::PeekWaterDepth 0x00B40BD0 (same Peek bodies, same globals, theme lookup per layer).
#define FABLE_RETAIL_PASS22_WATER_PATCH_LOAD
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
// NF44 (round 6): the world-map global 0x01436E8C is NEngineInternal::GPEngineWorldMap (Ego_r.pdb / FableWin.pdb
// ?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A), through the engine/CEngineMap.h opt-in (was g_pEngineWorldMap).
#define FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#include "engine/EWaterType.h"
#include "engine/CEngineMap.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawMap.h"
#include "engine/CDefinitionManager.h"
#include "engine/CEngineLandscapeRenderer.h"

namespace NEngineInternal { extern const CDefinitionManager* GPDefinitionManager; }   // 0x01436E28 ?GPDefinitionManager@NEngineInternal@@3PBVCDefinitionManager@@B (Ego_r / FableWin / Ego_d)

bool CEngineMap::PeekHasWaterFast(const long mapx, const long mapy) const
{
    long t0 = PeekThemeId(mapx, mapy, 0);
    if (t0 > 0) {
        if (NEngineInternal::GPLandscapeRenderer->PeekTheme(NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(t0)).WaterType != WATER_TYPE_NULL)
            return true;
        long t1 = PeekThemeId(mapx, mapy, 1);
        if (t1 > 0) {
            if (NEngineInternal::GPLandscapeRenderer->PeekTheme(NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(t1)).WaterType != WATER_TYPE_NULL)
                return true;
            long t2 = PeekThemeId(mapx, mapy, 2);
            if (t2 > 0) {
                if (NEngineInternal::GPLandscapeRenderer->PeekTheme(NEngineInternal::GPDefinitionManager->GetDefClassIndexFromGlobalIndex(t2)).WaterType != WATER_TYPE_NULL)
                    return true;
            }
        }
    }
    return false;
}