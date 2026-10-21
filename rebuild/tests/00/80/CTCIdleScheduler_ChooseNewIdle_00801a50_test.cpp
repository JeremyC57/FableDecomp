#include <cstdio>
// Behaviour test for CTCIdleScheduler::ChooseNewIdle (0x00801A50): same body as src/ over a hand-built STLport tree
// of idle groups, with the def lookups, weight maps and world frame stubbed.
struct CAIStateGroupBase { bool runnable; bool CanRun() { return runnable; } };
struct CWorld { static long frame; static long GetWorldFrame() { return frame; } };
long CWorld::frame = 0;
static int g_destroyed = 0;
struct CDefClassBase {
    long RefCount;
    void Destroy() { ++g_destroyed; }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
};
struct SWeightNode { long first; long second; };
struct CWeightMapIterator {
    SWeightNode* _M_node;
    CWeightMapIterator(SWeightNode* node) : _M_node(node) { }
};
struct CWeightMap {                                   // flat stand-in: entries[0..count), header = &entries[count]
    SWeightNode* _M_header;
    SWeightNode entries[8];
    int count;
    CWeightMap() : count(0) { _M_header = &entries[0]; }
    CWeightMapIterator find(const long& key)
    {
        for (int i = 0; i < count; ++i) if (entries[i].first == key) return CWeightMapIterator(&entries[i]);
        return CWeightMapIterator(_M_header);
    }
    long& operator[](const long& key)
    {
        for (int i = 0; i < count; ++i) if (entries[i].first == key) return entries[i].second;
        entries[count].first = key; entries[count].second = 0;
        _M_header = &entries[count + 1];
        return entries[count++].second;
    }
};
static inline bool Contains(CWeightMap& map, const long& key)
{
    CWeightMapIterator it = map.find(key);
    return it._M_node != map._M_header;
}
struct SIdleNode {
    long color; SIdleNode* parent; SIdleNode* left; SIdleNode* right;
    long first; CAIStateGroupBase* second;
};
struct CIdleMapIterator {
    SIdleNode* _M_node;
    CIdleMapIterator(SIdleNode* node) : _M_node(node) { }
    static SIdleNode* Increment(SIdleNode* node)
    {
        if (node->right != 0) {
            node = node->right;
            while (node->left != 0)
                node = node->left;
        } else {
            SIdleNode* y = node->parent;
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y)
                node = y;
        }
        return node;
    }
    void operator++() { _M_node = Increment(_M_node); }
    bool operator!=(const CIdleMapIterator& other) const { return _M_node != other._M_node; }
    SIdleNode* operator->() const { return _M_node; }
};
struct CIdleMap {
    SIdleNode* _M_header;
    CIdleMapIterator begin() { return CIdleMapIterator(_M_header->left); }
    CIdleMapIterator end() { return CIdleMapIterator(_M_header); }
};
struct CIdleSchedulerDef : CDefClassBase { CWeightMap IdleWeights; };
static CIdleSchedulerDef* g_subDef = 0;
struct CThingDef : CDefClassBase {
    bool GetSubDef(CDefPointer<CIdleSchedulerDef>* out)
    {
        out->Object = g_subDef;
        if (g_subDef) ++g_subDef->RefCount;
        return g_subDef != 0;
    }
};
static CThingDef g_thingDef;
static long g_requestedIndex = -1;
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingDef>* out)
    {
        g_requestedIndex = index;
        out->Object = &g_thingDef; ++g_thingDef.RefCount;
        return true;
    }
};
static CDefinitionManager g_manager;
static CWorld g_world;
struct CThing { unsigned short DefIndex; long GetDefIndex() const { return DefIndex; } };
struct CTCIdleScheduler {
    CThing*       ParentThing;
    CIdleMap      IdleStates;
    CWeightMap    IdleStatesWeight;
    long          Preferred;
    long          FrameLastChecked;
    CWorld* GetWorld() const { return &g_world; }
    CDefinitionManager* GetDefinitions() const { return &g_manager; }
    void ChooseNewIdle();
};
void CTCIdleScheduler::ChooseNewIdle()
{
    FrameLastChecked = GetWorld()->GetWorldFrame();
    if (Preferred != 0 && ParentThing->DefIndex != 0) {
        CDefPointer<CIdleSchedulerDef> idleDef;
        CDefPointer<CThingDef> thingDef;
        CThing* thing = ParentThing;
        GetDefinitions()->GetDef(thing->GetDefIndex(), &thingDef);
        if (thingDef.Object->GetSubDef(&idleDef)) {
            CWeightMap& weights = idleDef.Object->IdleWeights;
            if (Contains(weights, Preferred)) {
                long& total = IdleStatesWeight[Preferred];
                CWeightMapIterator found = weights.find(Preferred);
                total += found._M_node->second;
            }
        }
    }
    Preferred = 1;
    long bestWeight = 0;
    long best = 0;
    for (CIdleMapIterator it = IdleStates.begin(); it != IdleStates.end(); ++it) {
        if (best == 0 || IdleStatesWeight[it->first] < bestWeight) {
            if (it->second->CanRun()) {
                best = it->first;
                bestWeight = IdleStatesWeight[it->first];
            }
        }
    }
    Preferred = best;
}

