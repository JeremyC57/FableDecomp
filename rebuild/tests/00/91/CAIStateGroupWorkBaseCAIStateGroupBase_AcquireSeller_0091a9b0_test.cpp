// Behaviour test (0x0091a9b0): CAIStateGroup_WorkBase<CAIStateGroupBase>::AcquireSeller gathers the shop-keepers (TC 0x90) within 20 units of the creature through a +-20 integer box, then draws random candidates off the list until it is empty: a busy candidate is skipped, a free one makes the creature leave its interaction, open a new type-8 interaction and the candidate join it
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_allocs = 0, g_frees = 0;
static void* CountedMalloc(unsigned int size) { ++g_allocs; return std::malloc(size); }
static void CountedFree(void* p) { if (p) ++g_frees; std::free(p); }

struct C3DVector { float X, Y, Z; };
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI(long x0, long y0, long x1, long y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) { }
};

// STLport list<T*> model (malloc'd header, self-linked; erase unlinks and frees).
struct ListNodeBase { ListNodeBase* next; ListNodeBase* prev; };
struct Creature;
struct ListNode : ListNodeBase { Creature* data; };
struct CreatureList {
    ListNodeBase* header;
    CreatureList()
    {
        ListNodeBase* tmp = (ListNodeBase*)CountedMalloc(sizeof(ListNode));
        tmp->next = tmp; tmp->prev = tmp; header = tmp;
    }
    ~CreatureList()
    {
        ListNodeBase* cur = header->next;
        while (cur != header) { ListNodeBase* t = cur; cur = cur->next; CountedFree(t); }
        header->next = header; header->prev = header;
        CountedFree(header);
    }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (ListNodeBase* p = header->next; p != header; p = p->next) ++n;
        return n;
    }
    void push_back(Creature* c)
    {
        ListNode* n = (ListNode*)CountedMalloc(sizeof(ListNode));
        n->data = c; n->next = header; n->prev = header->prev;
        header->prev->next = n; header->prev = n;
    }
    void erase(ListNodeBase* node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        CountedFree(node);
    }
};

// Creatures: position, a component flag for TC 0x90, and a creature-interaction id (0 = none).
static int g_nextInteraction = 100;
static int g_interactionType[256];
static int g_removeCalls = 0, g_newCalls = 0, g_addCalls = 0;
struct Creature {
    C3DVector Pos;
    bool HasShopTC;
    int Interaction;
    const C3DVector& GetPosition() { return Pos; }
    void RemoveFromCreatureInteraction() { ++g_removeCalls; Interaction = 0; }
    void NewCreatureInteraction(long type)
    {
        ++g_newCalls;
        Interaction = g_nextInteraction++;
        g_interactionType[Interaction - 100] = (int)type;
    }
    void AddToCreatureInteraction(const int& interaction) { ++g_addCalls; Interaction = interaction; }
};

struct CThingFilter_HasTC { long TCType; CThingFilter_HasTC(long t) : TCType(t) { } };
struct CThingFilter_In2DRange {
    C3DVector Pos; float Range;
    CThingFilter_In2DRange(const C3DVector& p, float r) : Pos(p), Range(r) { }
};
struct Bind2 {
    const CThingFilter_HasTC* First; const CThingFilter_In2DRange* Second;
    Bind2(const CThingFilter_HasTC& a, const CThingFilter_In2DRange& b) : First(&a), Second(&b) { }
};

