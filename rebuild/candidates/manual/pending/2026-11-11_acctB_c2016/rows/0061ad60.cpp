// CTCInventoryQuests::DrawActiveTestQuests(const CEnginePrimitivePrepareInfo& info) -- the test-quest overlay of the quest screen: a four-colour full-screen backdrop, the "Test Quest Mode" title, up to 12 test quests from the top index (each in its colour, the selected one flashing white and remembered, active ones marked " [Active]", "..." above / below when the list is cut), then the selected quest's HSP name and the three button hints
// Leaf: ?DrawActiveTestQuests@CTCInventoryQuests@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0061AD60, 1631 bytes, thiscall `ret 4` (speed TU, esp frame 0x6C). Identity (two sources):
//   Ego_r.pdb 0x005987B0 ?DrawActiveTestQuests@CTCInventoryQuests@@QAEXABVCEnginePrimitivePrepareInfo@@@Z, right after
//   GetTestQuestsToDisplay 0x005982E0 in donor order (= retail 0x0061A8A0, which this row calls); its only caller is
//   DrawPreparePrimitivesForRendering (0x0061B518); the "Test Quest Mode" / "Selected HSP:" literals. Catalog name already right.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the two STLport vectors), CRenderManager2D::Draw2DBoxColours 0x009DDA50 on PRenderManager
//   0x013B8384, CDefStringTable::GetString 0x009D49B0 (the inline CDefString conversion of CInventoryDef::GenericFont), CFontManager::GetFont
//   0x009E2C80 on PFontManager 0x013B838C, NInventory::CTCInventoryBase::DrawInventoryText 0x005BCE49 / Draw2DText 0x005BCAFE (static
//   __fastcall), CWideString(const unsigned short*) 0x0099B6B0 / ~ 0x0099B510, GFACoordToVCoord 0x009E21D0, GFAYToVY 0x009E1DD0,
//   CCharString() 0x0099E4B0 / copy 0x0099EC30 / operator=(const CCharString&) 0x0099EFB0 / operator=(const char*) 0x0099EFE0 /
//   operator+=(const char*) 0x0099F100 / ToWideString 0x0099E480 / ~ 0x0099EAE0, CBasicString<char>::operator==(const char*) 0x004115A0,
//   CQuestManager::IsQuestActive 0x004AF610 on the singleton 0x013B89FC, CTCInventoryQuests::GetTestQuestsToDisplay 0x0061A8A0,
//   std::__destroy_aux<CTestQuest*> 0x004ABD90 (__fastcall).
// Data: VIRTUAL_SCREEN_WIDTH / HEIGHT 0x013961E8 / 0x013961F0 (doubles), the TU's statically initialised colours NInventory::WHITE /
//   GREY / BLACK 0x0137D700 / 0x0137D704 / 0x0137D708 (FableWin.pdb: the NInventory header constants of 148 TUs) and the three test-quest
//   colours 0x0137D70C / 0x0137D710 / 0x0137D714 (descriptive names), the flash toggle 0x013B8C84 (a function-local static bool, .bss),
//   the float 60.0f 0x0124F2FC; CInventoryDef GenericFont +0xCC, QuestCardInfoTLPos +0x7A0, QuestCardInfo2TLPos +0x7A8,
//   QuestCardInfoTextOffset +0x7BC (hdr/engine/CInventoryDef.h FULL); CTCInventoryQuests TestQuestSelectedIndex +0x158, TestQuestTopIndex
//   +0x15C (OPENPC); CTestQuest QuestName +0, HSPName +4, ColourIndex +8, DisplayName +0xC.
// Shape: the backdrop colours are a 4-element vector filled black, then three elements re-initialised in place (CRGBColour::Initialise,
//   byte stores into the buffer, 0x0061ADC9..0x0061ADF0); the title position is one C2DVector built from two GetPDef() copies (Y read
//   first: the constructor's arguments go right to left); the "..." markers are named locals (built before the argument pushes,
//   0x0061AFC2 / 0x0061B195); `selected = true` is written before the HSP-name assignment (retail stores it ahead of the call,
//   0x0061B065, and keeps it in memory); the quest counter is the loop's second increment (0x0061B182). GetTestQuestsToDisplay's body
//   (landed 0x0061A8A0) is visible and not expanded, as in retail's TU: VC7.1 then knows the local vector is not captured, keeping its
//   start in esi for the final destroy / free (0x0061B37A..0x0061B392) and the colour buffer's start in eax (0x0061AD9B) -- a
//   declaration only gives 1633v1631.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_PDEF
#define FABLE_RETAIL_CTCINVENTORYBASE_DRAWTEXT
#define FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
#define FABLE_RETAIL_CTCINVENTORYQUESTS_OPENPC
#define FABLE_RETAIL_CTCINVENTORYQUESTS_TESTQUESTS
#define FABLE_RETAIL_CINVENTORYDEF_FULL
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CTESTQUEST_NAMECOMPARE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ACTIVETESTQUESTS
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_HEIGHT
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERMANAGER2D_BOXCOLOURS
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ISQUESTACTIVE
// The overlay STLport header comes first so that its FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL branch is the one included.
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CDefString.h"
#include "engine/CInventoryDef.h"
#include "engine/CTestQuest.h"
#include "engine/CWorld.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryQuests.h"
#include "engine/CFontBank.h"
#include "engine/CFontManager.h"
#include "engine/CRenderManager.h"
#include "engine/CQuestManager.h"

