// Behaviour test for AddToRelevantBuilding (0x00502B00): a copy of the source's logic over small stand-ins for the
// STLport map / list / vector and the engine callees (seed LCG + GFROR13, def pointer reference counts).
#include <cstdio>
#include <cstdlib>
#include <cstring>

static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
long GFROR13(unsigned long value) { return (long)(ror13(value) & 0xffffffffUL); }
unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = (*seed * 0x24a1 + 0x24df) & 0xffffffffUL;
    *seed = (unsigned long)GFROR13(*seed) & 0xffffffffUL;
    return range ? *seed % range : 0;
}

class CDefinitionManager;
class CWorld {
public:
    unsigned long Seed;
    unsigned long* GetWorldSeed() const { return const_cast<unsigned long*>(&Seed); }
};

struct CRefCounted { long Count; void DecRef() { --Count; } long GetRefCount() const { return Count; } };

static int g_destroyed;
class CSimBuildingDef {
public:
    virtual ~CSimBuildingDef() { }
    virtual void Destroy() const { ++g_destroyed; }
    mutable CRefCounted Ref;
    long Property, WorkSpaces, LiveSpaces;
    void ReleaseTemporary() const { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};

template <class T> class CDefPointer {
public:
    T* Object;
    explicit CDefPointer(T* o) : Object(o) { ++o->Ref.Count; }      // test stand-in for PeekPSimDefinition's result
    ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    T* operator->() const { return Object; }
};

class CThingBuilding {
public:
    CSimBuildingDef Def;
    CDefPointer<const CSimBuildingDef> PeekPSimDefinition() const { return CDefPointer<const CSimBuildingDef>(&Def); }
};

template <class T> class CIntelligentPointer {
public:
    T* Item;
    CIntelligentPointer() : Item(0) { }
    CIntelligentPointer(T* item) : Item(item) { }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { Item = other.Item; return *this; }
};

enum ESex { SEX_NONE = 0, SEX_MALE = 1, SEX_FEMALE = 2 };
enum ECreatureType { CREATURE_TYPE_0 = 0, CREATURE_TYPE_1 = 1 };
class CThingAICreature {
public:
    ESex Sex;
    ECreatureType Type;
    CIntelligentPointer<CThingBuilding> HomeBuilding;
    ESex GetSex() const { return Sex; }
    ECreatureType GetCreatureType() const { return Type; }
};

namespace stl {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : public _List_node_base { T _M_data; };
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T> class list {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    _List_node_base* _M_data;
    iterator begin() const { return iterator(_M_data->_M_next); }
    iterator end() const { return iterator(_M_data); }
    unsigned int size() const
    {
        unsigned int n = 0;
        for (_List_node_base* p = _M_data->_M_next; p != _M_data; p = p->_M_next) ++n;
        return n;
    }
    void push_back(const T& x)
    {
        _Node* tmp = (_Node*)malloc(sizeof(_Node));
        tmp->_M_data = x;
        _List_node_base* n = _M_data;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n; tmp->_M_prev = p; p->_M_next = tmp; n->_M_prev = tmp;
    }
};
template <class K, class T> class map {                 // linear stand-in: operator[] inserts an empty list
public:
    K Keys[8];
    T Values[8];
    int Count;
    map() : Count(0) { }
    T& operator[](const K& k)
    {
        for (int i = 0; i < Count; ++i)
            if (Keys[i] == k) return Values[i];
        Keys[Count] = k;
        _List_node_base* s = (_List_node_base*)malloc(sizeof(_List_node_base));
        s->_M_next = s->_M_prev = s;
        Values[Count]._M_data = s;
        return Values[Count++];
    }
};
template <class T> class vector {
public:
    typedef T* iterator;
    T Store[8];
    T* _M_start;
    T* _M_finish;
    vector() : _M_start(Store), _M_finish(Store) { }
    void push(T v) { *_M_finish++ = v; }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    iterator erase(iterator position)
    {
        if (end() != position + 1)
            memmove(position, position + 1, (const char*)_M_finish - (const char*)(position + 1));
        --_M_finish;
        return position;
    }
};
}

typedef stl::map<CThingBuilding*, stl::list<CThingAICreature*> > TOccupants;
typedef stl::vector<CThingBuilding*> TBuildings;

void AddToRelevantBuilding(CThingAICreature* creature, TOccupants& occupants, TBuildings& buildings,
    const CDefinitionManager* defs, CWorld& world)
{
    TBuildings::iterator chosen = 0;
    long mostFree = 0;
    bool coin = GFRandom(2, world.GetWorldSeed()) == 0;
    TBuildings::iterator it;
    for (it = buildings.begin(); it != buildings.end(); ++it) {
        CDefPointer<const CSimBuildingDef> def = (*it)->PeekPSimDefinition();
        if (def->LiveSpaces > 1 && occupants[*it].size() == 0) {
            chosen = it;
            break;
        }
    }
    if (chosen == 0) {
        for (it = buildings.begin(); it != buildings.end(); ++it) {
            CDefPointer<const CSimBuildingDef> def = (*it)->PeekPSimDefinition();
            long spaces = def->LiveSpaces - occupants[*it].size();
            if (spaces > 0) {
                if (occupants[*it].size() == 0) {
                    if (def->LiveSpaces > 1 || creature->GetCreatureType() == 1 || coin) {
                        chosen = it;
                        break;
                    }
                }
                long males = 0;
                long females = 0;
                for (stl::list<CThingAICreature*>::iterator c = occupants[*it].begin(); c != occupants[*it].end(); ++c) {
                    if ((*c)->GetSex() == SEX_MALE)
                        ++males;
                    else if ((*c)->GetSex() == SEX_FEMALE)
                        ++females;
                }
                if ((females < males && creature->GetSex() == SEX_FEMALE) || (females > males && creature->GetSex() == SEX_MALE)) {
                    chosen = it;
                    break;
                }
                if (spaces > mostFree) {
                    mostFree = spaces;
                    chosen = it;
                }
            }
        }
    }
    if (chosen == 0)
        return;
    creature->HomeBuilding = CIntelligentPointer<CThingBuilding>(*chosen);
    occupants[*chosen].push_back(creature);
    CDefPointer<const CSimBuildingDef> def = (*chosen)->PeekPSimDefinition();
    if (occupants[*chosen].size() == (unsigned int)def->LiveSpaces)
        buildings.erase(chosen);
}

static int fail(const char* why) { std::printf("00502b00_TEST FAIL %s\n", why); return 1; }
static void setup(CThingBuilding& b, long live) { b.Def.Ref.Count = 1; b.Def.LiveSpaces = live; }
static unsigned long seedFor(bool wantCoin)
{
    for (unsigned long s = 1; ; ++s) {
        unsigned long t = s;
        if ((GFRandom(2, &t) == 0) == wantCoin) return s;
    }
}

int main()
{
    CDefinitionManager* defs = 0;
    CWorld world;
    // 1. an empty multi-family house wins straight away, and stays a candidate while it has room
    {
        CThingBuilding a, b; setup(a, 2); setup(b, 1);
        TOccupants occ; TBuildings v; v.push(&b); v.push(&a);
        CThingAICreature c; c.Sex = SEX_MALE; c.Type = CREATURE_TYPE_0;
        world.Seed = 7; unsigned long expect = 7; GFRandom(2, &expect);
        AddToRelevantBuilding(&c, occ, v, defs, world);
        if (world.Seed != expect) return fail("seed not advanced");
        if (c.HomeBuilding.Item != &a || occ[&a].size() != 1 || *occ[&a].begin() != &c) return fail("case 1 home");
        if (v.end() - v.begin() != 2) return fail("case 1 erased");
        if (a.Def.Ref.Count != 1 || b.Def.Ref.Count != 1 || g_destroyed) return fail("case 1 def refs");
    }
    // 2. an empty one-family house takes a type-1 creature and, now full, leaves the candidates
    {
        CThingBuilding b; setup(b, 1);
        TOccupants occ; TBuildings v; v.push(&b);
        CThingAICreature c; c.Sex = SEX_FEMALE; c.Type = CREATURE_TYPE_1;
        world.Seed = seedFor(false);
        AddToRelevantBuilding(&c, occ, v, defs, world);
        if (c.HomeBuilding.Item != &b || occ[&b].size() != 1) return fail("case 2 home");
        if (v.end() != v.begin()) return fail("case 2 not erased");
    }
    // 3./4. a woman: the coin sends her to the empty cottage, else she balances the man's house
    for (int coin = 0; coin < 2; ++coin) {
        CThingBuilding cottage, house; setup(cottage, 1); setup(house, 2);
        TOccupants occ; TBuildings v; v.push(&cottage); v.push(&house);
        CThingAICreature man; man.Sex = SEX_MALE; man.Type = CREATURE_TYPE_0;
        occ[&house].push_back(&man);
        CThingAICreature woman; woman.Sex = SEX_FEMALE; woman.Type = CREATURE_TYPE_0;
        world.Seed = seedFor(coin != 0);
        AddToRelevantBuilding(&woman, occ, v, defs, world);
        CThingBuilding* want = coin ? &cottage : &house;
        if (woman.HomeBuilding.Item != want) return fail(coin ? "case 4 coin" : "case 3 balance");
        if (v.end() - v.begin() != 1 || *v.begin() != (coin ? &house : &cottage)) return fail("case 3/4 erase");
        if (cottage.Def.Ref.Count != 1 || house.Def.Ref.Count != 1) return fail("case 3/4 def refs");
    }
    // 5. a man with no coin: the cottage by free room (the house's one free place does not beat it)
    {
        CThingBuilding cottage, house; setup(cottage, 1); setup(house, 2);
        TOccupants occ; TBuildings v; v.push(&cottage); v.push(&house);
        CThingAICreature man; man.Sex = SEX_MALE; man.Type = CREATURE_TYPE_0;
        occ[&house].push_back(&man);
        CThingAICreature man2; man2.Sex = SEX_MALE; man2.Type = CREATURE_TYPE_0;
        world.Seed = seedFor(false);
        AddToRelevantBuilding(&man2, occ, v, defs, world);
        if (man2.HomeBuilding.Item != &cottage) return fail("case 5 most free");
    }
    // 6. nothing has room: no home
    {
        CThingBuilding full; setup(full, 1);
        TOccupants occ; TBuildings v; v.push(&full);
        CThingAICreature man; man.Sex = SEX_MALE; man.Type = CREATURE_TYPE_0;
        occ[&full].push_back(&man);
        CThingAICreature other; other.Sex = SEX_FEMALE; other.Type = CREATURE_TYPE_0;
        world.Seed = seedFor(true);
        AddToRelevantBuilding(&other, occ, v, defs, world);
        if (other.HomeBuilding.Item != 0 || occ[&full].size() != 1 || v.end() - v.begin() != 1) return fail("case 6 full");
    }
    if (g_destroyed) return fail("a def was destroyed");
    std::printf("00502b00_TEST PASS\n");
    return 0;
}