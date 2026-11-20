// Behaviour test for 0x00A793D0 CNavQuadTree::ConnectHorizantalMapEdge (generated from the twins' common test text; VERTICAL = 0).
#define VERTICAL 0
#define PASS_TEXT "00a793d0_TEST PASS\n"
#define FAIL_TEXT "00a793d0_TEST FAIL\n"
// Shared body of the behaviour tests for 0x00A793D0 CNavQuadTree::ConnectHorizantalMapEdge and 0x00A79680 ConnectVerticalMapEdge
// (each test links the REAL row object: its STLport vector push_back / _M_insert_overflow / uninitialized copy / destroy, the counted
// pointer and make_pair COMDATs run as compiled). Define VERTICAL (0 / 1), ROW_NAME and PASS_TEXT / FAIL_TEXT before including.
// Fixtures (raw native layout, every offset read by an instruction of the row): CNavQuadTree MapPos +0x07 / +0x0B (packed C2DVector,
// 0x00A793D6 / 0x00A79686), MapWidth +0x10, MapHeight +0x14, NavigationLayers start +0x1C / finish +0x20 (0x28-byte elements: the
// row divides by 40), ConnectorNodes vector start +0x78 / finish +0x7C / end +0x80 and Dirty +0x85 (0x00A79506 `add ebx,78h`,
// [ebx+0Dh]); node vptr slot 5 GetRegionID (+0x14), QuadTreeLevel +0x0C, Blocked +0x0F; the node-size table is provided here.
// The two GetNodeAt lookups, the connector ctor / dtor and GetRegionID log every call (receiver, position, layer); an independent
// reference model of the edge walk (float arithmetic as stored floats) predicts the event list, the appended (key, connector)
// entries with their counted pointers, Dirty, and that the entries already in the vector keep their values and counts.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

extern const float g_QuadTreeLevelSizes[7] = { 32.0f, 16.0f, 8.0f, 4.0f, 2.0f, 1.0f, 0.5f };
class C2DVector { public: float X, Y; };
class CANavQuadTreeNode;
class CNavigatorManager { public: const CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer) const; };
class CNavQuadTree {
    void ConnectHorizantalMapEdge(float this_map_y, float adjacent_map_y, const CNavigatorManager& nav_manager);
    void ConnectVerticalMapEdge(float this_map_x, float adjacent_map_x, const CNavigatorManager& nav_manager);
public:
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& pos, long layer) const;
    static void Call(CNavQuadTree* t, float a, float b, const CNavigatorManager& m)
    {
#if VERTICAL
        t->ConnectVerticalMapEdge(a, b, m);
#else
        t->ConnectHorizantalMapEdge(a, b, m);
#endif
    }
};
class CNavConnectorNode {
public:
    const CANavQuadTreeNode* N1; const CANavQuadTreeNode* N2;
    CNavConnectorNode(const CANavQuadTreeNode* neighbour1, const CANavQuadTreeNode* neighbour2);
    ~CNavConnectorNode();
};

struct Ev { char t; long a, b; float x, y; long layer; };
static Ev g_log[4096]; static int g_n;
static Ev& Log(char t) { static Ev junk; if (g_n >= 4096) return junk; Ev& e = g_log[g_n++]; memset(&e, 0, sizeof e); e.t = t; return e; }
static int g_dtors;

// ---- nodes: raw {vptr, Position, QuadTreeLevel +0xC, NavLayer, LeafNode, Blocked +0xF, region id (test field)}
struct Node { void** vptr; float px, py; unsigned char level, layer, leaf, blocked; long region; };
static long __fastcall GetRegionID(Node* self, int) { Ev& e = Log('R'); e.a = (long)self; return self->region; }
static void* g_nodeVt[6];
extern "C" void __cdecl Trap() { printf("FAIL unexpected node virtual\n"); exit(3); }
enum { K = 24 };
static Node g_nbr[K], g_mine[K];
static unsigned long g_salt; static int g_nullEvery;

