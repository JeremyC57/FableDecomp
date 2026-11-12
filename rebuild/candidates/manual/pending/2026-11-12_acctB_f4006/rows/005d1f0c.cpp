// CTCInventoryStats::ConstructPersonalityListPC(NUISystem::CComponent* parent) -- builds the PC (mouse) personality page of the hero stats screen
// Leaf: ?ConstructPersonalityListPC@CTCInventoryStats@@UAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D1F0C, 17,873 bytes, vtable 0x01254ADC slot 89, size-optimised TU (`optimize("s")` alone: the
//   biased frame `push ebp ; lea ebp,[esp-74h] ; sub esp,0E4h`), no EH frame, `ret 4`.
// Identity: Ego_r.pdb + FableWin.pdb ?ConstructPersonalityListPC@CTCInventoryStats@@UAEPAVCComponent@NUISystem@@PAV23@@Z; the class
//   virtual order of engine/CTCInventoryStats.h (ConstructPersonalityList 86, ConstructExperienceList 87, ConstructExperienceListPC 88
//   landed 0x005D7081, this one 89); the strings PC_STATS_PERSONALITY_LIST / PC_UI_PERSONALITY_FRAME / PC_STATS_PERSONALITY_SCREEN.
// Shape: the scroll-button actions of the landed twin 0x005D7081 (list / description / viewport buttons), then, with a screen, the
//   statistics text: per section a CreateTitlePC title and one text block of "label: value" lines (hero, tattoos, creatures killed,
//   combat, magic, romance, regions, quests, towns, pubs, crime, money, experience), then the renown / alignment tick sliders and the
//   attractiveness / scariness static sliders in the non-scrolling container. Returns the list component.
// Statement shapes the bytes decide: the chicken-kick distance and the three best times round inside their text statement
//   (`GFRoundFloatNear(x, 2)` / `GFRoundFloatNear(x, 1)`: the statement's temporaries then sit in the frame words of a helper statement),
//   the fish weight is a named float rounded in its own statement (0x005D4B6B: its text temporaries use the other frame words);
//   NUMBUILDINGS goes through the inline CTCHeroStats::GetNumberOfBuildingsOwned (0x005D4698 `lea edx,[eax+ecx]`; FITTED, see the header).
// Same-TU body: CTCInventoryStats::ConvertMap (retail 0x005CD6B7, the function retail's TU compiles before this one; Ego_r.pdb
//   ?ConvertMap@CTCInventoryStats@@QAEXABV?$map@KJ..@std@@AAV?$list@U?$pair@VCWideString@@J@std@@..@3@@Z, landed row) is defined ahead of
//   the row, out of line, over the shared headers; this file's copy grades RELOCATION_MATCH against 0x005CD6B7. With only its
//   declaration the creature-list loop entry 0x005D2F77 gives eax to the list sentinel instead of the iterator (17880 v 17873 bytes).
// Retail-asm: 0x005D4A15 / 0x005D4B8C `fistp dword ptr [ebp-20h]` (GFRoundFloatNear of the chicken kick and the fish weight),
//   0x005D50B0 / 0x005D51D3 / 0x005D52F6 `fistp dword ptr [ebp-20h]` (GFRoundFloatNear of the three best times) and 0x005D4D1A /
//   0x005D4E4F / 0x005D4F84 `fistp dword ptr [ebp-4Ch]` (GFFloatToLongFloor of the coin-golf / shove-ha'penny scores) -- the engine's
//   float-to-long helpers (shared engine/GFFloatToLongNear.h through engine/GFPackFloat.h, and engine/GFFloatToLongFloor.h: retail-asm
//   helpers with FABLE_PORTABLE twins). Per-site attempts (ASM_RULES rule 4; table: probe/asm/tbl/table.md of the hand-back): 36 plain
//   C++ + 15 intrinsic spellings of the near conversion and 32 + 14 of the floor conversion, each as a fragment on the site's input in
//   a size TU under five flag sets: `__ftol2` / `fistp qword` (/QIfist) / `cvttss2si` (/arch:SSE*) or a magic-constant `fadd`, never a
//   dword fistp. This row carries no inline assembly of its own.
// Inline-budget: GFRoundFloatNear is FABLE_FORCEINLINE in the shared engine/GFPackFloat.h (FableWin.pdb ?GFRoundFloatNear@@YIMMJ@Z; retail
//   expands it at all five sites, e.g. 0x005D49FA `push 2 ; fld1 ; pop eax ; dec eax ; fmul 10.0f ; jne`). c2 inline trace of this row
//   with the helper as plain `inline`: callee size 48, penalty 0, static frequency 13.5 at each of the five sites, refused with no
//   score (the size-TU limit of 15), so no /d2inlT reaches it. FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ (existing shared opt-in) serves
//   the ConvertMap body only: its plain CWideString::operator== is refused the same way (size 40, frequency 25, no score) where
//   retail expands it (0x005CD6EB `mov eax,[esi+8] ; cmp eax,[ebp-4] ; je` .. the compare call 0x005CD701).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYSTATS
#define FABLE_RETAIL_CTCINVENTORYSTATS_PERSONALITY
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ISTROPHY
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CTCHERO_TITLEGUITEXT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CMANAGER_STATICSLIDERBIDIRECTIONAL
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_HEROSTATS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
enum EOpinion { OPINION_MORALITY, OPINION_RENOWN, OPINION_SCARINESS, OPINION_AGREEABLENESS, OPINION_ATTRACTIVENESS };
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroMorph.h"
#include "engine/CTCHero.h"
#include "engine/C2DCoordI.h"
#include "engine/CRGBFloatColour.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/CCountedPointer.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CWorld.h"
#include "engine/CGameTimeManager.h"
#include "engine/COpinionReactionManager.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFPackFloat.h"

