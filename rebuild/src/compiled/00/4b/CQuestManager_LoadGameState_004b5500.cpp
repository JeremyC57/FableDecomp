// CQuestManager::LoadGameState(CStringParser& parser) -- reload the quest manager's text game state: clear every quest list, then parse the quest-card types, the active and completed quest cards (by thing UID), the saved quest persistence script, the active quests (activated together), the creature-generation-disabled and between-screens quest names and the between-screens quest count
// Leaf: ?LoadGameState@CQuestManager@@QAEXAAVCStringParser@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B5500, 2525 bytes, `ret 4` (one ret). Called by the landed
// CQuestManager::LoadGameState(CPersistContext&) 0x004B6380 (the text reader of SaveGameState 0x004B64E0).
// Callees: the _List_base::clear instantiations 0x004B8050 (list<CIntelligentPointer<CThing> >), 0x004B8B70
// (list<pair<CCharString, unsigned long> >), 0x004B8BB0 (list<CQuestManager::CQuestCard>), 0x004B8C00 (list<CBoast>);
// vector<CCharString>::erase(first, last) 0x0043336A and _M_insert_overflow 0x00433530; CWorld::GetFrame 0x0049D870;
// CStringParser SkipPastString 0x009B9BA0, SkipWhitespace 0x009B9C60, SkipPastNextItemIfIs 0x009B9E00,
// ReadNextItemAsQuotedString 0x009BA1D0, ReadAsStringUntilString 0x009BA330, ReadAsStringUntilWhitespace 0x009BA4A0,
// ReadAsInteger 0x009BA540, ReadAsUHUGE 0x009BA5B0, ReadAsString 0x009BA6F0; CCharString 0x0099E4B0 / 0x0099EBF0 /
// 0x0099EC30 / operator= 0x0099EFB0 / operator!=(const char*) 0x0099E960 / ~ 0x0099EAE0; CBasicString<char>::operator==
// (const char*) 0x004115A0; CWorld::GetThingManager (slot 3), CThingManager::RemapUIDForSerialisation 0x0051E210 and
// GetPThingWithUID 0x0051ED80; CThing::SetInLimbo 0x004C8CF0; CBaseIntelligentPointer 0x00A01B10 / GetPItem 0x00A01B50 /
// SetPItem 0x00A01B90 / ~ 0x00A01C10; NQuestManager::CRuntimeQuestPersistence::LoadFromGameStateScript 0x004B5080;
// CQuestManager::ActivateMultipleQuests 0x004B4260; malloc 0x00BFEA0E, free 0x00BFEA14.
// Layout: engine/CQuestManager.h retail branch + the FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE members (chunk 795).
// Shape: the UID remap and the thing lookup are two statements (a named remapped UID): written as one nested expression VC7.1
//   loads World for the outer GetThingManager before the inner call and keeps it, spilling the UID pair (2567 v 2525); split, it
//   reloads +0x7C for each vcall and keeps the UID in esi:edi, as retail.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_READASUHUGE
#define FABLE_RETAIL_CSTRINGPARSER_READQUOTED
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CStringParser.h"
#include "engine/CQuestManager.h"

// STLport 4.6 _list.c: _List_base::clear is a non-inline member defined out of class; retail calls one instantiation per
// element type. Declared-only explicit specializations keep the calls out of line.
template <> void std::_List_base<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > >::clear();
template <> void std::_List_base<std::pair<CCharString, unsigned long>, std::allocator<std::pair<CCharString, unsigned long> > >::clear();
template <> void std::_List_base<CQuestManager::CQuestCard, std::allocator<CQuestManager::CQuestCard> >::clear();
template <> void std::_List_base<CQuestManager::CBoast, std::allocator<CQuestManager::CBoast> >::clear();

// Same-TU stand-in for ActivateMultipleQuests 0x004B4260 (retail compiled its body in this TU, just before LoadGameState): it
// reads the name array without keeping its address, so VC7.1 keeps the local array's words in registers in LoadGameState.
bool ActivateMultipleQuestsStandIn(CQuestManager* manager, const CCharString* first, const CCharString* last, bool load_resources,
                                   bool from_saved_game);
#pragma auto_inline(off)
__declspec(noinline) bool CQuestManager::ActivateMultipleQuests(const CArray<CCharString>& names, bool load_resources,
                                                                bool from_saved_game)
{
    return ActivateMultipleQuestsStandIn(this, names.begin(), names.end(), load_resources, from_saved_game);
}
#pragma auto_inline(on)

