// CMap::GetStrongestEngineThemeAt(const C2DCoordI& pos, long& strength) const -- the most-weighted of a game-map cell's three engine themes
//   (the third weighs 3 - blend0 - blend1); writes the weight to `strength` and returns the theme
// Leaf: ?GetStrongestEngineThemeAt@CMap@@IBEJABVC2DCoordI@@AAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081F910, 263 bytes, `ret 8`; leaf, no calls. The two blend reads take pos by reference, the final
//   theme lookup (GetEngineThemeAtIncludingExtraCells) takes X and Y loaded before the grid pointer.
// Identity: Ego_r.pdb 0x0067A530 / FableWin.pdb 0x02230780 ?GetStrongestEngineThemeAt@CMap@@IBEJABVC2DCoordI@@AAJ@Z (protected const,
//   strength by reference); Ego_r order = retail order (0x0081F910 then GetThemeSizeZOffsetAt 0x0081FA20 / GetThemeMinCameraSizeZOffsetAt
//   0x0081FA90, its two callers, both landed on engine/CMap.h by NF63).
// NF70 (round 7, land1117fu q10724 / q10735): was catalogued CMap::GetDominantEngineThemeAt(const C2DCoordI&, long*) over local CMap /
//   CGameMapGrid / CHeightMapCell models; now the PDB member on engine/CMap.h. Its body is the header's one definition (opt-in
//   FABLE_RETAIL_CMAP_GETSTRONGESTENGINETHEMEAT_BODY, which 0x0081FA20 / 0x0081FA90 also use to see it); this row defines
//   FABLE_RETAIL_CMAP_GETSTRONGESTENGINETHEMEAT_DEFINE, so the definition is emitted here as an ordinary function.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMAP_THEMESIZEZ
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#define FABLE_RETAIL_CMAP_ENGINEACCESSORS
#define FABLE_RETAIL_CHEIGHTMAP_GETCELL
#define FABLE_RETAIL_CHEIGHTMAPCELL_ACCESSORS
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMAP_GETSTRONGESTENGINETHEMEAT_BODY
#define FABLE_RETAIL_CMAP_GETSTRONGESTENGINETHEMEAT_DEFINE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMap.h"