using NUISystem::CComponent;
using NUISystem::CAction;
using NUISystem::CActionParamULONG;
using NUISystem::CActionParamSLONG;

CWideString FABLE_FASTCALL operator+(const CWideString& left, const CWideString& right);     // 0x0099BE70
CWideString FABLE_FASTCALL operator+(const CWideString& left, const unsigned short* right);         // 0x0099BF30
CWideString FABLE_FASTCALL GFIntToWideString(long value);                                    // 0x00405F00
CWideString FABLE_FASTCALL GFFloatToWideString(double value);                                // 0x00486C40
namespace NGameText {
bool FABLE_FASTCALL ConstructString(CWideString& text, const std::vector<CWideString>& arguments);   // 0x009C7C20
}

// Same-TU body of CTCInventoryStats::ConvertMap (retail 0x005CD6B7, 176 bytes; compiled before this row in retail's TU): folds the
// creatures-killed map (creature group -> count) into a list of (group name, count) pairs. The landed row's statements over the
// shared headers; out of line here (callee size > 15 in a size TU).
void CTCInventoryStats::ConvertMap(const std::map<unsigned long, long>& creatures, std::list<std::pair<CWideString, long> >& out)
{
    for (std::map<unsigned long, long>::const_iterator it = creatures.begin(); it != creatures.end(); ++it) {
        CWideString name = GetCreatureGroupAsString(it->first);
        std::list<std::pair<CWideString, long> >::iterator end = out.end();
        bool found = false;
        for (std::list<std::pair<CWideString, long> >::iterator li = out.begin(); li != end; ++li) {
            if (li->first == name) {
                li->second += it->second;
                found = true;
                break;
            }
        }
        if (!found)
            out.push_back(std::pair<CWideString, long>(name, it->second));
    }
}

