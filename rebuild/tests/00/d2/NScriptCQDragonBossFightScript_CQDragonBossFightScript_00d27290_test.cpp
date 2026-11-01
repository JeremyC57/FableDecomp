// Behaviour test for 0x00D27290 NScript::CQ_DragonBossFightScript::~CQ_DragonBossFightScript (implicit): timers, the cs_flags
// tree (clear: erase from the root, relink the header, zero the count, free the header), both spawn-point vectors (virtual
// element dtors, then the block), then the CScriptBase base dtor.
#include <cstdio>
#include <cstddef>
static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
class CTimer { public: long TimerIndex; ~CTimer() { Log(1000 + (int)TimerIndex); } };
class CScriptThing { public:
    int Id;
    static void* operator new(std::size_t, void* p) { return p; }
    static void operator delete(void*, void*) { }
    static void operator delete(void* p) { ::operator delete(p); }
    virtual ~CScriptThing() { Log(Id); }
};
struct Node { bool Color; Node* Parent; Node* Left; Node* Right; };
struct FlagTree {                                    // STLport _Rb_tree<CCharString, pair<const CCharString,bool>, ..>
    Node* Header; unsigned Count;
    void Erase(Node* x) { while (x) { Erase(x->Right); Node* y = x->Left; Log(500); delete x; x = y; } }
    ~FlagTree() { if (Count != 0) { Erase(Header->Parent); Header->Left = Header; Header->Parent = 0; Header->Right = Header; Count = 0; } Log(600); delete Header; }
};
struct ThingVector {                                 // STLport vector<CScriptThing>
    CScriptThing* Start; CScriptThing* Finish; int Tag;
    ~ThingVector() { for (CScriptThing* p = Start; p != Finish; ++p) p->~CScriptThing(); if (Start) { Log(700 + Tag); ::operator delete(Start); } }
};
class CScriptBase { public: virtual ~CScriptBase() { Log(9999); } };
struct CQ_DragonBossFightScript : public CScriptBase {
    void* Game; void* MasterScriptData; int DragonState;
    ThingVector MinionSpawnPoints; ThingVector SummonerSpawnPoints;
    long Counters[7]; bool MinionSpawningEnabled, SummonerSpawningEnabled;
    FlagTree cs_flags; CTimer MinionSpawnDelay; CTimer SummonerSpawnDelay;
};
static void Fill(ThingVector& v, int base, int n, int tag)
{
    v.Tag = tag;
    v.Start = (CScriptThing*)::operator new(sizeof(CScriptThing) * (n ? n : 1));
    for (int i = 0; i < n; ++i) { new (&v.Start[i]) CScriptThing; v.Start[i].Id = base + i; }
    v.Finish = v.Start + n;
}
int main()
{
    {
        CQ_DragonBossFightScript s;
        Fill(s.MinionSpawnPoints, 10, 2, 1);
        Fill(s.SummonerSpawnPoints, 20, 1, 2);
        Node* h = new Node; Node* r = new Node; Node* l = new Node;
        h->Parent = r; h->Left = l; h->Right = r; r->Parent = h; r->Left = l; r->Right = 0; l->Parent = r; l->Left = 0; l->Right = 0;
        s.cs_flags.Header = h; s.cs_flags.Count = 2;
        s.MinionSpawnDelay.TimerIndex = 3; s.SummonerSpawnDelay.TimerIndex = 4;
    }
    static const int expect[] = { 1004, 1003, 500, 500, 600, 20, 702, 10, 11, 701, 9999 };
    if (g_n != 11) { std::printf("00d27290_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 11; ++i) if (g_log[i] != expect[i]) { std::printf("00d27290_TEST FAIL\n"); return 1; }
    std::printf("00d27290_TEST PASS\n");
    return 0;
}