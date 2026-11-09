// CGameScriptInterface::SmashAllWindowsWithinRadiusOfPoint(const C3DVector& point, float radius) -- gather every thing with a smashable component in the integer box of half-size `radius` around the point's X/Y and smash each one's window component (CTCSmashableWindow::Smash, vtable slot 28)
// Leaf: ?SmashAllWindowsWithinRadiusOfPoint@CGameScriptInterface@@UAEXABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A8010, 218 bytes, `ret 8`; CGameScriptInterface vtable slot 755 (engine/CGameScriptInterface.h).
// Callees: __ftol2 0x00BFEA70 (x, y, radius); CThingSearchTools::GetAllThingsInRoughArea<CThing, vector<CThing*>, CThingFilter_HasTC>
//   0x008ADCC0 (World +4 -> PThingSearchTools +0x20); the CVectorMap LowerBound 0x0040F020 (CThing::GetTC<CTCSmashableWindow>,
//   FableWin.pdb ??$GetTC@VCTCSmashableWindow@@@CThing@@QAE_NPAPAVCTCSmashableWindow@@@Z among the script-interface instantiations,
//   0x02ACD110); CTCSmashableWindow::Smash through the vtable (+0x70); free 0x00BFEA14 (the vector storage).
// Shape: the box is the Ego_r.pdb / FableWin.pdb centre-radius constructor ??0C2DBoxI@@QAE@ABVC2DCoordI@@J@Z on a temporary
//   C2DCoordI of the two truncated coordinates (named x / y locals) with the radius truncated in the argument: the coordinates are
//   forwarded into the temporary, so VC7.1 truncates X, Y, radius in that order and computes `x + r` / `y + r` in place
//   (`add esi,eax ; add edi,eax`, 0x008A803D / 0x008A8043; with a named radius local the radius is truncated first, with a named
//   centre or the box written out `x + r` goes through `lea`). The filter's call operator is out of line in retail
//   (FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE, 0x006A80A0): the filter temporary is then visible to an unknown call and its
//   `mov [esp+38h],4Eh` store issues before the search tools load (0x008A8068). The rough-area body is visible in the TU
//   (FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY), so the result vector's first / last pointers stay in ebx / ebp across the
//   loop's virtual calls and the inline destructor frees the cached first pointer.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CWORLD
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
class CTCMapwho;                                                    // engine/CTCMapwho.h names it in NThingMap::CMapwhoNode first
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CTCSmashableBase.h"
#include "engine/CGameScriptInterface.h"
#include "stlport/fable_containers.h"

void CGameScriptInterface::SmashAllWindowsWithinRadiusOfPoint(const C3DVector& point, float radius)
{
    long x = (long)point.X;
    long y = (long)point.Y;
    C2DBoxI area(C2DCoordI(x, y), (long)radius);
    std::vector<CThing*> things;
    World->GetThingSearchTools().GetAllThingsInRoughArea<CThing>(area, CThingFilter_HasTC(CTCSmashableWindow::StatGetInterfaceType()), things);
    for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
        CTCSmashableWindow* window;
        if ((*it)->GetTC(&window))
            window->Smash();
    }
}