static int Pick(float pos, long layer, unsigned long salt)
{
    long cell = (long)floor((double)pos * 4.0);
    unsigned long h = (unsigned long)cell * 2654435761ul + (unsigned long)layer * 40503ul + salt;
    h ^= h >> 13; h *= 2246822519ul; h ^= h >> 16;
    return (int)(h % (K + (unsigned long)g_nullEvery));
}
static const Node* Nbr(float x, float y, long layer) { int i = Pick(VERTICAL ? y : x, layer, g_salt); return i < K ? &g_nbr[i] : 0; }
static const Node* Mine(float x, float y, long layer) { return &g_mine[Pick(VERTICAL ? y : x, layer, g_salt * 7u + 3u) % K]; }

static unsigned char g_mgr[4], g_tree[0x88];
const CANavQuadTreeNode* CNavigatorManager::GetNodeAt(const C2DVector& p, unsigned char layer) const
{ Ev& e = Log('M'); e.a = (long)this; e.x = p.X; e.y = p.Y; e.layer = layer; return (const CANavQuadTreeNode*)Nbr(p.X, p.Y, layer); }
const CANavQuadTreeNode* CNavQuadTree::GetNodeAt(const C2DVector& p, long layer) const
{ Ev& e = Log('Q'); e.a = (long)this; e.x = p.X; e.y = p.Y; e.layer = layer; return (const CANavQuadTreeNode*)Mine(p.X, p.Y, layer); }
CNavConnectorNode::CNavConnectorNode(const CANavQuadTreeNode* n1, const CANavQuadTreeNode* n2)
{ N1 = n1; N2 = n2; Ev& e = Log('K'); e.a = (long)n1; e.b = (long)n2; }
CNavConnectorNode::~CNavConnectorNode() { ++g_dtors; }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

struct Entry { long key; CNavConnectorNode* data; unsigned long* info; };
struct Cfg { float mapX, mapY, width, height, thisC, adjC; int layers; int preexisting; int capacity; };

// reference model of the walk
static int Model(const Cfg& c, Ev* ev, long* keys, const Node** n1s, const Node** n2s, int& nk)
{
    int n = 0; nk = 0;
    float lo = VERTICAL ? c.mapY : c.mapX;
    float hi = VERTICAL ? (c.height + c.mapY) : (c.width + c.mapX);
    float start = lo + 0.25f;
    for (long layer = 0; layer < c.layers; ++layer) {
        for (float w = start; w < hi; ) {
            float nx = VERTICAL ? c.adjC : w, ny = VERTICAL ? w : c.adjC;
            float mx = VERTICAL ? c.thisC : w, my = VERTICAL ? w : c.thisC;
            Ev* e = &ev[n++]; memset(e, 0, sizeof *e); e->t = 'M'; e->a = (long)g_mgr; e->x = nx; e->y = ny; e->layer = layer & 0xFF;
            const Node* nb = Nbr(nx, ny, layer);
            if (!nb) { w = w + 0.5f; continue; }
            float nsize = g_QuadTreeLevelSizes[nb->level];
            e = &ev[n++]; memset(e, 0, sizeof *e); e->t = 'Q'; e->a = (long)g_tree; e->x = mx; e->y = my; e->layer = layer;
            const Node* mn = Mine(mx, my, layer);
            float msize = g_QuadTreeLevelSizes[mn->level];
            if (!nb->blocked && !mn->blocked) {
                e = &ev[n++]; memset(e, 0, sizeof *e); e->t = 'R'; e->a = (long)nb;
                e = &ev[n++]; memset(e, 0, sizeof *e); e->t = 'K'; e->a = (long)mn; e->b = (long)nb;
                keys[nk] = (nb->region >> 16) & 0xFFFF; n1s[nk] = mn; n2s[nk] = nb; ++nk;
                w = w + (nsize < msize ? nsize : msize);
            } else if (nb->blocked && mn->blocked)
                w = w + (msize < nsize ? nsize : msize);
            else if (nb->blocked)
                w = w + nsize;
            else
                w = w + msize;
            if (n > 4000) return n;
        }
    }
    return n;
}

