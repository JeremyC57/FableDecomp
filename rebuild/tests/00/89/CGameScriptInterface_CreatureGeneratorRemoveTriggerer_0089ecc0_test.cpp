#include <cstdio>
#if !defined(_MSC_VER)
#  define __fastcall
#  define __stdcall
#  define __forceinline inline
#endif

static int  g_consumed;
static void* g_consumedSelf;
static unsigned long g_findKey;

struct BObj {
    unsigned char pad[0x6c];
    unsigned char flag6c;
    void consume(BObj* b);   // real member => thiscall, arg on stack
};
struct Node { int key; BObj* value; };
struct MapT {
    Node* first;
    Node* end;
    Node* find(const unsigned long& key);    // the map lower bound (0x0040F020), key by address
    __forceinline BObj* Find(long kind)   // the inlined map find: end when the key is absent
    {
        Node* it = find((unsigned long)kind);
        if (it == end || kind < it->key)
            it = end;
        return it->value;
    }
};
struct AObj {
    unsigned char pad0[0x24];
    unsigned int field24; // read as [esi+0x24]
    unsigned char pad2[0x1c];
    MapT map44;           // at +0x44
};
template <class T> __forceinline bool GetTCOut(AObj* thing, long kind, T** out)
{
    if (thing->field24 & 0x20000000) {
        *out = (T*)thing->map44.Find(kind);
        return true;
    }
    return false;
}
struct Host { void** vt; };

typedef AObj* (__fastcall *GetA_t)(void*);
typedef BObj* (__fastcall *GetB_t)(void*);

void __stdcall CGameScriptInterface_CreatureGeneratorRemoveTriggerer(Host* p0, Host* p1)
{
    AObj* a = ((GetA_t)(p0->vt[0x0b]))(p0);
    BObj* b = ((GetB_t)(p1->vt[0x0b]))(p1);

    BObj* val;
    if (GetTCOut(a, 0x3d, &val) && (b->flag6c & 8))
        val->consume(b);
}


static Node g_end;
static Node g_hit;
static BObj g_valObj;

Node* MapT::find(const unsigned long& key) { g_findKey = key; return &g_hit; }
void  BObj::consume(BObj* b) { (void)b; g_consumed = 1; g_consumedSelf = this; }

static AObj g_a;
static BObj g_b;
static AObj* __fastcall getA(void* self) { (void)self; return &g_a; }
static BObj* __fastcall getB(void* self) { (void)self; return &g_b; }

int main()
{
    void* vtA[16]; void* vtB[16];
    vtA[0x0b] = (void*)(GetA_t)getA;
    vtB[0x0b] = (void*)(GetB_t)getB;
    Host h0; h0.vt = vtA;
    Host h1; h1.vt = vtB;

    g_a.field24 = 0x20000000;
    g_a.map44.end = &g_end;
    g_hit.key = 0x10;
    g_hit.value = &g_valObj;
    g_b.flag6c = 0x08;

    g_consumed = 0; g_consumedSelf = 0;
    CGameScriptInterface_CreatureGeneratorRemoveTriggerer(&h0, &h1);
    if (g_consumed != 1 || g_findKey != 0x3d) { std::printf("FAIL: not consumed\n"); return 1; }
    if (g_consumedSelf != (void*)&g_valObj) { std::printf("FAIL: wrong self\n"); return 1; }

    // key > 0x3d -> forced to end; end's value used
    g_end.value = 0;
    g_hit.key = 0x50;
    g_consumed = 0; g_consumedSelf = 0;
    CGameScriptInterface_CreatureGeneratorRemoveTriggerer(&h0, &h1);
    if (g_consumed != 1) { std::printf("FAIL: case2 not consumed\n"); return 1; }
    if (g_consumedSelf != (void*)0) { std::printf("FAIL: case2 wrong self\n"); return 1; }

    // the triggerer flag clear -> no call
    g_hit.key = 0x3d;
    g_b.flag6c = 0;
    g_consumed = 0;
    CGameScriptInterface_CreatureGeneratorRemoveTriggerer(&h0, &h1);
    if (g_consumed != 0) { std::printf("FAIL: case3 consumed\n"); return 1; }

    // component bit clear -> no-op
    g_b.flag6c = 0x08;
    g_a.field24 = 0;
    g_consumed = 0;
    CGameScriptInterface_CreatureGeneratorRemoveTriggerer(&h0, &h1);
    if (g_consumed != 0) { std::printf("FAIL: case4 consumed\n"); return 1; }

    std::printf("CGameScriptInterface_0089ecc0_TEST PASS\n");
    return 0;
}
