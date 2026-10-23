// Behaviour test for CTCInventory::ConstructStyleEntryPC (0x0058315b): an empty style list clears the list; otherwise one sub-list
// entry per (style, count) pair with select (0x7d0), focus (0x7df), view (0x7d3), focus (0x7e2) and unfocus (0x7d6) actions
// whose long parameters are the style and count (plus the owner component on the select and view actions when given), and the list gets the sub-list, the name STYLE_CARDS_LIST and a 0x15 action.
#include <cstdio>
#include <cstring>

struct SAction { long type; long longs[4]; int nLongs; int nComponents; };
struct SEntry { long style; SAction actions[5]; int nActions; long userValue; };
struct SList {
    bool cleared;
    int nEntries;
    SEntry entries[8];
    char name[32];
    bool hasSubList;
    int headerSet;
    SAction listAction;
};
struct SPair { long first, second; };

static void AddLong(SAction& a, long v) { a.longs[a.nLongs++] = v; }
static void Model(SList& list, const SPair* begin, const SPair* end, bool hasOwner, bool pc)
{
    if (begin == end) {
        list.cleared = true;
        return;
    }
    if (pc && hasOwner)
        ++list.headerSet;
    for (const SPair* it = begin; it != end; ++it) {
        SEntry& e = list.entries[list.nEntries++];
        e.style = it->first;
        e.nActions = 0;
        e.userValue = -1;
        SAction* a = &e.actions[e.nActions++];
        a->type = 0x7d0; a->nLongs = 0; a->nComponents = 0;
        AddLong(*a, it->first); ++a->nComponents; if (pc && hasOwner) ++a->nComponents;
        a = &e.actions[e.nActions++];
        a->type = 0x7df; a->nLongs = 0; a->nComponents = 1;
        a = &e.actions[e.nActions++];
        a->type = 0x7d3; a->nLongs = 0; a->nComponents = 2;
        AddLong(*a, it->first); AddLong(*a, it->second); if (pc && hasOwner) ++a->nComponents;
        a = &e.actions[e.nActions++];
        a->type = 0x7e2; a->nLongs = 0; a->nComponents = 1;
        a = &e.actions[e.nActions++];
        a->type = 0x7d6; a->nLongs = 0; a->nComponents = 1;
        AddLong(*a, it->first); AddLong(*a, it->second);
        if (!pc)
            e.userValue = it->first;
    }
    list.hasSubList = true;
    std::strcpy(list.name, "STYLE_CARDS_LIST");
    list.listAction.type = 0x15;
}

int main()
{
    int fails = 0;
    const bool pc = true;
    SList list;
    std::memset(&list, 0, sizeof(list));
    Model(list, 0, 0, true, pc);
    if (!list.cleared || list.nEntries != 0 || list.hasSubList) ++fails;

    SPair styles[2] = { { 5, 2 }, { 9, 1 } };
    std::memset(&list, 0, sizeof(list));
    Model(list, styles, styles + 2, true, pc);
    if (list.cleared || list.nEntries != 2 || !list.hasSubList || std::strcmp(list.name, "STYLE_CARDS_LIST") != 0) ++fails;
    if (list.listAction.type != 0x15) ++fails;
    if (list.entries[1].actions[2].longs[0] != 9 || list.entries[1].actions[2].longs[1] != 1) ++fails;
    if (list.entries[0].actions[4].type != 0x7d6 || list.entries[0].actions[4].longs[1] != 2) ++fails;
    if (pc) {
        if (list.headerSet != 1 || list.entries[0].actions[0].nComponents != 2 || list.entries[0].userValue != -1) ++fails;
    } else {
        if (list.headerSet != 0 || list.entries[0].actions[0].nComponents != 1 || list.entries[1].userValue != 9) ++fails;
    }
    if (fails == 0)
        std::printf("0058315b_TEST PASS\n");
    else
        std::printf("0058315b_TEST FAIL %d\n", fails);
    return 0;
}