static void Run(const Cfg& c, int k)
{
    memset(g_tree, 0, sizeof g_tree);
    memcpy(g_tree + 0x07, &c.mapX, 4); memcpy(g_tree + 0x0B, &c.mapY, 4); memcpy(g_tree + 0x10, &c.width, 4); memcpy(g_tree + 0x14, &c.height, 4);
    static unsigned char layers[40 * 8];
    unsigned char* ls = layers; unsigned char* lf = layers + 40 * c.layers;
    memcpy(g_tree + 0x1C, &ls, 4); memcpy(g_tree + 0x20, &lf, 4);
    // the connector map: `preexisting` entries in malloc storage of `capacity` slots (0 / 0: empty, the first insert overflows)
    Entry* v = c.capacity ? (Entry*)malloc(sizeof(Entry) * c.capacity) : 0;
    unsigned long pre[16][3]; CNavConnectorNode* preData[16];
    for (int i = 0; i < c.preexisting; ++i) {
        unsigned long* info = (unsigned long*)malloc(12); info[0] = 1; info[1] = 0x1234; info[2] = 0;
        preData[i] = (CNavConnectorNode*)(0x100000 + i * 16); info[2] = (unsigned long)preData[i];
        v[i].key = 1000 + i; v[i].data = preData[i]; v[i].info = info; pre[i][0] = (unsigned long)info;
    }
    Entry* vs = v; Entry* vf = v + c.preexisting; Entry* ve = v + c.capacity;
    memcpy(g_tree + 0x78, &vs, 4); memcpy(g_tree + 0x7C, &vf, 4); memcpy(g_tree + 0x80, &ve, 4);
    g_tree[0x84] = 0x5A; g_tree[0x85] = 0;
    g_n = 0; g_dtors = 0;
    CNavQuadTree::Call((CNavQuadTree*)(void*)g_tree, c.thisC, c.adjC, *(const CNavigatorManager*)(void*)g_mgr);
    static Ev exp[4096]; static long keys[2048]; static const Node* n1s[2048]; static const Node* n2s[2048]; int nk;
    int n = Model(c, exp, keys, n1s, n2s, nk);
    check(g_n == n, "event count", k);
    for (int i = 0; i < n && i < g_n; ++i) {
        const Ev& a = g_log[i]; const Ev& b = exp[i];
        bool ok = a.t == b.t && a.a == b.a && a.b == b.b &&
                  (b.t == 'M' || b.t == 'Q' ? memcmp(&a.x, &b.x, 4) == 0 && memcmp(&a.y, &b.y, 4) == 0 && a.layer == b.layer : true);
        if (!ok) { check(false, "event", k); printf("  case %d event %d: got %c(%lx,%lx,%.9g,%.9g,%ld) want %c(%lx,%lx,%.9g,%.9g,%ld)\n", k, i,
                                                 a.t, a.a, a.b, a.x, a.y, a.layer, b.t, b.a, b.b, b.x, b.y, b.layer); break; }
    }
    memcpy(&vs, g_tree + 0x78, 4); memcpy(&vf, g_tree + 0x7C, 4); memcpy(&ve, g_tree + 0x80, 4);
    check(vf - vs == c.preexisting + nk, "entry count", k);
    check(ve >= vf, "capacity", k);
    check(g_tree[0x85] == (nk ? 1 : 0), "Dirty", k);
    check(g_tree[0x84] == 0x5A, "Compare untouched", k);
    check(g_dtors == 0, "no connector destroyed", k);
    if (vf - vs == c.preexisting + nk) {
        for (int i = 0; i < c.preexisting; ++i)
            check(vs[i].key == 1000 + i && vs[i].data == preData[i] && (unsigned long)vs[i].info == pre[i][0] && vs[i].info[0] == 1,
                  "pre-existing entry kept", k);
        for (int i = 0; i < nk; ++i) {
            const Entry& e = vs[c.preexisting + i];
            bool ok = e.key == keys[i] && e.data && e.data->N1 == (const CANavQuadTreeNode*)n1s[i] &&
                      e.data->N2 == (const CANavQuadTreeNode*)n2s[i] && e.info && e.info[0] == 1 &&
                      e.info[2] == (unsigned long)e.data && e.info[1] != 0;
            if (!ok) { check(false, "new entry", k); printf("  case %d entry %d: key %ld want %ld\n", k, i, e.key, keys[i]); break; }
        }
    }
}

