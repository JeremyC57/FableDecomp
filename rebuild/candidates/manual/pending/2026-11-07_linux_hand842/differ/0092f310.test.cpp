// Behaviour test for 0x0092f310 CAIStateGroup_MinionReactToPlayerInteraction::GetGossipPhrase(unsigned long&): collect each
// gossip category that is active, not marked not-for-player, allowed for the creature's village and faction (when the category
// lists any) and has a phrase with a text entry in the creature's dialogue voice (or voice + "_1_5"); false when the creature has
// no voice or nothing qualifies; else pick a category and a phrase with GFRandom and return the phrase's bank index (> 0).
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

typedef std::vector<std::string> CGossipList;
static std::map<std::string, bool> ActiveGossipCategories, IsGossipForPlayer;
static std::map<std::string, CGossipList> Gossip, GossipVillages, GossipFactions;
static std::map<std::string, unsigned long> g_bank;            // symbol -> index
static std::map<std::string, bool> g_entries;                  // "index:group" present
static unsigned long g_seed = 1;
static unsigned long GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{ *seed = *seed * 0x24A1 + 0x24DF; *seed = GFROR13(*seed); return range ? *seed % range : 0; }
template <class T> static T* Find(std::map<std::string, T>& m, const std::string& k)
{ typename std::map<std::string, T>::iterator it = m.find(k); if (it == m.end()) return 0; return &m[k]; }
static unsigned long FindIndex(const std::string& s) { std::map<std::string, unsigned long>::iterator it = g_bank.find(s); return it == g_bank.end() ? 0 : it->second; }
static bool HasEntry(unsigned long index, const std::string& group)
{ char b[16]; std::sprintf(b, "%lu:", index); return g_entries.count(b + group) != 0; }

struct Creature { std::string Village, Faction, Voice; bool HasTalk; };

static bool GetGossipPhrase(const Creature& c, unsigned long& phrase)
{
    std::string village_name(c.Village), faction_name(c.Faction);
    CGossipList categories;
    for (std::map<std::string, CGossipList>::iterator it = Gossip.begin(); it != Gossip.end(); ++it) {
        std::string category = it->first;
        bool* active = Find(ActiveGossipCategories, category);
        if (!active || !*active) continue;
        bool* for_player = Find(IsGossipForPlayer, category);
        if (for_player && !*for_player) continue;
        CGossipList* villages = Find(GossipVillages, category);
        if (villages && (village_name == "" || std::find(villages->begin(), villages->end(), village_name) == villages->end())) continue;
        CGossipList* factions = Find(GossipFactions, category);
        if (factions && (faction_name == "" || std::find(factions->begin(), factions->end(), faction_name) == factions->end())) continue;
        if (!c.HasTalk || c.Voice == "") return false;
        CGossipList* phrases = Find(Gossip, category);
        if (phrases && phrases->size()) {
            for (CGossipList::iterator name = phrases->begin(); name != phrases->end(); ++name) {
                unsigned long index = FindIndex(*name);
                if (index) {
                    if (HasEntry(index, c.Voice)) { categories.push_back(category); break; }
                    if (HasEntry(index, c.Voice + "_1_5")) { categories.push_back(category); break; }
                }
            }
        }
    }
    if (categories.size() == 0) return false;
    std::string chosen = categories[GFRandom(categories.size(), &g_seed)];
    CGossipList* phrases = Find(Gossip, chosen);
    if (!phrases) return false;
    phrase = FindIndex((*phrases)[GFRandom(phrases->size(), &g_seed)]);
    return phrase > 0;
}

int main()
{
    bool ok = true;
    Gossip["WEATHER"].push_back("TEXT_WEATHER_1");
    Gossip["BANDITS"].push_back("TEXT_BANDITS_1");
    Gossip["SECRET"].push_back("TEXT_SECRET_1");
    ActiveGossipCategories["WEATHER"] = true; ActiveGossipCategories["BANDITS"] = true; ActiveGossipCategories["SECRET"] = false;
    IsGossipForPlayer["BANDITS"] = true;
    GossipVillages["BANDITS"].push_back("Oakvale");
    GossipFactions["WEATHER"].push_back("FACTION_VILLAGERS");
    g_bank["TEXT_WEATHER_1"] = 11; g_bank["TEXT_BANDITS_1"] = 12; g_bank["TEXT_SECRET_1"] = 13;
    g_entries["11:MALE_1_5"] = true; g_entries["12:MALE"] = true; g_entries["13:MALE"] = true;
    Creature oak = { "Oakvale", "FACTION_GUARDS", "MALE", true };
    unsigned long phrase = 0;
    ok = ok && GetGossipPhrase(oak, phrase) && phrase == 12;                     // only BANDITS qualifies (faction blocks WEATHER)
    Creature vil = { "Bowerstone", "FACTION_VILLAGERS", "MALE", true };
    phrase = 0;
    ok = ok && GetGossipPhrase(vil, phrase) && phrase == 11;                     // WEATHER through the "_1_5" voice
    Creature mute = { "Oakvale", "FACTION_VILLAGERS", "", true };
    ok = ok && !GetGossipPhrase(mute, phrase);                                   // no dialogue voice
    Creature none = { "", "", "MALE", true };
    ok = ok && !GetGossipPhrase(none, phrase);                                   // village and faction filters reject all
    IsGossipForPlayer["BANDITS"] = false;
    ok = ok && !GetGossipPhrase(oak, phrase);                                    // BANDITS not for the player
    std::printf(ok ? "0092f310_TEST PASS\n" : "0092f310_TEST FAIL\n");
    return ok ? 0 : 1;
}