// The NInventory header constants (FableWin.pdb: NInventory::`dynamic initializer for 'WHITE' / 'GREY' / 'BLACK' in each of 148 TUs;
// retail initialises them statically: no initializer code, read from .data) and this TU's three test-quest colours (no symbol survives
// in either PDB name set: descriptive names after the values). Internal linkage, as namespace-scope consts.
namespace NInventory {
const CRGBColour WHITE(255, 255, 255, 255);             // 0x0137D700
const CRGBColour GREY(125, 125, 175, 255);              // 0x0137D704
const CRGBColour BLACK(0, 0, 0, 255);                   // 0x0137D708
}
namespace {
const CRGBColour TEST_QUEST_GREEN(25, 100, 25, 255);    // 0x0137D70C
const CRGBColour TEST_QUEST_YELLOW(100, 100, 25, 255);  // 0x0137D710
const CRGBColour TEST_QUEST_RED(100, 25, 25, 255);      // 0x0137D714
}

// Established spellings (no PDB data symbols): VIRTUAL_SCREEN_WIDTH / HEIGHT, PRenderManager (the landed CPlayer::DrawFreeCamDebug
// 0x00487570). GFACoordToVCoord: Ego_r.pdb / FableWin.pdb ?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z (no shared header declares it).
extern double VIRTUAL_SCREEN_WIDTH;                     // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                    // 0x013961F0
extern CRenderManager* PRenderManager;                  // 0x013B8384
void __fastcall GFACoordToVCoord(const C2DVector& in, C2DVector& out);   // 0x009E21D0

// 0x0061A8A0 (landed), the same TU, compiled before this function: its body is visible here but not expanded (retail calls it,
// 0x0061AF99), so VC7.1 sees that it does not capture the local vector (the vector's start stays in esi for the final destroy
// and free, 0x0061B37A..0x0061B392, and the colour vector's start in eax, 0x0061AD9B).
#pragma auto_inline(off)
void CTCInventoryQuests::GetTestQuestsToDisplay(std::vector<CTestQuest>& quests)
{
    quests = PeekWorld().GetActiveTestQuests();
    std::sort(quests.begin(), quests.end(), CTestQuestNameCompare());
    if (TestQuestShowAll)
        return;
    std::vector<CTestQuest> active;
    for (unsigned long i = 0; i < quests.size(); ++i)
        if (CQuestManager::Get().IsQuestActive(quests[i].QuestName))
            active.push_back(quests[i]);
    quests = active;
}
#pragma auto_inline(on)