static Creature g_world[8];
static int g_worldCount = 0;
static C2DBoxI g_lastArea(0, 0, 0, 0);
static long g_lastTC = 0;
static float g_lastRange = 0.0f;
struct ThingSearchTools {
    void FindThingsInRoughArea(const C2DBoxI& area, const Bind2& filter, CreatureList& things)
    {
        g_lastArea = area; g_lastTC = filter.First->TCType; g_lastRange = filter.Second->Range;
        for (int i = 0; i < g_worldCount; ++i) {
            Creature& c = g_world[i];
            if (c.Pos.X < (float)area.X0 || c.Pos.X > (float)area.X1 || c.Pos.Y < (float)area.Y0 || c.Pos.Y > (float)area.Y1)
                continue;
            if (filter.First->TCType != 0x90 || !c.HasShopTC)
                continue;
            float dx = c.Pos.X - filter.Second->Pos.X, dy = c.Pos.Y - filter.Second->Pos.Y;
            if (dx * dx + dy * dy > filter.Second->Range * filter.Second->Range)
                continue;
            things.push_back(&c);
        }
    }
};

static unsigned long Ror13(unsigned long v) { return ((v >> 13) | (v << 19)) & 0xFFFFFFFFUL; }
static unsigned long g_seed = 12345;
static ThingSearchTools g_search;

struct Brain { Creature* PCreature; };
struct WorkGroup {
    Brain* PBrain;
    ThingSearchTools* PeekThingSearchTools() { return &g_search; }
    unsigned long* GetPWorldSeed() { return &g_seed; }
    unsigned long GetRandomIndex(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = (*seed * 0x24A1UL + 0x24DFUL) & 0xFFFFFFFFUL;
        *seed = next;
        unsigned long rotated = Ror13(next);
        *seed = rotated;
        unsigned long index;
        if (range != 0) index = rotated % range; else index = 0;
        return index;
    }
    // The function under test, in the source's shape.
    void AcquireSeller()
    {
        C3DVector pos = PBrain->PCreature->GetPosition();
        Creature* seller = 0;
        CreatureList sellers;
        long x = (long)pos.X;
        long y = (long)pos.Y;
        C2DBoxI area(x - 20, y - 20, x + 20, y + 20);
        CThingFilter_HasTC hasTC(0x90);
        CThingFilter_In2DRange inRange(pos, 20.0f);
        PeekThingSearchTools()->FindThingsInRoughArea(area, Bind2(hasTC, inRange), sellers);
        while (sellers.size() != 0) {
            unsigned long count = sellers.size();
            unsigned long index = GetRandomIndex(count);
            for (ListNodeBase* it = sellers.header->next; it != sellers.header; it = it->next) {
                if (index == 0) {
                    seller = ((ListNode*)it)->data;
                    sellers.erase(it);
                    break;
                }
                --index;
            }
            if (seller->Interaction != 0)
                continue;
            if (PBrain->PCreature->Interaction != 0)
                PBrain->PCreature->RemoveFromCreatureInteraction();
            PBrain->PCreature->NewCreatureInteraction(8);
            seller->AddToCreatureInteraction(PBrain->PCreature->Interaction);
        }
    }
};

static int fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("  fail: %s\n", what); ++fails; } }

static void SetCreature(int i, float x, float y, bool shop, int interaction)
{
    g_world[i].Pos.X = x; g_world[i].Pos.Y = y; g_world[i].Pos.Z = 0.0f;
    g_world[i].HasShopTC = shop; g_world[i].Interaction = interaction;
}

