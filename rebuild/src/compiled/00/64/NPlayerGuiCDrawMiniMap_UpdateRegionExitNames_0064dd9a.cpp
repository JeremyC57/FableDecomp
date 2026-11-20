// NPlayerGui::CDrawMiniMap::UpdateRegionExitNames(const CRegion& region) -- rebuild the mini map's region-exit labels: every active region exit and every scripted-hook entrance on a map of this region, at its position (+ the region's text offset for that exit), with its display name
// Leaf: ?UpdateRegionExitNames@CDrawMiniMap@NPlayerGui@@IAEXABVCRegion@@@Z
// Flags: none
// Inline-budget: header devices only, each retail's own out-of-line COMDAT with its body visible to the TU -- PeekTCDriver<const
//   CTCDRegionExit> plain inline (FABLE_RETAIL_CTHING_PEEKTCDRIVER_PLAIN_INLINE; retail calls 0x00617D30, Ego_r.pdb has the COMDAT),
//   the _List_base ctor / dtor (FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE: calls 0x00615520 / 0x00615540, the allocator temporary
//   homed in the argument byte [ebp+0Bh]), the GetAllThings bodies (FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY: calls 0x00617CA0 /
//   0x00618E60), and the visible filter call operator (FABLE_RETAIL_CTHINGFILTER_HASTCOFLEAFTYPE_BODY: declared only, the empty filter
//   temporary takes its own frame word, frame 0x50 v 0x4C; stub-body probe and real body both match).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064DD9A, 580 bytes, `ret 4` (size-optimised NPlayerGui TU, player_gui_draw_modules.obj).
// Identity: Ego_r.pdb / FableWin.pdb ?UpdateRegionExitNames@CDrawMiniMap@NPlayerGui@@IAEXABVCRegion@@@Z (protected); called by the
//   landed SetRegion 0x0064DFDE. Locals and scopes: Ego_d.pdb.
// Callees: vector<pair<C3DVector,CCharString> >::erase 0x00656837 / push_back 0x00656EF2, CWorld::PeekWorldMap (slot 13),
//   CWorldMap::GetRegionNumberFromName 0x004FC210 / GetRegionNumberMapIsIn 0x004FC190, list<const CThing*> base ctor 0x00615520 /
//   dtor 0x00615540, GetAllThings<CThing,list<const CThing*>,CThingFilter_HasTC> 0x00617CA0 and
//   <..,CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook> > 0x00618E60, CThing::PeekTCDriver<const CTCDRegionExit> 0x00617D30,
//   CTCDRegionExit::GetRegionName 0x007791F0 / GetDisplayName 0x00779260, CThing::GetPos 0x004C73D0,
//   CRegion::GetMiniMapRegionExitTextOffset 0x006BC350, pair ctor 0x0065470C, ~CCharString 0x0099EAE0, bitset test 0x00410DE0,
//   const CVectorMap::LowerBound 0x004365B0, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CTCActionUseScriptedHook::GetRegionEntranceName 0x00729C50 / GetRegionEntranceDisplayName 0x00729CA0.
// Shape: the TU-mates' size optimisation (`#pragma optimize("s", on)`, as the landed SetRegion / Update); Ego_d's locals in Ego_d's
//   scopes; both lists live to the end of the function (destroyed objects_list first, 0x0064DFC7); the region exit's name is asked
//   before the thing's position (call order 0x0064DE3A / 0x0064DE45); the scripted hook through the expanded PeekTC
//   (FABLE_RETAIL_CTHING_GETTC_EXPANDED: key 0xC2 in edi for HasTC and the LowerBound key) and its pointer read even when absent,
//   as retail does (0x0064DF36).
// Ego_d line table (C13 lines, carved): every statement in the text's order and scopes (world_map 1184 before region_number 1185;
//   exit_name 1202 before exit_pos 1204). Unreproduced: lines 1198 (474 B) and 1229 (481 B), each around the PeekTCDriver / PeekTC call,
//   are far larger than the call (retail keeps only the call and ignores its result); not written.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_MINIMAPREGIONEXITTEXTOFFSET
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_PEEKTCDRIVER_PLAIN_INLINE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCOFLEAFTYPE
#define FABLE_RETAIL_CTHINGFILTER_HASTCOFLEAFTYPE_BODY
#define FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_STATCLASSTYPENAME
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_ISUSABLE
#define FABLE_RETAIL_CTCDREGIONEXIT_MINIMAPLABEL
#define FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_REGIONEXITS
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CThing.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_HasTCOfLeafType.h"
#include "engine/CTCDRegionExit.h"
#include "engine/CTCActionUseScriptedHook.h"
#include "engine/CDrawMiniMap.h"

void NPlayerGui::CDrawMiniMap::UpdateRegionExitNames(const CRegion& region)
{
    RegionExits.clear();
    const CWorldMap& world_map = World->PeekWorldMap();
    long region_number = world_map.GetRegionNumberFromName(region.GetName());
    std::list<const CThing*> exit_list;
    World->GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTC(CTCDRegionExit::StatGetInterfaceType()), exit_list);
    for (std::list<const CThing*>::const_iterator it = exit_list.begin(); it != exit_list.end(); ++it)
    {
        const CThing* pthing = *it;
        if (world_map.GetRegionNumberMapIsIn(pthing->GetCurrentMapNumber()) == region_number)
        {
            const CTCDRegionExit* ptcd_region_exit;
            pthing->PeekTCDriver(&ptcd_region_exit);
            if (ptcd_region_exit->IsActive())
            {
                const CCharString& exit_name = ptcd_region_exit->GetRegionName();
                C3DVector exit_pos = pthing->GetPos();
                C2DVector text_offset;
                if (region.GetMiniMapRegionExitTextOffset(exit_name, text_offset))
                {
                    exit_pos.X += text_offset.X;
                    exit_pos.Y += text_offset.Y;
                }
                const CCharString& display_name = ptcd_region_exit->GetDisplayName();
                RegionExits.push_back(std::pair<C3DVector, CCharString>(exit_pos, display_name));
            }
        }
    }
    std::list<const CThing*> objects_list;
    World->GetThingSearchTools().GetAllThings<CThing>(CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>(), objects_list);
    for (std::list<const CThing*>::const_iterator it = objects_list.begin(); it != objects_list.end(); ++it)
    {
        const CThing* pthing = *it;
        if (world_map.GetRegionNumberMapIsIn(pthing->GetCurrentMapNumber()) == region_number)
        {
            const CTCActionUseScriptedHook* ptc_scripted_hook;
            pthing->PeekTC(&ptc_scripted_hook);
            if (ptc_scripted_hook->IsUsable())
            {
                CThing* pentrance = ptc_scripted_hook->GetPEntranceConnectedTo();
                if (pentrance)
                {
                    const CCharString& exit_name = ptc_scripted_hook->GetRegionEntranceName();
                    C3DVector exit_pos = pthing->GetPos();
                    C2DVector text_offset;
                    if (region.GetMiniMapRegionExitTextOffset(exit_name, text_offset))
                    {
                        exit_pos.X += text_offset.X;
                        exit_pos.Y += text_offset.Y;
                    }
                    const CCharString& display_name = ptc_scripted_hook->GetRegionEntranceDisplayName();
                    RegionExits.push_back(std::pair<C3DVector, CCharString>(exit_pos, display_name));
                }
            }
        }
    }
}