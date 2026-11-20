// CRegionMinimap::LoadRegionMinimapTGAFile  0x0066ca80
// Naming (NF142, 2026-11-20, q4295): the Temp / SubObj stand-ins replaced by the shared classes they stood for: Temp (FormatIndex -1, then
//   Init(8,8,8,8,1) at 0x009E3790) is engine/CPixelFormat.h (default constructor, then ?Initialise@CPixelFormat@@QAEXJJJJ_N@Z: Ego_r.pdb
//   0x00B35630 + FableWin.pdb 0x0308D290), and SubObj at +0x24 is the CSurface RegionMinimapSurface (Ego_r.pdb CRegionMinimap +0x24) of
//   engine/CSurface.h: Build 0x009F3230 = ?InitialiseAsImage@CSurface@@QAE_NABVC2DExtentsI@@ABVCPixelFormat@@_N@Z and Finish 0x009F2DD0 =
//   ?LoadFromBitmapFileToPreallocatedSurface@CSurface@@QAE_NABVCWideString@@K@Z (the spellings landed rows already bind); the extents pair
//   is engine/C2DExtentsI.h. The row's existing local CRegionMinimap / CWideString models are unchanged otherwise.
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_LOADGRAPHICFRAME
#define FABLE_RETAIL_CSURFACE_BITMAP
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CSurface.h"
class CWideString { public:
    bool Check();               // 0x999230 : thiscall bool()
};

struct CRegionMinimap {
    int    RegionMinimapSize;            // +0x00
    char   gap[0x24-4];
    CSurface sub;               // +0x24
    void LoadRegionMinimapTGAFile(CWideString* ws);
};

void CRegionMinimap::LoadRegionMinimapTGAFile(CWideString* ws)
{
    CPixelFormat t;
    t.Initialise(8, 8, 8, 8, true);

    this->sub.InitialiseAsImage(C2DExtentsI(this->RegionMinimapSize, this->RegionMinimapSize), t, false);

    if (ws->Check()) {
        this->sub.LoadFromBitmapFileToPreallocatedSurface(*ws, 1);
    }
}
