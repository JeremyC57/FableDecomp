// CRegionMinimap::BuildRegionMinimapTGA(const CWideString& filename) -- colour every minimap pixel from its cell's theme (lit themes blended towards a darkened copy by the cell's light value), save the surface as a TGA, then release the cell list
// Leaf: ?BuildRegionMinimapTGA@CRegionMinimap@@QAEXABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066CD50, 423 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb 0x006A2860 / FableWin.pdb 0x021886B0 ?BuildRegionMinimapTGA@CRegionMinimap@@QAEXABVCWideString@@@Z; caller
//   CreateRegionMinimapTGAFile 0x0066DC50. Locals pos, themeCol, tga_width, tga_height, interpCol, col, y, x, theme, interpRange,
//   interpVal: Ego_d.pdb.
// Shape: FableWin 0x021886B0 statement for statement (two GetRegionMinimapSize() reads = retail's two size copies [esp+2Ch] /
//   [esp+38h]; SetR / SetG / SetB / SetA of interpCol; Interpolate; SetPixel(x, tga_height - 1 - y, col)), then SaveAsTGA and
//   ReleaseMinimapThemeData. Retail's literals are floats (FableWin: 0.5 and 256 are doubles).
// Callees: CSurface::SetPixel(long, long, const CRGBColour&) 0x009F4120, CSurface::SaveAsTGA 0x009F4590, GFGetInterpolatedColour
//   0x00A44250, __ftol2 0x00BFEA70 (the three channel conversions), free 0x00BFEA14. Data: g_RegionMinimapLightingMap 0x0137E9D8
//   (landed 0x0066CF00 spelling; the row reads .useLighting at +4).
// Retail-asm: 0x0066CE2B `fld dword ptr [esp+30h]` + 0x0066CE2F `fistp dword ptr [esp+34h]` -- engine/GFFloatToLongNear.h inside
//   CRGBColour::Interpolate. This row carries no asm of its own.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DCOORDI_INITIALISE
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_SETPIXEL_COLOUR
#define FABLE_RETAIL_CSURFACE_SAVEASTGA
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_SETCHANNEL
#define FABLE_RETAIL_CRGBCOLOUR_INTERPOLATE
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#define FABLE_RETAIL_CREGIONMINIMAP
#define FABLE_RETAIL_CREGIONMINIMAP_POPULATE
#define FABLE_RETAIL_CREGIONMINIMAP_BUILDTGA
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CMainGameComponent.h"
#include "engine/CRegionMinimap.h"

struct RegionMinimapLightingMap { CRegionMinimap::RegionMinimapTheme eTheme; bool useLighting; };
extern RegionMinimapLightingMap g_RegionMinimapLightingMap[];       // 0x0137E9D8 (descriptive name)

void CRegionMinimap::BuildRegionMinimapTGA(const CWideString& filename)
{
    long tga_width = GetRegionMinimapSize();
    long tga_height = GetRegionMinimapSize();
    C2DCoordI pos(0, 0);
    CRGBColour themeCol;
    CRGBColour interpCol;
    CRGBColour col;
    for (long y = 0; y < tga_height; ++y) {
        for (long x = 0; x < tga_width; ++x) {
            pos.Initialise(x, y);
            RegionMinimapTheme theme = GetRegionMinimapThemeValue(pos);
            themeCol = RegionMinimapThemeColour[theme];
            if (g_RegionMinimapLightingMap[theme].useLighting) {
                float interpRange = 0.8f;
                interpCol.SetR((unsigned char)(themeCol.GetR() * interpRange));
                interpCol.SetG((unsigned char)(themeCol.GetG() * interpRange));
                interpCol.SetB((unsigned char)(themeCol.GetB() * interpRange));
                interpCol.SetA(0x80);
                float interpVal = (GetRegionMinimapThemeLighting(pos) + 1.0f) * 0.5f;
                col = themeCol.Interpolate(interpCol, interpVal);
            } else {
                col = themeCol;
            }
            RegionMinimapSurface.SetPixel(x, tga_height - 1 - y, col);
        }
    }
    RegionMinimapSurface.SaveAsTGA(filename, 0, false);
    ReleaseMinimapThemeData();
}
