// NPlayerGui::CDrawQuestInfo::DrawMiniGameInfo(const CEnginePrimitivePrepareInfo& info) -- while a mini-game is shown: clear every quest-info
//   element but the mini-game bar, then draw the bar at the top-right quest-info anchor (below the clock ring while the mini-map draws),
//   its extra elements stacked under it, and for the trophy mini-game a framed box around them
// Leaf: DrawMiniGameInfo
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064D404, 399 bytes, ebp frame (`leave`), `ret 4`. Callees: the element's virtual GetType (slot 3)
//   and DrawAt (slot 0), vector<CEnginePrimitiveHandle>::erase(iterator, iterator) 0x00440D70 (the element's PrimitiveHandles.clear()),
//   CPlayerGui::PeekGuiDef 0x00438570, NPlayerGui::GFAnchorToTopRight 0x00643CA1, CPlayerGui::IsMiniMapDrawing 0x00437870, the bank's
//   GetGraphicMaxHeight (slot 24, +0x60), CDrawQuestInfo::GetElementByIndex 0x00644FC5 and CDrawQuestInfo::DrawBox 0x00643EB2.
// Name: Ego_r.pdb / FableWin.pdb ?DrawMiniGameInfo@CDrawQuestInfo@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z (header comment
//   under FABLE_RETAIL_CDRAWQUESTINFO_DRAW: the landed CPlayerGui::DrawPreparePrimitivesForEngine 0x00439070 calls it on +0x1C8).
// Shape: FableWin.pdb 0x020E86BF statement for statement (its four ASSERTs compiled out); Ego_d locals info, pminigame_element, it,
//   pclear_element, height, pbar, quest_info_pos, extra_pos, gui_def, graphic_height, i, pextra_element, box. FableWin adds the
//   double constants 16.0 / 20.0 / 12.0; retail loads the floats 16.0f (0x01238164), 20.0f (0x0122F160) and 12.0f (0x01230884).
//   The earlier parks (DIFFER 393v399) used local models and kept `height` in a frame slot.
// Lever: GetElementByIndex (0x00644FC5, landed, defined earlier in the same retail source file) has its real body here as an ordinary
//   (non-inline) definition under `#pragma auto_inline(off)`. VC7.1 then uses its register summary and keeps the drawn height in st(0)
//   across the call (0x0064D4EA); a declaration, or an in-class `__declspec(noinline)` COMDAT body, spills it to [ebp-4]. The body is
//   FableWin 0x020EC8C4 (Ego_d locals it / pelement) and grades MATCH at 0x00644FC5 from this source.
// Pragmas: `#pragma optimize("s", on)`: the HUD TU's mode. Without `optimize("y", off)`: retail's GetElementByIndex has no frame pointer
//   (`optimize("y", off)` gives both functions a frame; the row alone matches either way).
// Headers: NPlayerGui_CDrawQuestInfo.h + FABLE_RETAIL_CDRAWQUESTINFO_DRAW (the DrawMiniGameInfo declaration) and the new
//   FABLE_RETAIL_CDRAWQUESTINFO_DRAWMINIGAMEINFO (DrawBox, GetElementByIndex declarations); FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
//   (retail calls the erase COMDAT); CPlayerGuiDef QUESTINFO, CPlayerGui MINIMAPDRAWING, CGraphicDataBank GRAPHICSIZE, CDrawBase
//   SETASDRAWING, NPlayerGui ANCHORS (all landed). No layout change.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CPLAYERGUIDEF_QUESTINFO
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_MINIMAPDRAWING
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CDRAWQUESTINFO_DRAW
#define FABLE_RETAIL_CDRAWQUESTINFO_DRAWMINIGAMEINFO
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/CGraphicDataBank.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"

namespace NPlayerGui {
#pragma auto_inline(off)
CDrawQuestInfo::CQuestInfoElement* CDrawQuestInfo::GetElementByIndex(long index) const   // 0x00644FC5 (landed)
{
    for (std::list<CCountedPointer<CQuestInfoElement> >::const_iterator it = Elements.begin(); it != Elements.end(); ++it)
    {
        CQuestInfoElement* pelement = (*it).Get();
        if (pelement->ID == index)
            return pelement;
    }
    return 0;
}
#pragma auto_inline(on)

void CDrawQuestInfo::DrawMiniGameInfo(const CEnginePrimitivePrepareInfo& info)
{
    if (MiniGameElementIndex > 0)
    {
        CQuestInfoElement* pminigame_element = 0;
        for (std::list<CCountedPointer<CQuestInfoElement> >::iterator it = Elements.begin(); it != Elements.end(); ++it)
        {
            CQuestInfoElement* pclear_element = (*it).Get();
            if (pclear_element->GetType() == QUEST_INFO_ELEMENT_MINIGAME_BAR)
                pminigame_element = pclear_element;
            else
                pclear_element->PrimitiveHandles.clear();
        }
        if (pminigame_element)
        {
            SetAsDrawing(true);
            const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
            C2DVector quest_info_pos = GFAnchorToTopRight(gui_def.QuestInfoPos);
            if (PlayerGui->IsMiniMapDrawing())
            {
                long graphic_height = Bank->GetGraphicMaxHeight(gui_def.ClockRingGraphic);
                quest_info_pos.Y += graphic_height + 16.0f;
            }
            else
                quest_info_pos.Y += 20.0f;
            float height = pminigame_element->DrawAt(quest_info_pos, info);
            CQuestInfoElementMiniGameBar* pbar = static_cast<CQuestInfoElementMiniGameBar*>(pminigame_element);
            C2DVector extra_pos(quest_info_pos);
            extra_pos.X -= 12.0f;
            for (unsigned long i = 0; i < pbar->ExtraElements.size(); ++i)
            {
                extra_pos.Y += height;
                CQuestInfoElement* pextra_element = GetElementByIndex(pbar->ExtraElements[i]);
                if (pextra_element)
                    height = pextra_element->DrawAt(extra_pos, info);
            }
            if (pbar->Type == MINIGAME_TROPHY)
            {
                extra_pos.Y += height;
                C2DBoxF box(quest_info_pos, C2DExtentsF((float)gui_def.QuestInfoGraphicWidth, extra_pos.Y - quest_info_pos.Y));
                if (gui_def.QuestInfoGraphicMinHeight > box.BRY - box.TLY)
                    box.BRY = box.TLY + gui_def.QuestInfoGraphicMinHeight;
                box.TLX -= 16.0f;
                box.BRX += 16.0f;
                DrawBox(info, box);
            }
        }
    }
}
}