void CTCInventoryQuests::DrawActiveTestQuests(const CEnginePrimitivePrepareInfo& info)
{
    C2DBoxF screen(0.0f, 0.0f, (float)VIRTUAL_SCREEN_WIDTH, (float)VIRTUAL_SCREEN_HEIGHT);
    std::vector<CRGBColour> colours(4, CRGBColour(0, 0, 0));
    colours[1].Initialise(100, 0, 100, 255);
    colours[2].Initialise(0, 100, 100, 255);
    colours[3].Initialise(0, 0, 128, 255);
    PRenderManager->Draw2DBoxColours(screen, &colours[0], 0);

    const CFontBank* font = PFontManager->GetFont(GetPDef()->GenericFont).Get();
    C2DVector title_pos(GetPDef()->QuestCardInfo2TLPos.X, GetPDef()->QuestCardInfo2TLPos.Y);
    DrawInventoryText(title_pos, *font, CWideString(L"Test Quest Mode"), NInventory::TEXT_LAYOUT_LEFT, CRGBColour(0, 0, 0), false);

    C2DVector pos = GetPDef()->QuestCardInfoTLPos;
    pos.X += GetPDef()->QuestCardInfoTextOffset.X;
    pos.Y -= 60.0f;
    GFACoordToVCoord(pos, pos);

    long count = 0;
    CCharString selected_hsp;
    std::vector<CTestQuest> quests;
    GetTestQuestsToDisplay(quests);
    if (TestQuestTopIndex > 0) {
        C2DVector dots_pos(pos.X, pos.Y - GFAYToVY((float)font->GetFontHeight()));
        CWideString dots(L"...");
        Draw2DText(dots_pos, dots, *font, NInventory::WHITE, 0.8f, false);
    }
    for (std::vector<CTestQuest>::iterator it = quests.begin() + TestQuestTopIndex; it != quests.end(); ++it, ++count) {
        bool selected = false;
        if (count == 12) {
            CWideString dots(L"...");
            Draw2DText(pos, dots, *font, NInventory::WHITE, 0.8f, false);
            break;
        }
        CRGBColour colour = NInventory::WHITE;
        if (count == TestQuestSelectedIndex - TestQuestTopIndex) {
            selected = true;
            selected_hsp = it->HSPName;
        }
        switch (it->ColourIndex) {
        case 0: colour = TEST_QUEST_RED; break;
        case 1: colour = TEST_QUEST_YELLOW; break;
        case 2: colour = TEST_QUEST_GREEN; break;
        }
        if (selected) {
            static bool flash = false;                  // 0x013B8C84
            if (flash)
                colour = NInventory::WHITE;
            flash = !flash;
        }
        CCharString name = it->DisplayName;
        if (name == "")
            name = it->QuestName;
        if (CQuestManager::Get().IsQuestActive(it->QuestName))
            name += " [Active]";
        Draw2DText(pos, name.ToWideString(), *font, colour, 0.8f, false);
        pos.Y += GFAYToVY((float)(font->GetFontHeight() + 4));
    }

    title_pos.Y += (float)font->GetFontHeight() * 2.0f;
    DrawInventoryText(title_pos, *font, CWideString(L"Selected HSP:"), NInventory::TEXT_LAYOUT_LEFT, CRGBColour(0, 0, 0), false);
    if (selected_hsp.IsEmpty())
        selected_hsp = "<unspecified>";
    DrawInventoryText(C2DVector(380.0f, 370.0f), *font, CWideString(L"[X] - Activate"), NInventory::TEXT_LAYOUT_LEFT, CRGBColour(0, 0, 0), false);
    DrawInventoryText(C2DVector(380.0f, 390.0f), *font, CWideString(L"[B] - Deactivate & Complete"), NInventory::TEXT_LAYOUT_LEFT,
                      CRGBColour(0, 0, 0), false);
    DrawInventoryText(C2DVector(380.0f, 410.0f), *font, CWideString(L"[Y] - Toggle Active Quests"), NInventory::TEXT_LAYOUT_LEFT,
                      CRGBColour(0, 0, 0), false);
    title_pos.Y += (float)font->GetFontHeight();
    DrawInventoryText(title_pos, *font, selected_hsp.ToWideString(), NInventory::TEXT_LAYOUT_LEFT, CRGBColour(0, 0, 0), false);
}