void CQuestManager::LoadGameState(CStringParser& parser)
{
    ActiveQuestCards.clear();
    CompletedQuestCards.clear();
    CreatureGenerationDisabledQuests.clear();
    BetweenStartAndEndScreenQuests.clear();
    PendingQuestsToDeactivate.clear();
    Boasts.clear();
    AcceptedBoasts.clear();
    FrameBoastsLastUpdated = CWorld::GetFrame();
    AvailableQuestCardTypes.clear();
    QuestCardsNeedUpdating = false;

    if (parser.SkipPastString("START_QUEST_CARDS")) {
        parser.SkipWhitespace();
        CCharString name = parser.ReadAsString();
        while (name != "END_QUEST_CARDS") {
            parser.SkipWhitespace();
            parser.SkipPastNextItemIfIs(",");
            parser.SkipWhitespace();
            if (name == "OBJECT_FEAT_CARD_DUMMY") {
                CCharString def_name;
                parser.ReadNextItemAsQuotedString(def_name);
                parser.SkipWhitespace();
                parser.SkipPastNextItemIfIs(",");
                parser.SkipWhitespace();
                CCharString script_name;
                parser.ReadNextItemAsQuotedString(script_name);
                parser.SkipWhitespace();
                parser.SkipPastNextItemIfIs(",");
                parser.SkipWhitespace();
                long feat_index = parser.ReadAsInteger();
                AvailableQuestCardTypes.push_back(CQuestCard(def_name, script_name, feat_index));
            } else {
                CCharString script_name = parser.ReadAsString();
                parser.SkipWhitespace();
                parser.SkipPastNextItemIfIs(",");
                parser.SkipWhitespace();
                AvailableQuestCardTypes.push_back(CQuestCard(name, script_name, parser.ReadAsInteger() > 0));
            }
            parser.SkipWhitespace();
            name = parser.ReadAsString();
        }
    }

    if (parser.SkipPastString("START_ACTIVE_QUEST_CARDS")) {
        parser.SkipWhitespace();
        unsigned __int64 uid;
        while ((uid = parser.ReadAsUHUGE()) > 0) {
            parser.SkipWhitespace();
            unsigned __int64 remapped = World->GetThingManager().RemapUIDForSerialisation(false, true, 0, uid);
            CThing* thing = World->GetThingManager().GetPThingWithUID(remapped);
            if (thing && !thing->IsDead()) {
                thing->SetInLimbo(true);
                ActiveQuestCards.push_back(CIntelligentPointer<CThing>(thing));
            }
            parser.SkipWhitespace();
        }
    }

    if (parser.SkipPastString("START_SAVED_QUESTS")) {
        CCharString text = parser.ReadAsStringUntilString("END_SAVED_QUESTS");
        RuntimeQuestPersistence.LoadFromGameStateScript(text);
        parser.SkipPastString("END_SAVED_QUESTS");
    }

    if (parser.SkipPastString("START_ACTIVE_QUESTS")) {
        CArray<CCharString> names;
        parser.SkipWhitespace();
        CCharString name = parser.ReadAsStringUntilWhitespace();
        while (name != "END_ACTIVE_QUESTS") {
            names.push_back(name);
            parser.SkipWhitespace();
            name = parser.ReadAsStringUntilWhitespace();
        }
        if (names.size() != 0)
            ActivateMultipleQuests(names, false, true);
    }

    if (parser.SkipPastString("START_COMPLETED_QUESTS")) {
        parser.SkipWhitespace();
        unsigned __int64 uid;
        while ((uid = parser.ReadAsUHUGE()) > 0) {
            parser.SkipWhitespace();
            unsigned __int64 remapped = World->GetThingManager().RemapUIDForSerialisation(false, true, 0, uid);
            CThing* thing = World->GetThingManager().GetPThingWithUID(remapped);
            if (thing && !thing->IsDead()) {
                thing->SetInLimbo(true);
                CompletedQuestCards.push_back(CIntelligentPointer<CThing>(thing));
            }
            parser.SkipWhitespace();
        }
    }

    if (parser.SkipPastString("START_CG_DISABLED_QUESTS")) {
        parser.SkipWhitespace();
        CCharString name = parser.ReadAsStringUntilWhitespace();
        while (name != "END_CG_DISABLED_QUESTS") {
            CreatureGenerationDisabledQuests.push_back(name);
            parser.SkipWhitespace();
            name = parser.ReadAsStringUntilWhitespace();
        }
    }

    if (parser.SkipPastString("BETWEEN_START_AND_END_SCREEN_QUESTS")) {
        parser.SkipWhitespace();
        CCharString name = parser.ReadAsStringUntilWhitespace();
        while (name != "END_BETWEEN_START_AND_END_SCREEN_QUESTS") {
            BetweenStartAndEndScreenQuests.push_back(name);
            parser.SkipWhitespace();
            name = parser.ReadAsStringUntilWhitespace();
        }
    }

    if (parser.SkipPastString("NumberOfQuestsBetweenStartAndFinishScreens")) {
        parser.SkipWhitespace();
        NumberOfQuestsBetweenStartAndFinishScreens = parser.ReadAsInteger();
    }
}