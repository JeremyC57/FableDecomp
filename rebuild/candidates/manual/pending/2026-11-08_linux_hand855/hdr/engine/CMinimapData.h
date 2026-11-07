#ifndef FABLE_ENGINE_CMINIMAPDATA_H
#define FABLE_ENGINE_CMINIMAPDATA_H
// CMinimapData (chunk 855, row 0x00642AA0 PopulateMinimapThemeList). Ego_r.pdb: 0x44 bytes with debug-STL vectors; retail STLport
// 0x3c (the landed rows 0x006427F0 SetMinimapExtents, 0x00642820 LoadMinimapTGAFile, 0x00642880 InitialiseMinimapTexture,
// 0x00642970 BuildMinimapTGA, 0x00642A30 ctor model the same offsets): MinimapExtents +0x00, MinimapThemeList +0x08,
// MinimapSurface +0x14 (CSurface, 16), MinimapTexture +0x24 (CTexture, 8), MaxMinimapThemeColours +0x2c, ColourMinimapTheme
// +0x30. MinimapTheme is the PDB's nested enum (W4MinimapTheme@CMinimapData@@); its enumerators are in neither PDB name list.
// Inline accessors (Ego_r ?SetMinimapThemeValue@CMinimapData@@QAEXABVC2DCoordI@@W4MinimapTheme@1@@Z, ?PeekDefinitionManager@
// CMinimapData@@QBEABVCGameDefinitionManager@@XZ; no out-of-line copy is called by the row).
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DCoordI.h"
#include "engine/CRGBColour.h"
class CWorldMap; class CWideString; class CGameDefinitionManager;
class CMinimapData {
public:
    enum MinimapTheme { };
    const C2DExtentsI& GetMinimapExtents() const { return MinimapExtents; }
    void SetMinimapThemeValue(const C2DCoordI& position, MinimapTheme theme) { MinimapThemeList[MinimapExtents.X * position.Y + position.X] = theme; }
#ifdef FABLE_RETAIL_CMINIMAPDATA_PEEKDEFS
    const CGameDefinitionManager& PeekDefinitionManager() const { return CMainGameComponent::Get()->PeekDefinitionManager(); }
#endif
    void PopulateMinimapThemeList(const CWorldMap& worldMap, long mapNumber);              // 0x00642AA0
    void BuildMinimapTGA(const CWideString& filename);                                       // 0x00642970
    C2DExtentsI                   MinimapExtents;                        // +0x00
    std::vector<MinimapTheme>     MinimapThemeList;                      // +0x08
    unsigned char                 MinimapSurface[0x10];                  // +0x14 CSurface
    unsigned char                 MinimapTexture[0x8];                   // +0x24 CTexture
    unsigned long                 MaxMinimapThemeColours;                // +0x2c
    std::vector<CRGBColour>       ColourMinimapTheme;                    // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CMinimapData) == 0x3c);
#endif