int main()
{
    int fails = 0;
    CAIStateGroupBase g1 = { true }, g2 = { true }, g3 = { true };
    SIdleNode header = { 0 }, a = { 0 }, b = { 0 }, c = { 0 };
    b.parent = &header; b.left = &a; b.right = &c; b.first = 2; b.second = &g2;
    a.parent = &b; a.first = 1; a.second = &g1;
    c.parent = &b; c.first = 3; c.second = &g3;
    header.parent = &b; header.left = &a; header.right = &c;
    CThing thing = { 0x55 };
    CTCIdleScheduler s;
    s.ParentThing = &thing; s.IdleStates._M_header = &header; s.Preferred = 0; s.FrameLastChecked = 0;
    CIdleSchedulerDef idleDef; idleDef.RefCount = 1;
    idleDef.IdleWeights[2] = 5; idleDef.IdleWeights[3] = 7;
    g_subDef = &idleDef; g_thingDef.RefCount = 1;
    CWorld::frame = 42;
    s.ChooseNewIdle();                               // no preference yet: first runnable group wins (all weights 0)
    if (s.FrameLastChecked != 42 || s.Preferred != 1 || g_requestedIndex != -1) ++fails;
    s.Preferred = 2;
    s.ChooseNewIdle();                               // group 2 credited with 5; group 1 (0) still lowest
    if (s.IdleStatesWeight[2] != 5 || s.Preferred != 1 || g_requestedIndex != 0x55) ++fails;
    if (idleDef.RefCount != 1 || g_thingDef.RefCount != 1 || g_destroyed != 0) ++fails;   // both released
    s.IdleStatesWeight[1] = 9;
    s.Preferred = 3;
    s.ChooseNewIdle();                               // group 3 credited 7; weights 9/5/7 -> group 2
    if (s.IdleStatesWeight[3] != 7 || s.Preferred != 2) ++fails;
    g2.runnable = false;
    s.Preferred = 0;
    s.ChooseNewIdle();                               // 2 not runnable: lowest runnable is 3
    if (s.Preferred != 3) ++fails;
    g1.runnable = false; g3.runnable = false;
    thing.DefIndex = 0; s.Preferred = 3;
    s.ChooseNewIdle();                               // nothing runnable, no def lookup: preference cleared
    if (s.Preferred != 0 || s.IdleStatesWeight[3] != 7) ++fails;
    if (fails) { std::printf("00801a50_TEST FAIL\n"); return 1; }
    std::printf("00801a50_TEST PASS\n");
    return 0;
}