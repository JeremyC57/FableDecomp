// Behaviour test for NGameText::CDataBank::GetTextEntryFromGroup (0x009C9280): a standalone copy of the row's logic over a
// small model bank (entry types, text infos with narrator indices and load state, groups of text ids, the LCG seed).
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>

static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed *= 0x24a1;
    *seed += 0x24df;
    *seed = ror13(*seed);
    unsigned long result = 0;
    if (range)
        result = *seed % range;
    return result;
}

struct TextEntry { bool hasPending; bool finished; };
struct TextInfo { TextEntry* text; long narratorIndex; };

struct Bank {
    std::vector<int> types;                      // 0 = text, 1 = group, 2 = other
    std::vector<TextInfo*> infos;                // per id (text ids)
    std::vector<std::vector<long> > groups;      // per id (group ids)
    std::vector<bool> groupLoaded;
    std::vector<std::string> narrators;
    unsigned long seed;
    std::vector<long> loadRequests;
    int updates;

    unsigned long GetEntryDataType(unsigned long id) const { return (unsigned long)types[id]; }
    const TextInfo* GetTextInfo(unsigned long id) const { return GetEntryDataType(id) == 0 ? infos[id] : 0; }
    static bool IsLoaded(const TextInfo* info)
    {
        return info->text != 0 && (!info->text->hasPending || info->text->finished);
    }

    const TextEntry* GetTextEntryFromGroup(unsigned long groupId, const std::string& narrator, bool& found, long* lastId,
                                           bool allowPending)
    {
        found = false;
        if (groupId > 0 && GetEntryDataType(groupId) == 1) {
            std::vector<long> ids;
            if (!groupLoaded[groupId]) {
                found = true;
                return 0;
            }
            const std::vector<long>& entries = groups[groupId];
            for (unsigned long i = 0; i < entries.size(); ++i) {
                long id = entries[i];
                const std::string& name = narrators[GetTextInfo(id)->narratorIndex];
                if (name == narrator || name == narrator + "_1_5")
                    ids.push_back(id);
            }
            if (!ids.empty()) {
                found = true;
                long count = (long)ids.size();
                long id = ids[0];
                unsigned long pick = 0;
                if (count > 1) {
                    do {
                        pick = GFRandom(count, &seed);
                        id = ids[pick];
                    } while (lastId && id == *lastId);
                }
                ++updates;
                if (IsLoaded(GetTextInfo(id)) || allowPending) {
                    if (lastId)
                        *lastId = id;
                    return infos[id]->text;
                }
                loadRequests.push_back(id);
                long index = (long)pick;
                for (long tries = 0; tries < count; ++tries) {
                    long other = ids[index];
                    ++updates;
                    if (IsLoaded(GetTextInfo(other))) {
                        if (lastId)
                            *lastId = other;
                        return infos[other]->text;
                    }
                    index = (index + 1) % count;
                }
            }
        }
        return 0;
    }
};

static int fail(const char* why) { std::printf("009c9280_TEST FAIL (%s)\n", why); return 1; }

