// CRegionMinimap::LoadRegionMinimapTGAFile(const CWideString& filename) -- size the region minimap surface as a square RegionMinimapSize image in 8.8.8.8 linear format, then load the TGA into it if the file exists
// Leaf: ?LoadRegionMinimapTGAFile@CRegionMinimap@@QAEXABVCWideString@@@Z
// Naming (NF142, 2026-11-20, q4295): the Temp / SubObj stand-ins replaced by the shared classes they stood for: Temp (FormatIndex -1, then
//   Init(8,8,8,8,1) at 0x009E3790) is engine/CPixelFormat.h (default constructor, then ?Initialise@CPixelFormat@@QAEXJJJJ_N@Z: Ego_r.pdb
//   0x00B35630 + FableWin.pdb 0x0308D290), and SubObj at +0x24 is the CSurface RegionMinimapSurface (Ego_r.pdb CRegionMinimap +0x24) of
//   engine/CSurface.h: Build 0x009F3230 = ?InitialiseAsImage@CSurface@@QAE_NABVC2DExtentsI@@ABVCPixelFormat@@_N@Z and Finish 0x009F2DD0 =
//   ?LoadFromBitmapFileToPreallocatedSurface@CSurface@@QAE_NABVCWideString@@K@Z (the spellings landed rows already bind); the extents pair
//   is engine/C2DExtentsI.h. The row's existing local CRegionMinimap / CWideString models are unchanged otherwise.
// Identity: Ego_r.pdb 0x006A1770 + FableWin.pdb 0x02188CF0 ?LoadRegionMinimapTGAFile@CRegionMinimap@@QAEXABVCWideString@@@Z (the path by
//   reference); the landed caller 0x00829D90 binds this spelling.
// Ported (NF205, 2026-11-20, q11862) onto engine/CRegionMinimap.h (the retail branch with the typed CSurface member, NF205 declaration opt-in
//   FABLE_RETAIL_CREGIONMINIMAP_LOADTGAFILE) and fable_string.h's CWideString; was a local CRegionMinimap model defining the pointer form
//   ?LoadRegionMinimapTGAFile@CRegionMinimap@@QAEXPAVCWideString@@@Z.
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
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_LOADGRAPHICFRAME
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#define FABLE_RETAIL_CREGIONMINIMAP
#define FABLE_RETAIL_CREGIONMINIMAP_POPULATE
#define FABLE_RETAIL_CREGIONMINIMAP_BUILDTGA
#define FABLE_RETAIL_CREGIONMINIMAP_LOADTGAFILE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CMainGameComponent.h"
#include "engine/CRegionMinimap.h"
#include "engine/CAFile.h"   // CAFile::PathExists(const CWideString&) 0x00999230 (Ego_r + FableWin, static __fastcall)

void CRegionMinimap::LoadRegionMinimapTGAFile(const CWideString& filename)
{
    CPixelFormat t;
    t.Initialise(8, 8, 8, 8, true);

    RegionMinimapSurface.InitialiseAsImage(C2DExtentsI(RegionMinimapSize, RegionMinimapSize), t, false);

    if (CAFile::PathExists(filename)) {
        RegionMinimapSurface.LoadFromBitmapFileToPreallocatedSurface(filename, 1);
    }
}
