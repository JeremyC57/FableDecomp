// NPlayerGui::CDrawQuestInfo::DrawMiniGameInfo(const CEnginePrimitivePrepareInfo& info)
#pragma optimize("y", off)
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
#define FABLE_RETAIL_CDRAWQUESTINFO_GETELEMENTBYINDEX_BODY
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/CGraphicDataBank.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"

namespace NPlayerGui {
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
