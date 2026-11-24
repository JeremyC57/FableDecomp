// CMap::GetThemeMinCameraHeightAt(const C2DCoordI& pos) -- the dominant engine theme at a map cell; when it covers the cell fully (blend >= 255) the theme definition's float at +0x84 (PDB CEngineThemeDef MinCameraHeight, one dword lower in retail), else 0
// Leaf: ?GetThemeMinCameraSizeZOffsetAt@CMap@@IBEMABVC2DCoordI@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081FA90, length 103 bytes (twin of 0x0081FA20 at the neighbouring float).
// Callees: CMap::GetDominantEngineThemeAt(const C2DCoordI&, long* blend) @ 0x0081F910 (the manifest's "CopyPos" label is wrong: it returns the
//   palette's theme def index and writes the blend), CDefinitionManager::GetDef<CEngineThemeDef> @ 0x0063F3A0 (manifest label GetDef<CThingPhysicalSwitchDef>,
//   a byte-twin), the def's virtual slot 1 (self-destroy) on the last reference. Floats: 0x01230014 = 255.0f, 0x0122DEDC = 0.0f.
// Layout: CMap DefinitionManager +0x1c (retail; PDB +0x28 less the debug list/vector words).
// Tried: an inline Release() member (load/dec/store of the count), the hand `if (--def->RefCount == 0)` (this source: `dec [eax+4]` then a
//   re-read), a copied register local for the def (worse), a CDefPointer object with an unchecked release destructor (same frame).
// next: lane gap: multi-local homing into a dead argument slot.
#include "fable_compiler.h"
// NF70 (round 7, q10735): the body of 0x0081F910 GetStrongestEngineThemeAt that this TU needs visible (retail compiled both in one object) now
//   comes from its one definition in engine/CMap.h (opt-in FABLE_RETAIL_CMAP_GETSTRONGESTENGINETHEMEAT_BODY: `inline __declspec(noinline)`), not
//   from a copy in this file; the row 0x0081F910 emits it.
// NF63 (round 7): moved onto engine/CMap.h. The self is ?GetThemeMinCameraSizeZOffsetAt@CMap@@IBEMABVC2DCoordI@@@Z (protected const; Ego_r.pdb and
// FableWin.pdb, retail order after GetStrongestEngineThemeAt 0x0081F910; FABLE_RETAIL_CMAP_THEMESIZEZ in the overlay); the callee
// 0x0081F910 is ?GetStrongestEngineThemeAt@CMap@@IBEJABVC2DCoordI@@AAJ@Z (both PDBs, strength by reference; was the local
// GetDominantEngineThemeAt(const C2DCoordI&, long*) with a same-TU stand-in body). DefinitionManager is the header's
// const CGameDefinitionManager& at +0x1C; GetDef is called through const CDefinitionManager& (CGameDefinitionManager's own GetDef
// overloads hide the template). The real body of 0x0081F910 precedes it in this TU as in retail (same object, adjacent; VC7.1 does
// not inline it and, seeing it only writes through `strength`, homes the strength local in the dead argument slot); it is the
// landed 0x0081F910 row's code on the header's typed members (height map GetCell, GetEngineThemeAtIncludingExtraCells). CEngineThemeDef has no shared header (engine/_quarantine only) and stays modelled on CDefClassBase.
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMAP_THEMESIZEZ
#define FABLE_RETAIL_CMAP_GETSTRONGESTENGINETHEMEAT_BODY
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#define FABLE_RETAIL_CMAP_ENGINEACCESSORS
#define FABLE_RETAIL_CHEIGHTMAP_GETCELL
#define FABLE_RETAIL_CHEIGHTMAPCELL_ACCESSORS
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMap.h"
class CEngineThemeDef : public CDefClassBase { public:            // Ego_r.pdb CEngineThemeDef (no shared header); retail offsets
    unsigned char _pad_0x25[0x80 - 0x25];
    float Height;                                  // +0x80 (retail)
    float MinCameraHeight;                         // +0x84 (retail)
};
float CMap::GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos) const
{
    long blend;
    long themeIndex = GetStrongestEngineThemeAt(pos, blend);
    if (blend >= 255.0f) {
        CDefPointer<const CEngineThemeDef> def;
        static_cast<const CDefinitionManager&>(DefinitionManager).GetDef(themeIndex, &def);
        return def->MinCameraHeight;
    }
    return 0.0f;
}