int main()
{
    TextEntry loadedA = { false, false }, loadedB = { true, true }, pendingC = { true, false };
    TextInfo t1 = { &loadedA, 0 }, t2 = { &loadedB, 1 }, t3 = { &pendingC, 0 }, t4 = { &loadedA, 2 };
    Bank bank;
    bank.types.resize(8, 2);
    bank.infos.resize(8, (TextInfo*)0);
    bank.groups.resize(8);
    bank.groupLoaded.resize(8, true);
    bank.types[1] = 0; bank.infos[1] = &t1;
    bank.types[2] = 0; bank.infos[2] = &t2;
    bank.types[3] = 0; bank.infos[3] = &t3;
    bank.types[4] = 0; bank.infos[4] = &t4;
    bank.types[5] = 1; bank.groups[5].push_back(1); bank.groups[5].push_back(2); bank.groups[5].push_back(3); bank.groups[5].push_back(4);
    bank.types[6] = 1; bank.groupLoaded[6] = false;
    bank.types[7] = 1; bank.groups[7].push_back(3);
    bank.types.resize(10, 2); bank.infos.resize(10, (TextInfo*)0); bank.groups.resize(10); bank.groupLoaded.resize(10, true);
    bank.types[8] = 1; bank.groups[8].push_back(2);                         // only a HERO_1_5 entry
    bank.types[9] = 1; bank.groups[9].push_back(1); bank.groups[9].push_back(2);   // HERO + HERO_1_5, both loaded
    bank.narrators.push_back("HERO"); bank.narrators.push_back("HERO_1_5"); bank.narrators.push_back("GUARD");
    bank.seed = 12345;
    bank.updates = 0;

    bool found = true;
    // group id 0 and non-group ids: nothing, found cleared
    if (bank.GetTextEntryFromGroup(0, "HERO", found, 0, false) != 0 || found) return fail("id 0");
    found = true;
    if (bank.GetTextEntryFromGroup(1, "HERO", found, 0, false) != 0 || found) return fail("not a group");
    // a group still pending: null but found
    if (bank.GetTextEntryFromGroup(6, "HERO", found, 0, false) != 0 || !found) return fail("pending group");
    // no narrator matches: null, not found
    if (bank.GetTextEntryFromGroup(5, "NOBODY", found, 0, false) != 0 || found) return fail("no match");
    // "GUARD" matches only id 4 (one candidate: no random draw)
    long last = -1;
    unsigned long seedBefore = bank.seed;
    const TextEntry* e = bank.GetTextEntryFromGroup(5, "GUARD", found, &last, false);
    if (e != &loadedA || !found || last != 4 || bank.seed != seedBefore) return fail("single candidate");
    // a loaded first pick is returned on the fast path: it must not queue a load request
    if (!bank.loadRequests.empty()) return fail("loaded pick queued a load");
    // "HERO" matches ids 1, 2 ("HERO_1_5") and 3; repeated picks always return a loaded entry
    // (id 3 is pending: it is queued for loading and the next loaded candidate is returned instead)
    last = 2;
    for (int n = 0; n < 50; ++n) {
        e = bank.GetTextEntryFromGroup(5, "HERO", found, &last, false);
        if (!found || e == 0) return fail("hero pick");
        if (e != bank.infos[last]->text) return fail("returned entry vs lastId");
        if (last == 3) return fail("pending entry returned");
    }
    // only the "_1_5" narrator matches: "HERO" still finds id 2 (narrator HERO_1_5)
    last = -1;
    e = bank.GetTextEntryFromGroup(8, "HERO", found, &last, false);
    if (e != &loadedB || !found || last != 2) return fail("narrator _1_5 alternative");
    // two loaded candidates: every pick re-rolls away from *lastId, so the picks alternate
    last = 1;
    size_t requestsBeforeLoadedPicks = bank.loadRequests.size();
    for (int n = 0; n < 50; ++n) {
        long before = last;
        e = bank.GetTextEntryFromGroup(9, "HERO", found, &last, false);
        if (!found || e == 0 || e != bank.infos[last]->text) return fail("two-candidate pick");
        if (last == before) return fail("pick repeated the last id");
    }
    // both candidates are loaded: none of those picks may have asked for a load
    if (bank.loadRequests.size() != requestsBeforeLoadedPicks) return fail("loaded picks queued a load");
    // without lastId there is no re-roll constraint, but the pick is still one of the candidates
    e = bank.GetTextEntryFromGroup(9, "HERO", found, 0, false);
    if (e != &loadedA && e != &loadedB) return fail("pick without lastId");
    if (bank.loadRequests.empty()) return fail("pending pick never queued a load");
    // with pending allowed, the only candidate (pending id 3 in group 7) is returned as is
    last = -1;
    e = bank.GetTextEntryFromGroup(7, "HERO", found, &last, true);
    if (e != &pendingC || last != 3) return fail("allow pending");
    // without pending allowed and nothing loaded: load requested, null, found
    size_t requests = bank.loadRequests.size();
    e = bank.GetTextEntryFromGroup(7, "HERO", found, &last, false);
    if (e != 0 || !found || bank.loadRequests.size() != requests + 1 || bank.loadRequests.back() != 3) return fail("nothing loaded");
    std::printf("009c9280_TEST PASS\n");
    return 0;
}