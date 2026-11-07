// CAIStateGroup_MinionReactToPlayerInteraction::GetGossipPhrase(unsigned long& phrase) -- pick a gossip line for the creature: collect every active gossip category that is for the player, allowed for the creature's village and faction and has at least one phrase the creature's dialogue voice can say (plain or "_1_5"), then choose a random category and a random phrase of it, returning its text-bank index
// Grade: DIFFER(1994v1945) -- md 288: iterator slot [esp+0x48] v retail [esp+0x3c]; categories._M_start kept in ebx through the loop (each continue reloads it, so the villages continue is not merged into the shared 0x0092F47B pad); both loop-body CTCTalk::GetDialogueVoice sites expanded (retail calls 0x004FAEE0 there, expands only the first test); the __find tag is pushed as 0 where retail passes the dead phrase slot [esp+0x6c] (GOTCHAS chunk 149 lane gap); size()==0 return not tail-merged
// Next idea: a same-TU auto_inline(off) GetDialogueVoice twin is not allowed (one address, one name); look for the CTCTalk header form that keeps the accessor small enough to inline once (a by-value getter of a reference member?) and re-time the ebx allocation
// Leaf: ?GetGossipPhrase@CAIStateGroup_MinionReactToPlayerInteraction@@QAE_NAAK@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_BRIBE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCWIFE_GREET
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCSHOP_AVAILABLE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CDATABANK
#define FABLE_RETAIL_CDATABANK_TEXTENTRY
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_DIALOGUEVOICE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCWife.h"
#include "engine/CTCShop.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CTCHeroStats.h"
#include "engine/C3DVector.h"
#include "fable_game_text_bank.h"
#include "engine/CTCTalk.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
long __fastcall GFROR13(unsigned long value);                       // 0x00497890 ?GFROR13@@YIJK@Z

class CBankFile { public:                                           // engine/CBankFile.h is HOLD: the one member this row calls
    unsigned long FindIndexBySymbolCRC(unsigned long crc) const;    // 0x009CC410 (Ego_r ?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z)
};
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
inline unsigned long CCharString::GetCRC() const { return CCRC::Calc(0, GetAsCharArray(), GetLength()); }
inline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
class CFaction { public:                                            // Ego_r.pdb CFaction (36 bytes): Name +0
    CCharString Name;
    CCharString GetName() const { return Name; }                    // FableWin ?GetName@CFaction@@QBE?AVCCharString@@XZ (inline)
};
class CTCEnemy : public CTCBase { public:                          // TCI_ENEMY (73); retail PFaction +0x18
    unsigned char _pad_0x0c[0x18 - 0x0c];
    CFaction* PFaction;                                             // +0x18
    const CFaction* GetPFaction() const { return PFaction; }        // FableWin ?GetPFaction@CTCEnemy@@QBEPBVCFaction@@XZ (inline)
};
typedef std::vector<CCharString> CGossipList;
class CTCAIScratchpad { public:                                    // the gossip statics (Ego_r.pdb ?Get<Name>@CTCAIScratchpad@@SI...)
    static std::map<CCharString, bool> ActiveGossipCategories;      // 0x013BAE2C
    static std::map<CCharString, bool> IsGossipForPlayer;           // 0x013BAE38
    static std::map<CCharString, CGossipList> Gossip;               // 0x013BAE44
    static std::map<CCharString, CGossipList> GossipVillages;       // 0x013BAE50
    static std::map<CCharString, CGossipList> GossipFactions;       // 0x013BAE5C
    static bool* __fastcall GetActiveGossipCategories(CCharString name)
    { std::map<CCharString, bool>::iterator it = ActiveGossipCategories.find(name); if (it == ActiveGossipCategories.end()) return 0; return &ActiveGossipCategories[name]; }
    static bool* __fastcall GetIsGossipForPlayer(CCharString name)
    { std::map<CCharString, bool>::iterator it = IsGossipForPlayer.find(name); if (it == IsGossipForPlayer.end()) return 0; return &IsGossipForPlayer[name]; }
    static const CGossipList* __fastcall GetGossip(CCharString name)
    { std::map<CCharString, CGossipList>::iterator it = Gossip.find(name); if (it == Gossip.end()) return 0; return &Gossip[name]; }
    static const CGossipList* __fastcall GetGossipVillages(CCharString name)
    { std::map<CCharString, CGossipList>::iterator it = GossipVillages.find(name); if (it == GossipVillages.end()) return 0; return &GossipVillages[name]; }
    static const CGossipList* __fastcall GetGossipFactions(CCharString category);   // 0x008AE970 (landed, out of line)
};
namespace std {
struct random_access_iterator_tag;
const CCharString* __fastcall __find(const CCharString* first, const CCharString* last, const CCharString& val, const random_access_iterator_tag&);   // 0x004B8FF0
}
inline const CCharString* GossipFind(const CCharString* first, const CCharString* last, const CCharString& val)
{ return std::__find(first, last, val, *(const std::random_access_iterator_tag*)0); }