CComponent* CTCInventoryStats::ConstructPersonalityListPC(CComponent* parent)
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    CWideString newLine(L"\n");
    CWideString colon(L": ");
    CWideString empty(L"");
    CWideString space(L" ");
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CTCHeroExperience* heroExperience;
    ParentThing->GetTC(TCI_HERO_EXPERIENCE, &heroExperience);

    CComponent* list = manager->CreateComponent("PC_STATS_PERSONALITY_LIST", false);
    CComponent* frame = list->FindChild("PC_UI_PERSONALITY_FRAME");
    CComponent* scrollListUp = parent->FindChild("SCROLL_LIST_UP");
    CComponent* scrollListDown = parent->FindChild("SCROLL_LIST_DOWN");
    {
        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(scrollListUp, observeUp);
        observeUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(scrollListUp, ignoreUp);
        ignoreUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(ignoreUp);
        list->AddActionOnInactive(observeUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(scrollListDown, observeDown);
        observeDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(scrollListDown, ignoreDown);
        ignoreDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(ignoreDown);
        list->AddActionOnInactive(observeDown);
    }
    CComponent* scrollDescriptionUp = parent->FindChild("SCROLL_DESCRIPTION_UP");
    CComponent* scrollDescriptionDown = parent->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (scrollDescriptionUp) {
        CCountedPointer<CAction> observe(new CAction);
        observe->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observe->AddParam(scrollDescriptionUp, observe);
        observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignore(new CAction);
        ignore->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignore->AddParam(scrollDescriptionUp, ignore);
        ignore->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(ignore);
        list->AddActionOnInactive(observe);
    }
    if (scrollDescriptionDown) {
        CCountedPointer<CAction> observe(new CAction);
        observe->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observe->AddParam(scrollDescriptionDown, observe);
        observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignore(new CAction);
        ignore->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignore->AddParam(scrollDescriptionDown, ignore);
        ignore->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(ignore);
        list->AddActionOnInactive(observe);
    }
    if (frame) {
        CComponent* viewportUp = frame->FindChild("SCROLL_STATS_PERSONALITY_VIEWPORT_UP");
        CComponent* viewportDown = frame->FindChild("SCROLL_STATS_PERSONALITY_VIEWPORT_DOWN");
        CComponent* viewport = list->FindChild("PC_STATS_PERSONALITY_VIEWPORT");

        CCountedPointer<CAction> sendUp(new CAction);
        sendUp->ActionType = NUISystem::UI_ACTION_TYPE_SEND_OWNED_EVENT;
        sendUp->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_RIGHT_STICK_UP));
        sendUp->AddParam(viewport, sendUp);
        viewportUp->AddActionOnLeftClicked(sendUp);
        viewportUp->AddActionOnLeftHeld(sendUp);
        CCountedPointer<CAction> sendDown(new CAction);
        sendDown->ActionType = NUISystem::UI_ACTION_TYPE_SEND_OWNED_EVENT;
        sendDown->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_RIGHT_STICK_DOWN));
        sendDown->AddParam(viewport, sendDown);
        viewportDown->AddActionOnLeftClicked(sendDown);
        viewportDown->AddActionOnLeftHeld(sendDown);

        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(viewportUp, observeUp);
        observeUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(viewportUp, ignoreUp);
        ignoreUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observeUp);
        list->AddActionOnInactive(ignoreUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(viewportDown, observeDown);
        observeDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(viewportDown, ignoreDown);
        ignoreDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observeDown);
        list->AddActionOnInactive(ignoreDown);
    }

    CComponent* screen = list->FindChild("PC_STATS_PERSONALITY_SCREEN");
    if (screen) {
        frame->AddPositionChild(screen);
        CTCHeroMorph* heroMorph;
        ParentThing->GetTC(&heroMorph);
        CTCHeroStats* heroStats;
        ParentThing->GetTC(&heroStats);
        heroStats->UpdateAppearanceStats(true);
        CComponent* container = screen->FindChild("PC_PERSONALITY_COMPONENT_CONTAINER");
        CWideString text;
        CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("HERO")), container, true);
        CTCHero* hero;
        ParentThing->GetTC(&hero);
        CWideString title = hero->GetTitleGuiText();
        std::vector<CWideString> titleParts;
        titleParts.push_back(BuildRenownString());
        titleParts.push_back(BuildHeroClass());
        if (title.GetLength() == 0) {
            titleParts.push_back(CWideString::NullString);
            titleParts.push_back(CWideString::NullString);
        } else {
            titleParts.push_back(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CALLED")));
            titleParts.push_back(title);
        }
        CWideString heroTitle;
        heroTitle = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TITLE_BUILDER"));
        NGameText::ConstructString(heroTitle, titleParts);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TITLE")) + colon + heroTitle + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("AGE")) + colon + GFIntToWideString((long)heroStats->GetAge()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("BODY_WEIGHT")) + colon + GetFatnessAsString(heroStats->GetFatness()) + newLine;
        text += GetTimeSpentInGame(CWorld::GetFrame(), 0) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMDAYS")) + colon + GFIntToWideString(PeekWorld().PeekGameTimeManager().GetDayCount()) + newLine + newLine;
        CreateTitlePC(text, container, false);
        std::vector<CWideString> tattooNames;
        heroMorph->GetTattooNames(tattooNames);
        if (tattooNames.size() != 0) {
            CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TATTOOS")), container, true);
            text = empty;
            for (unsigned long i = 0; i != tattooNames.size(); ++i) {
                text += tattooNames[i];
                text += newLine;
            }
            text += newLine;
            CreateTitlePC(text, container, false);
        }
        if (CreaturesKilled.size() != 0) {
            CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CREATURESKILLED")), container, true);
            text = empty;
            std::list<std::pair<CWideString, long> > creatures;
            ConvertMap(CreaturesKilled, creatures);
            std::list<std::pair<CWideString, long> >::iterator it;
            for (it = creatures.begin(); it != creatures.end(); ++it)
                text += it->first + colon + GFIntToWideString(it->second) + newLine;
            text += newLine;
            CreateTitlePC(text, container, false);
        }
        CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("COMBAT")), container, true);
        text = empty;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FAVWEAPON")) + colon + heroStats->GetFavouriteWeapon(0) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FAVMELWEAPON")) + colon + heroStats->GetFavouriteWeapon(1) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FAVRANGEWEAPON")) + colon + heroStats->GetFavouriteWeapon(2) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("DECAPITATIONS")) + colon + GFIntToWideString(heroStats->GetNumberOfDecapitations()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MAXMULTIPLIER")) + colon + GFIntToWideString(heroStats->GetMaximumMultiplier()) + newLine;
        text += newLine;
        CreateTitlePC(text, container, false);
        CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MAGIC")), container, true);
        long favouriteAbility = heroStats->GetFavouriteAbility();
        CTCInventoryExperience* inventoryExperience;
        ParentThing->GetTC(TCI_INVENTORY_EXPERIENCE, &inventoryExperience);
        CWideString abilityName;
        if (favouriteAbility == -1)
            abilityName = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NONE"));
        else
            abilityName = inventoryExperience->GetExperienceAbilityName(favouriteAbility);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FAVABILITY")) + colon + abilityName + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMCASTS")) + colon + GFIntToWideString(heroStats->GetNumberOfAbilityCasts()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMABILITIES")) + colon + GFIntToWideString(heroStats->GetNumberOfAvailableAbilities()) + newLine + newLine;
        CreateTitlePC(text, container, false);
        CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("ROMANCE")), container, true);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SEXUALITY")) + colon + GetHeroSexualityAsString(heroStats->GetHeroSexuality()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMSPOUSES")) + colon + GFIntToWideString(heroStats->GetNumberOfCurrentSpouses()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMINLOVE")) + colon + GFIntToWideString(heroStats->GetNumberOfLoversAcquired()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMWEDDINGS")) + colon + GFIntToWideString(heroStats->GetNumberOfWeddings()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMDIVORCES")) + colon + GFIntToWideString(heroStats->GetNumberOfDivorces()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMMURDEREDSPOUSES")) + colon + GFIntToWideString(heroStats->GetNumberOfSpousesMurdered()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMTIMESHADSEX")) + colon + GFIntToWideString(heroStats->GetNumberOfTimesHadSex()) + newLine + newLine;
        CreateTitlePC(text, container, false);
        CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("REGIONS")), container, true);
        long favouriteRegion;
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FAVREGION")) + colon + heroStats->GetFavouriteRegion(favouriteRegion) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPLORED_REGIONS")) + colon + GFIntToWideString(heroStats->GetNumRegionsExplored()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUM_TELEPORTS")) + colon + GFIntToWideString(heroStats->GetNumTeleports()) + newLine + newLine;
        CreateTitlePC(text, container, false);
        CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("QUESTS")), container, true);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("QUESTS_COMPLETED")) + colon + GFIntToWideString(heroStats->GetNumCompletedAQuest()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("QUESTREWARD")) + colon + GFIntToWideString(heroStats->GetRewardMoney()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMCOREQUESTS")) + colon + GFIntToWideString(heroStats->GetNumCompletedACoreQuest()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMOPTQUESTS")) + colon + GFIntToWideString(heroStats->GetNumCompletedAnOptionalQuest()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("BOASTS")) + colon + GFIntToWideString(heroStats->GetNumBoastsCompleted()) + CWideString(L"/") + GFIntToWideString(heroStats->GetNumBoastsTaken()) + newLine;
        long numTrophies = 0;
        const CTCInventory* inventory;
        if (ParentThing->PeekTC(&inventory)) {
            std::vector<std::pair<long, long> > items;
            inventory->GetAllItems(items);
            for (unsigned long i = 0; i < items.size(); ++i) {
                if (CTCInventoryItem::IsTrophy(items[i].first))
                    numTrophies += items[i].second;
            }
            text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TROPHIES")) + colon + GFIntToWideString(heroStats->GetNumMountedTrophies() + numTrophies) + newLine;
        }
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TROPHIESUSED")) + colon + GFIntToWideString(heroStats->GetNumTrophyGamePlayed()) + newLine;
        if (heroStats->GetNumTrophyGamePlayed() > 0)
            text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("BESTTROPHY")) + colon + GFIntToWideString(heroStats->GetBestTrophyWitnesses()) + newLine;
        text += newLine;
        CreateTitlePC(text, container, false);
        CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TOWNS")), container, true);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMBUILDINGS")) + colon + GFIntToWideString(heroStats->GetNumberOfBuildingsOwned()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMHOUSES")) + colon + GFIntToWideString(heroStats->GetNumberOfHousesOwned()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMSHOPS")) + colon + GFIntToWideString(heroStats->GetNumberOfShopsOwned()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMRENTS")) + colon + GFIntToWideString(heroStats->GetNumberOfBuildingsRented()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TOTALRENT")) + colon + GFIntToWideString(heroStats->GetRentMoney()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CHICKENKICK")) + colon + GFFloatToWideString(GFRoundFloatNear(heroStats->GetMaxChickenThrow(), 2)) + L" " + bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("METRES")) + newLine;
        CWideString grams = bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_GRAMS");
        float biggestFish = GFRoundFloatNear(heroStats->GetBiggestFish(), 2);
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("FISHING")) + colon + GFFloatToWideString(biggestFish) + L" " + grams + newLine + newLine;
        CreateTitlePC(text, container, false);
        CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("PUBS")), container, true);
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("COINGOLFOV")) + colon + GFIntToWideString(GFFloatToLongFloor(heroStats->GetBestScoreCoinGolfOakVale())) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_STROKES") + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("COINGOLFSS")) + colon + GFIntToWideString(GFFloatToLongFloor(heroStats->GetBestScoreCoinGolfSnowSpire())) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_STROKES") + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SHOVEHAPENNY")) + colon + GFIntToWideString(GFFloatToLongFloor(heroStats->GetBestScoreShoveHaPenny())) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_POINTS") + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("GUESS")) + colon + GFFloatToWideString(GFRoundFloatNear(heroStats->GetBestTimeGuessTheAddition(), 1)) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_SECONDS") + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("PAIRS")) + colon + GFFloatToWideString(GFRoundFloatNear(heroStats->GetBestTimePairs(), 1)) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_SECONDS") + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("SORTING")) + colon + GFFloatToWideString(GFRoundFloatNear(heroStats->GetBestTimeSorting(), 1)) + L" " + bank.GetTextBySymbol("TEXT_GUI_SCOREBOARD_UNITS_SECONDS") + newLine;
        text += newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("VOMITS")) + colon + GFIntToWideString(heroStats->GetNumberOfVomits()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("DRINKS")) + colon + GFIntToWideString(heroStats->GetNumberOfDrinks()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("DRINKERCLASS")) + colon + heroStats->GetDrinkResistance() + newLine + newLine;
        CreateTitlePC(text, container, false);
        CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRIME")), container, true);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("NUMCRIMES")) + colon + GFIntToWideString(heroStats->GetNumberOfCrimes()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TOTALFINES")) + colon + GFIntToWideString(heroStats->GetTotalFine()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TOTALFINESPAID")) + colon + GFIntToWideString(heroStats->GetFinePaid()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("THROWNOUT")) + colon + GFIntToWideString(heroStats->GetTimesThrownOut()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("TOTALBRIBE")) + colon + GFIntToWideString(heroStats->GetTotalBribeAmount()) + newLine + newLine;
        CreateTitlePC(text, container, false);
        CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MONEY")), container, true);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MONEYMAX")) + colon + GFIntToWideString(heroStats->GetMaxMoney()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MONEYEARNED")) + colon + GFIntToWideString(heroStats->GetTotalMoneyEarned()) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("MONEYSPENT")) + colon + GFIntToWideString(heroStats->GetTotalMoneySpent()) + newLine + newLine;
        CreateTitlePC(text, container, false);
        CreateTitlePC(bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPER")), container, true);
        long totalExperience = heroStats->GetHeroStatExperience(NUMBER_OF_HERO_STAT_CATEGORIES) + heroStats->GetHeroStatExperience(HERO_STAT_WILL) + heroStats->GetHeroStatExperience(HERO_STAT_SKILL) + heroStats->GetHeroStatExperience(HERO_STAT_STRENGTH);
        text = bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERTOTAL")) + colon + GFIntToWideString(totalExperience) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERGENERAL")) + colon + GFIntToWideString(heroStats->GetHeroStatExperience(NUMBER_OF_HERO_STAT_CATEGORIES)) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERSTRENGTH")) + colon + GFIntToWideString(heroStats->GetHeroStatExperience(HERO_STAT_STRENGTH)) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERSKILL")) + colon + GFIntToWideString(heroStats->GetHeroStatExperience(HERO_STAT_SKILL)) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERWILL")) + colon + GFIntToWideString(heroStats->GetHeroStatExperience(HERO_STAT_WILL)) + newLine;
        text += bank.GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("EXPERSPENT")) + colon + GFIntToWideString(heroStats->GetSpentExperience()) + newLine + newLine;
        CreateTitlePC(text, container, false);
        CComponent* sliderContainer = screen->FindChild("PC_STATS_EXP_STATIC_SLIDER_CONTAINER_NOT_SCROLLING");
        if (sliderContainer) {
            CCountedPointer<CComponent> slider;
            float percentage = heroStats->GetPercentageThroughRenownLevel();
            long renownLevel = heroStats->GetRenownLevel();
            slider.Reset(manager->CreateTickSlider(bank.GetTextBySymbol("TEXT_GUI_STATS_LABEL_RENOWN"), C2DCoordI(5, 0), heroStats->GetRenownLevel(), heroStats->GetRenownLevelMaxLevel(),
                                                   CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f), (float)renownLevel + percentage, 275.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 0.0f));
            sliderContainer->AddChild(slider);
            float goodness = GetWorld().GetOpinionReactionManager().GetHeroStatsInOpinionFormat(OPINION_MORALITY);
            CRGBFloatColour colour(0.5f, 0.1f, 0.1f, 1.0f);
            if (goodness >= 0.0f)
                colour = CRGBFloatColour(0.8f, 0.8f, 0.8f, 1.0f);
            slider.Reset(manager->CreateTickSlider(bank.GetTextBySymbol("TEXT_GUI_STATS_LABEL_ALIGNMENT"), C2DCoordI(5, 30), 0, 14,
                                                   colour, goodness * 7.0f, 285.0f, true, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 0.0f));
            sliderContainer->AddChild(slider);
            float attractiveness = GetWorld().GetOpinionReactionManager().GetHeroStatsInOpinionFormat(OPINION_ATTRACTIVENESS);
            slider.Reset(manager->CreateStaticSliderBidirectional(bank.GetTextBySymbol("TEXT_GUI_STATS_LABEL_ATTRACTIVENESS"), attractiveness * 100.0f, C2DCoordI(285, 0)));
            sliderContainer->AddChild(slider);
            float scariness = GetWorld().GetOpinionReactionManager().GetHeroStatsInOpinionFormat(OPINION_SCARINESS);
            slider.Reset(manager->CreateStaticSliderBidirectional(bank.GetTextBySymbol("TEXT_GUI_STATS_LABEL_SCARINESS"), scariness * 100.0f, C2DCoordI(285, 30)));
            sliderContainer->AddChild(slider);
        }
    }
    return list;
}
