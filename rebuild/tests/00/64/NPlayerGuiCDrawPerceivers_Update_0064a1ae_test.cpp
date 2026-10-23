// Behaviour test for 0064a1ae: body copied from src/0064a1ae.cpp (NPlayerGui::CDrawPerceivers::Update), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
struct CPresenceBits {
    unsigned long words[9];
    bool test(long bit) { return (words[bit >> 5] & (1UL << (bit & 31))) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned char _pad0[0x20];
    mutable CPresenceBits m_present;
    CInterfaceMap m_interfaces;
    bool alive;
    bool HasInterface(long type) const { return m_present.test(type); }
    template <class T> T* GetInterfaceUncheckedConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing { };
struct CThingPlayerCreature : CThingCreatureBase { };
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { return thing != 0 && thing->alive; }
template <class T> bool __fastcall IS_THING_ALIVE_00436380(const T& thing) { return thing != 0 && thing->alive; }
static CThingPlayerCreature* g_hero = 0;
struct CPlayer {
    CThingPlayerCreature* GetControlledCreature() { return g_hero; }
};
static CPlayer g_player;
struct CPlayerManager {
    CPlayer* GetMainPlayer() { return &g_player; }
};
static const CThing* g_enemyOf = 0; static const CThing* g_allyOf = 0; static int g_enemyCalls = 0, g_allyCalls = 0;
bool __fastcall AreThingsEnemies(const CThing* a, const CThing* b) { ++g_enemyCalls; CHECK(a == (const CThing*)g_hero); return b == g_enemyOf; }
bool __fastcall AreThingsAllies(const CThing* a, const CThing* b) { ++g_allyCalls; CHECK(a == (const CThing*)g_hero); return b == g_allyOf; }
struct CTCPerceiveThing {
    unsigned char _pad[0x2d];
    bool          PerceivesHostile;
    bool          PerceivesSomething;
    bool          aware;
    bool IsAwareOfThing() { return aware; }
};
struct CBaseIntelligentPointer {
    void* m_pItem;
    void* m_pInfo;
    const CThing* GetPItem() { return (const CThing*)m_pItem; }
};
static int g_erased = 0;
struct CRetailThingPointerList {
    struct node { node* next; node* prev; CBaseIntelligentPointer value; };
    struct iterator {
        node* m_pNode;
        iterator() { }
        explicit iterator(node* n) : m_pNode(n) { }
        iterator(const iterator& o) : m_pNode(o.m_pNode) { }
        CBaseIntelligentPointer& operator*() const { return m_pNode->value; }
        iterator& operator++() { m_pNode = m_pNode->next; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    node* head;
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
    iterator erase(iterator where) { ++g_erased; node* n = where.m_pNode; n->prev->next = n->next; n->next->prev = n->prev; return iterator(n->next); }
};
namespace NPlayerGui {
struct CDrawPerceivers {
    void*                   _vtable;
    void*                   PlayerGui;
    CPlayerManager*         PlayerManager;
    unsigned char           _pad0c[0x28 - 0x0c];
    long                    State;
    long                    Allegiance;
    unsigned char           _pad30[8];
    long                    ThingCount;
    CRetailThingPointerList Things;
    void Update();
};
void CDrawPerceivers::Update()
{
    State = 0;
    Allegiance = 0;
    ThingCount = 0;
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
    if (!IS_THING_ALIVE(hero))
        return;
    for (CRetailThingPointerList::iterator it = Things.begin(); it != Things.end(); ) {
        const CThing* thing = (*it).GetPItem();
        if (IS_THING_ALIVE_00436380(thing)) {
            long countBefore = ThingCount;
            if (thing->HasInterface(0x34)) {
                CTCPerceiveThing* perceive = thing->GetInterfaceUncheckedConst<CTCPerceiveThing>(0x34);
                long level;
                if (perceive->PerceivesHostile)
                    level = 2;
                else if (perceive->IsAwareOfThing() || perceive->PerceivesSomething)
                    level = 1;
                else
                    goto counted;
                if (State < level)
                    State = level;
                ++ThingCount;
            }
        counted:
            if (ThingCount > countBefore) {
                if (Allegiance != 2 && AreThingsEnemies(hero, thing))
                    Allegiance = 2;
                if (Allegiance == 0 && AreThingsAllies(hero, thing))
                    Allegiance = 1;
            }
            ++it;
        } else
            it = Things.erase(it);
    }
}
}
static CThing g_things[4]; static CTCPerceiveThing g_perceive[4]; static CInterfaceMap::SEntry g_entries[4];
static CRetailThingPointerList::node g_nodes[5];
static void Link(int n) { for (int i = 0; i <= n; ++i) { g_nodes[i].next = &g_nodes[(i + 1) % (n + 1)]; g_nodes[(i + 1) % (n + 1)].prev = &g_nodes[i]; } }
int main()
{
    static CPlayerManager manager; static CThingPlayerCreature hero; memset(&hero, 0, sizeof(hero)); hero.alive = true; g_hero = &hero;
    NPlayerGui::CDrawPerceivers d; memset(&d, 0, sizeof(d)); d.PlayerManager = &manager; d.State = 9; d.Allegiance = 9; d.ThingCount = 9; d.Things.head = &g_nodes[0];
    for (int i = 0; i < 4; ++i) {
        memset(&g_things[i], 0, sizeof(CThing)); memset(&g_perceive[i], 0, sizeof(CTCPerceiveThing)); g_things[i].alive = true;
        g_entries[i].type = 0x34; g_entries[i].component = (CTCBase*)&g_perceive[i]; g_things[i].m_interfaces.first = &g_entries[i]; g_things[i].m_interfaces.last = &g_entries[i] + 1;
        g_things[i].m_present.words[0x34 >> 5] |= 1UL << (0x34 & 31); g_nodes[i + 1].value.m_pItem = &g_things[i];
    }
    // no hero: counters reset, nothing walked
    g_hero = 0; Link(4); d.Update(); CHECK(d.State == 0 && d.Allegiance == 0 && d.ThingCount == 0 && g_enemyCalls == 0); g_hero = &hero;
    // thing0 hostile (level 2, enemy), thing1 aware (level 1, ally), thing2 perceives nothing (not counted), thing3 dead (erased)
    g_perceive[0].PerceivesHostile = true; g_perceive[1].aware = true; g_things[3].alive = false; g_enemyOf = &g_things[0]; g_allyOf = &g_things[1];
    d.Update();
    CHECK(d.State == 2 && d.ThingCount == 2 && d.Allegiance == 2); CHECK(g_erased == 1 && g_nodes[3].next == &g_nodes[0]); CHECK(g_enemyCalls == 1 && g_allyCalls == 0);   // the first counted thing settles the allegiance as enemy
    // only an ally perceiving something: level 1, allegiance 1; a thing without the perceive interface is skipped
    g_perceive[0].PerceivesHostile = false; g_perceive[1].aware = false; g_perceive[1].PerceivesSomething = true; g_things[0].m_present.words[0x34 >> 5] = 0; g_enemyOf = 0;
    d.Update(); CHECK(d.State == 1 && d.ThingCount == 1 && d.Allegiance == 1); CHECK(g_enemyCalls == 2 && g_allyCalls == 1);
    if (fails == 0) printf("0064a1ae_TEST PASS\n"); else printf("0064a1ae_TEST FAIL\n");
    return fails;
}