int main()
{
    // Scenario 1: me at (100.5, 200.75) already in interaction 7; candidates: three free shop-keepers in range, one busy one,
    // a shop-keeper in the box but outside the 20-unit circle, a non-shop creature, a far shop-keeper.
    Creature me; me.Pos.X = 100.5f; me.Pos.Y = 200.75f; me.Pos.Z = 3.0f; me.HasShopTC = false; me.Interaction = 7;
    g_worldCount = 7;
    SetCreature(0, 105.0f, 205.0f, true, 0);    // in
    SetCreature(1, 90.0f, 195.0f, true, 0);     // in
    SetCreature(2, 115.5f, 215.5f, true, 0);    // in the box, outside the circle
    SetCreature(3, 101.0f, 199.0f, true, 55);   // in, busy
    SetCreature(4, 100.0f, 210.0f, false, 0);   // not a shop-keeper
    SetCreature(5, 300.0f, 200.0f, true, 0);    // far
    SetCreature(6, 110.0f, 190.0f, true, 0);    // in
    Creature* candidates[8]; int n = 0;
    candidates[n++] = &g_world[0]; candidates[n++] = &g_world[1]; candidates[n++] = &g_world[3]; candidates[n++] = &g_world[6];
    // expected model: draw with the same seed sequence off an array
    unsigned long seed = 777;
    int expectedMe = 7, expectedNext = 100;
    int expectedState[8]; for (int i = 0; i < 7; ++i) expectedState[i] = g_world[i].Interaction;
    int expectedRemoves = 0, expectedNews = 0, expectedAdds = 0;
    while (n != 0) {
        unsigned long next = (seed * 0x24A1UL + 0x24DFUL) & 0xFFFFFFFFUL;
        seed = Ror13(next);
        int idx = (int)(seed % (unsigned long)n);
        Creature* pick = candidates[idx];
        for (int k = idx; k + 1 < n; ++k) candidates[k] = candidates[k + 1];
        --n;
        int wi = (int)(pick - g_world);
        if (expectedState[wi] != 0) continue;
        if (expectedMe != 0) { expectedMe = 0; ++expectedRemoves; }
        expectedMe = expectedNext++; ++expectedNews;
        expectedState[wi] = expectedMe; ++expectedAdds;
    }
    g_seed = 777; g_allocs = g_frees = 0;
    Brain brain; brain.PCreature = &me;
    WorkGroup group; group.PBrain = &brain;
    group.AcquireSeller();
    Check(g_lastArea.X0 == 80 && g_lastArea.Y0 == 180 && g_lastArea.X1 == 120 && g_lastArea.Y1 == 220, "search box is the integer position +-20");
    Check(g_lastTC == 0x90, "filter asks for TC 0x90");
    Check(g_lastRange > 19.999f && g_lastRange < 20.001f, "filter range 20");
    Check(me.Interaction == expectedMe, "creature ends in the expected interaction");
    for (int i = 0; i < 7; ++i) Check(g_world[i].Interaction == expectedState[i], "candidate interaction state");
    Check(g_removeCalls == expectedRemoves && g_newCalls == expectedNews && g_addCalls == expectedAdds, "interaction call counts");
    Check(g_newCalls == 3 && g_world[3].Interaction == 55, "three free sellers joined, the busy one untouched");
    Check(g_world[2].Interaction == 0 && g_world[4].Interaction == 0 && g_world[5].Interaction == 0, "filtered things untouched");
    for (int t = 0; t < g_newCalls; ++t) Check(g_interactionType[t] == 8, "interaction type 8");
    Check(g_seed == seed, "seed advanced once per draw");
    Check(g_allocs == g_frees && g_allocs == 5, "list header + four nodes allocated and freed");

    // Scenario 2: nobody in range -- no draws, no interaction changes.
    g_worldCount = 1;
    SetCreature(0, 500.0f, 500.0f, true, 0);
    me.Interaction = 9;
    g_removeCalls = g_newCalls = g_addCalls = 0; g_seed = 5; g_allocs = g_frees = 0;
    group.AcquireSeller();
    Check(me.Interaction == 9 && g_removeCalls == 0 && g_newCalls == 0 && g_addCalls == 0, "empty search changes nothing");
    Check(g_seed == 5, "no draw on an empty list");
    Check(g_allocs == 1 && g_frees == 1, "header only");

    // Scenario 3: one busy candidate only -- drawn and skipped, creature keeps its interaction.
    SetCreature(0, 101.0f, 201.0f, true, 42);
    g_seed = 1;
    group.AcquireSeller();
    Check(me.Interaction == 9 && g_newCalls == 0 && g_world[0].Interaction == 42, "busy candidate skipped");
    Check(g_seed != 1, "one draw made");

    if (fails) { std::printf("0091a9b0_TEST FAIL\n"); return 1; }
    std::printf("0091a9b0_TEST PASS\n");
    return 0;
}