static unsigned long g_rs = 99991;
static unsigned long Rnd() { g_rs = g_rs * 1103515245ul + 12345ul; return g_rs >> 8; }
static void MakeNodes(int levelMax, int blockedPct)
{
    for (int i = 0; i < K; ++i) {
        Node* ns[2] = { &g_nbr[i], &g_mine[i] };
        for (int j = 0; j < 2; ++j) {
            Node* p = ns[j]; memset(p, 0, sizeof *p); p->vptr = g_nodeVt;
            p->level = (unsigned char)(Rnd() % (unsigned long)(levelMax + 1)); p->blocked = (Rnd() % 100) < (unsigned long)blockedPct;
            p->region = (long)(Rnd() * 2654435761ul);
        }
    }
}

int main()
{
    for (int i = 0; i < 6; ++i) g_nodeVt[i] = (void*)Trap;
    g_nodeVt[5] = (void*)GetRegionID;
    Cfg c;
    // targeted: one layer of fine nodes, all free -- every step connects (min of the two sizes)
    MakeNodes(6, 0); g_salt = 1; g_nullEvery = 0;
    c.mapX = 10.0f; c.mapY = -20.0f; c.width = 6.0f; c.height = 5.0f; c.thisC = 3.5f; c.adjC = 4.25f; c.layers = 1; c.preexisting = 0; c.capacity = 0;
    Run(c, 1);
    MakeNodes(6, 100); Run(c, 2);                                         // all blocked: max steps, no connector
    MakeNodes(6, 50); g_nullEvery = 6; c.layers = 3; c.preexisting = 2; c.capacity = 3; Run(c, 3);   // nulls (0.5 step), 3 layers, overflow after 1
    c.layers = 0; Run(c, 4);                                              // no layers: nothing
    c.layers = 2; c.width = 0.25f; c.height = 0.25f; Run(c, 5);           // start == end: the strict `<` makes no step
    c.width = 0.2500001f; c.height = 0.2500001f; c.preexisting = 0; c.capacity = 64; Run(c, 6);
    MakeNodes(0, 30); c.width = 200.0f; c.height = 200.0f; c.mapX = -100.5f; c.mapY = 33.25f; Run(c, 7);   // level-0 32-unit nodes
    // region ids with the sign bit: the high word is masked after the arithmetic shift
    MakeNodes(6, 20); for (int i = 0; i < K; ++i) g_nbr[i].region = (long)(0x80000000ul | (unsigned long)i * 0x10001ul);
    c.width = 30.0f; c.height = 30.0f; c.mapX = 1.0f; c.mapY = 2.0f; c.thisC = -7.0f; c.adjC = 9.5f; c.layers = 2; c.preexisting = 0; c.capacity = 0;
    Run(c, 8);
    // random sweep
    for (int k = 0; k < 400; ++k) {
        MakeNodes((int)(Rnd() % 7), (int)(Rnd() % 101));
        g_salt = Rnd(); g_nullEvery = (int)(Rnd() % 10);
        c.mapX = (float)((long)(Rnd() % 4001) - 2000) / 8.0f; c.mapY = (float)((long)(Rnd() % 4001) - 2000) / 8.0f;
        c.width = (float)(Rnd() % 641) / 16.0f; c.height = (float)(Rnd() % 641) / 16.0f;
        c.thisC = (float)((long)(Rnd() % 2001) - 1000) / 4.0f; c.adjC = (float)((long)(Rnd() % 2001) - 1000) / 4.0f;
        c.layers = (int)(Rnd() % 5); c.capacity = (int)(Rnd() % 6); c.preexisting = c.capacity ? (int)(Rnd() % (unsigned long)(c.capacity + 1)) : 0;
        if (c.preexisting > 5) c.preexisting = 5;
        Run(c, 100 + k);
    }
    printf(failures ? FAIL_TEXT : PASS_TEXT);
    return failures ? 1 : 0;
}