#pragma pack(push, 1)
class CAIStateGroup_MinionReactToPlayerInteraction : public CAIStateGroupBase {
public:
    bool GetGossipPhrase(unsigned long& phrase);                    // 0x0092F310
private:
    unsigned long NextReactionFrame[5];                             // +0x14
    long          ReactionType;                                     // +0x28
    bool          WaitingForConfirmation;                           // +0x2c
    bool          DidTraderInteraction;                             // +0x2d
    C3DVector     PositionPlayerWasWhenWaitingForConfirmation;      // +0x2e
};
#pragma pack(pop)

bool CAIStateGroup_MinionReactToPlayerInteraction::GetGossipPhrase(unsigned long& phrase)
{
    CThingAICreature& creature = GetCreature();
    CCharString village_name("");
    CTCVillageMember* member;
    if (creature.GetTC(TCI_VILLAGE_MEMBER, &member) && member->GetPVillage())
        village_name = member->GetPVillage()->GetParentThing().PeekBaseDef().GetInstantiationName();
    CCharString faction_name("");
    CTCEnemy* enemy;
    if (creature.GetTC(TCI_ENEMY, &enemy) && enemy->GetPFaction())
        faction_name = enemy->GetPFaction()->GetName();
    CGossipList categories;
    for (std::map<CCharString, CGossipList>::iterator it = CTCAIScratchpad::Gossip.begin(); it != CTCAIScratchpad::Gossip.end(); ++it) {
        CCharString category = it->first;
        bool* active = CTCAIScratchpad::GetActiveGossipCategories(category);
        if (!active || !*active)
            continue;
        bool* for_player = CTCAIScratchpad::GetIsGossipForPlayer(category);
        if (for_player && !*for_player)
            continue;
        const CGossipList* villages = CTCAIScratchpad::GetGossipVillages(category);
        if (villages && (village_name == CCharString("") || GossipFind(villages->begin(), villages->end(), village_name) == villages->end()))
            continue;
        const CGossipList* factions = CTCAIScratchpad::GetGossipFactions(category);
        if (factions && (faction_name == CCharString("") || GossipFind(factions->begin(), factions->end(), faction_name) == factions->end()))
            continue;
        const CTCTalk* talk;
        if (!static_cast<const CThing&>(GetCreature()).PeekTC(TCI_TALK, &talk) || talk->GetDialogueVoice() == "")
            return false;
        const CGossipList* phrases = CTCAIScratchpad::GetGossip(category);
        if (phrases && phrases->size()) {
            const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
            for (const CCharString* name = phrases->begin(); name != phrases->end(); ++name) {
                unsigned long index = reinterpret_cast<const CBankFile&>(bank).FindIndexBySymbolCRC(name->GetCRC());
                if (index) {
                    bool found = false;
                    bank.GetTextEntryFromGroup(index, talk->GetDialogueVoice(), found, 0, false);
                    if (found) {
                        categories.push_back(category);
                        break;
                    }
                    bank.GetTextEntryFromGroup(index, talk->GetDialogueVoice() + "_1_5", found, 0, false);
                    if (found) {
                        categories.push_back(category);
                        break;
                    }
                }
            }
        }
    }
    if (categories.size() == 0)
        return false;
    CCharString chosen = categories[GFRandom(categories.size(), GetPWorldSeed())];
    const CGossipList* phrases = CTCAIScratchpad::GetGossip(chosen);
    if (!phrases)
        return false;
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    phrase = reinterpret_cast<const CBankFile&>(bank).FindIndexBySymbolCRC((*phrases)[GFRandom(phrases->size(), GetPWorldSeed())].GetCRC());
    return phrase > 0;
}
