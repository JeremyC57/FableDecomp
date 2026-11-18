// CMap::InitPassabilityFromThemesIfNecessary(const C2DBoxI&) -- when the map still needs it, mark each height-field cell of the area passable unless its theme forbids it or the ground there is steeper than a 0.7 normal
// Leaf: ?InitPassabilityFromThemesIfNecessary@CMap@@QAEXABVC2DBoxI@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: the local C2DCoordI removed too (engine/C3DVector.h includes engine/C2DCoordI.h, Ego_r TPI 0xa59e: public long X/Y, size 8).
// Shape: same-TU out-only stand-ins for GetDef<CThemeDef> (0x0063F3A0) and CMap::GetThemeIndexAt (0x0081F910) keep the
//   cell position, the weight and the def from escaping (x in edi, one shared out/def word, the def released without a
//   re-read); the theme clears passability when it lacks flag 0 or has a positive Passability (`if (...) passable =
//   false;` -- retail keeps the cell passable for Passability <= 0); the cell pointer steps before x.
// Callees: CMap theme lookup 0x0081F910 (labelled CopyPos), CGameDefinitionManager::Get 0x0044C6B0 (__fastcall), CDefinitionManager::GetDef<T> 0x0063F3A0, CMap::GetGroundFaceNormalAt 0x0081F6C0; constants 0.0f @ 0x0122DEDC, 0.7f @ 0x01255318.
// Layout: no CMap engine header; local model (height field +0x1A7C: cells +0x10 of 12 bytes, width +0x204, height +0x208; passability flag +0x1D38; theme def flags +0xA0 bit 0, passability +0x80).
// NF49 (round 7): the def classes and the manager come from the shared headers (were local CThemeDef / CDefPointer /
// CDefinitionManager / CGameDefinitionManager models plus a same-TU stand-in GetDef body over a made-up
// FindDef_CDefinitionManager_GetDef callee): the lookup at 0x0063F3A0 is the member template
// ??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z (FableWin.pdb; the landed
// 0x0063DD70 / 0x0066CF00 include chain), with the header's same-TU body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, the
// stand-in's role). The descriptive CThemeDef is CEngineThemeDef (the template argument the FableWin symbol names); it has no
// shared header (engine/_quarantine only), so it stays modelled on the shared CDefClassBase as landed 0x0063DD70 does.
// NF70 (round 7, land1117fu q10724 / q10735): moved onto engine/CMap.h (the local CMap / CHeightField / C2DBoxI models and the same-TU
//   stand-in GetThemeIndexAt body over the made-up LookUpThemeIndex_CMap are gone). The theme lookup is the PDB
//   ?GetStrongestEngineThemeAt@CMap@@IBEJABVC2DCoordI@@AAJ@Z 0x0081F910, whose real body this TU sees through its one definition in the
//   header (FABLE_RETAIL_CMAP_GETSTRONGESTENGINETHEMEAT_BODY, `inline __declspec(noinline)`; retail compiled both in one object);
//   GetGroundFaceNormalAt is the PDB const member (was QAE). Overlay opt-in FABLE_RETAIL_CMAP_INITPASSABILITYFROMTHEMES declares both.
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAP_THEMESIZEZ
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#define FABLE_RETAIL_CMAP_ENGINEACCESSORS
#define FABLE_RETAIL_CMAP_INITPASSABILITYFROMTHEMES
#define FABLE_RETAIL_CMAP_GETSTRONGESTENGINETHEMEAT_BODY
#define FABLE_RETAIL_CHEIGHTMAP_GETCELL
#define FABLE_RETAIL_CHEIGHTMAPCELL_ACCESSORS
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/C3DVector.h"
#include "engine/CMap.h"
class CEngineThemeDef : public CDefClassBase { public:            // Ego_r.pdb CEngineThemeDef (no shared header); retail offsets
    unsigned char _pad_0x25[0x80 - 0x25];
    float         Passability;                  // +0x80
    unsigned char _pad84[0x1c];
    unsigned char Flags;                        // +0xa0 bit 0: passable
};

void CMap::InitPassabilityFromThemesIfNecessary(const C2DBoxI& area)
{
    if (!InitialisePassabilityFromThemes)
        return;
    CHeightMap* heightMap = HeightMap.get();
    C2DCoordI pos;
    for (pos.Y = area.TLY; pos.Y < area.BRY; ++pos.Y) {
        CHeightMapCell* cell = &heightMap->GetCell(C2DCoordI(area.TLX, pos.Y));
        for (pos.X = area.TLX; pos.X < area.BRX; ++cell, ++pos.X) {
            bool passable = true;
            long theme;
            {
                long strength;
                theme = GetStrongestEngineThemeAt(pos, strength);
            }
            if (theme > 0) {
                CDefPointer<const CEngineThemeDef> def;
                CGameDefinitionManager::Get().GetDef(theme, &def);
                if (!(def->Flags & 1) || def->Passability > 0.0f)
                    passable = false;
            }
            if (passable) {
                C3DVector normal = GetGroundFaceNormalAt(C3DVector((float)pos.X, (float)pos.Y, 0.0f));
                if (normal.Z < 0.7f)
                    passable = false;
            }
            cell->Passable = passable;
        }
    }
}
