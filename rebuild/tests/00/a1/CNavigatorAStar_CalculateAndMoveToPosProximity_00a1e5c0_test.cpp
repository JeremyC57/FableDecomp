// Behaviour test for 0x00A1E5C0 CNavigatorAStar::CalculateAndMoveToPosProximity (links the REAL row object: the row body, both expanded
// IsRouteClear bodies (with their expanded C2DVector / C3DVector arithmetic, Normalise / GetSize / GetSmallestNodeSize calls and the
// GetNodeAt walk), the expanded GetNodeAt / GetMyNodeAt (interface slot 16 + CNavigatorManager::GetNodeAt), IsEqual (+ operator==), the
// STLport list bodies the row compiles (push_back / push_front / erase / pop_front / size / back / front, malloc / free of 0x14-byte nodes),
// the inline CSteeringAgent accessors and the TU-mate IsCurrentNavigabilityBetter all run as compiled). No class models: every external is
// bound by /alternatename to an extern "C" __fastcall stub (thiscall-compatible, unused edx) and every unused vtable slot to a trap. The
// navigator is a raw 0xC4-byte buffer at the engine header offsets (CNavigatorBase PInterface +0x14, OnRoute +0x18, Interrupted +0x19,
// NavigationManager +0x1C, SteeringAgent +0x20; CNavigatorAStar BlockedStartNode +0x30, BlockedEndNode +0x40, BlockedNodeAskedToGetTo
// +0x50, GoalNode +0x6C, GoalPosition +0x70, NodeAskedToGetTo +0x78, PosAskedToGetTo +0x7C, LastNavigableNodeVisited +0x88, Route +0x8C,
// CullFront / CullBack +0x90 / +0x94, LastWayPointVisited +0x98, LastKnownNavPos +0xA8, LastKnownNavSpeed +0xB4, HeadingForActionPoint +0xBC,
// GotRoute +0xBE, Culled +0xC0, PartWayThroughAStar +0xC1); quad-tree nodes are 0x10-byte objects (vptr, Position +4, NavLayer +0xD, the
// blocked flag +0xF) on a 6 x 6 x 2 grid of 10-unit cells, with virtual IsNeighbourOf / GetNodeType / GetRegion / GetPreferabilityCost.
// Reference model: Model() below, written from the source statements (including IsRouteClear's), run on a copy of the navigator with a
// deep-copied route; every run compares the return value, every byte of the navigator (node / iterator pointers translated), the route
// contents, the steering agent, resulting_vector and the full call log (kind, `this`, every argument; floats to 1e-5).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static const char* CurCase = "";
static long __stdcall Crash(void*) { printf("FAIL crash in %s\n", CurCase); fflush(stdout); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call in %s\n", CurCase); fflush(stdout); ExitProcess(3); }

#pragma comment(linker, "/alternatename:@CallRow@60=?CalculateAndMoveToPosProximity@CNavigatorAStar@@AAE?AW4ENavigatorReturn@@ABVC3DVector@@0MMAAV3@_N222222M@Z")
#pragma comment(linker, "/alternatename:??4CNavBlockedQuadTreeNode@@QAEABV0@ABV0@@Z=@BlockAssign@12")
#pragma comment(linker, "/alternatename:??8CANavQuadTreeNode@@QBE_NABV0@@Z=@NodeEq@12")
#pragma comment(linker, "/alternatename:?CalculateNavigableWidthOfLine@CNavigatorAStar@@QAEXABVC3DVector@@@Z=@WidthStub@12")
#pragma comment(linker, "/alternatename:?CalculateRoute@CNavigatorAStar@@AAE_N_N@Z=@CalcRouteStub@12")
#pragma comment(linker, "/alternatename:?CullWayPoints@CNavigatorAStar@@AAEXJ@Z=@CullStub@12")
#pragma comment(linker, "/alternatename:?DELTA_NOTIONAL_ZERO@@3MB=_g_Delta")
#pragma comment(linker, "/alternatename:?FindNearestNavigableNode@CNavigatorBase@@IBEPBVCANavQuadTreeNode@@PBV2@0ABVC3DVector@@MPBVCNavigatorManager@@@Z=@NearestStub@28")
#pragma comment(linker, "/alternatename:?GetNodeAt@CNavigatorManager@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@E@Z=@NodeAtStub@16")
#pragma comment(linker, "/alternatename:?GetSize@CANavQuadTreeNode@@QBEMXZ=@SizeStub@8")
#pragma comment(linker, "/alternatename:?GetSmallestNodeSize@CNavQuadTree@@SIMXZ=@SmallestStub@0")
#pragma comment(linker, "/alternatename:?GetSquaredDist@C2DLineF@@QAEMABVC2DVector@@@Z=@SqDistStub@12")
#pragma comment(linker, "/alternatename:?Initialise@CNavigatorAStar@@AAEXPBVCANavQuadTreeNode@@0ABVC2DVector@@10ABVC3DVector@@_N3M@Z=@InitStub@44")
#pragma comment(linker, "/alternatename:?IsActionPoint@CNavigatorManager@@QBE_NABVC2DVector@@@Z=@ActionStub@12")
#pragma comment(linker, "/alternatename:?IsAnyPartOfNodeWithinDistOfPos@CNavigatorBase@@IBE_NPBVCANavQuadTreeNode@@MABVC3DVector@@AAVC2DVector@@@Z=@AnyPartStub@24")
#pragma comment(linker, "/alternatename:?IsConnectedToRegion@CNavRegion@@QBE_NABV1@@Z=@ConnStub@12")
#pragma comment(linker, "/alternatename:?MoveTowardNextWayPoint@CNavigatorAStar@@AAEXABVC3DVector@@MMAAV2@_N222@Z=@MoveStub@40")
#pragma comment(linker, "/alternatename:?Normalise@C2DVector@@QAEAAV1@XZ=@NormStub@8")
#pragma comment(linker, "/alternatename:?SetGoal@CSteeringAgent@@QAEXABVC2DVector@@0@Z=@GoalStub@16")
#pragma comment(linker, "/alternatename:?UpdateGoal@CNavigatorAStar@@QAEXABVC3DVector@@VC2DVector@@1@Z=@UpdGoalStub@28")

#pragma pack(push, 1)
struct V2 { float X, Y; };
struct V3 { float X, Y, Z; };
struct WP { V2 Position; unsigned char NavLayer; unsigned char Removable; };
struct LNode { LNode* next; LNode* prev; WP data; };
struct Node { void** vptr; V2 Position; unsigned char Level, NavLayer, Leaf, Blocked; };
#pragma pack(pop)
extern "C" float g_Delta = 0.0001f;

// ---------------- world ----------------
enum { GW = 6, GH = 6, GL = 2, CELL = 10 };
static Node World[GL][GH][GW];
static bool  Hole[GL][GH][GW];
static unsigned char Type[GL][GH][GW], Pref[GL][GH][GW], Group[GL][GH][GW];
static unsigned char LayerAt[GH][GW];
struct Region { int group; };
static Region Regions[4];
static bool Connected[4][4];
static void* NodeVtbl[32];
static int CellOf(const Node* n, int& l, int& y, int& x)
{
    l = n->NavLayer; x = (int)floor(n->Position.X / CELL); y = (int)floor(n->Position.Y / CELL);
    return l >= 0 && l < GL && x >= 0 && x < GW && y >= 0 && y < GH;
}
static int NodeId(const void* p)
{
    for (int l = 0; l < GL; ++l) for (int y = 0; y < GH; ++y) for (int x = 0; x < GW; ++x) if (p == &World[l][y][x]) return 1 + (l * GH + y) * GW + x;
    return 0;
}
extern "C" bool __fastcall NbStub(const Node* self, void*, const Node* other)
{
    if (!other) return false;
    int l1, y1, x1, l2, y2, x2; CellOf(self, l1, y1, x1); CellOf(other, l2, y2, x2);
    int dx = abs(x1 - x2), dy = abs(y1 - y2);
    return l1 == l2 && (dx > dy ? dx : dy) == 1;
}
extern "C" long __fastcall TypeStub(const Node* self, void*) { int l, y, x; CellOf(self, l, y, x); return Type[l][y][x]; }
extern "C" const Region* __fastcall RegionStub(const Node* self, void*) { int l, y, x; CellOf(self, l, y, x); return &Regions[Group[l][y][x]]; }
extern "C" unsigned char __fastcall PrefStub(const Node* self, void*) { int l, y, x; CellOf(self, l, y, x); return Pref[l][y][x]; }

// ---------------- call log ----------------
struct SEvent { int kind; unsigned long p[10]; unsigned char isf[10]; };
enum { EV_UPDLAST = 1, EV_RADIUS, EV_LEVEL, EV_NODEAT, EV_ACTION, EV_NEAREST, EV_ANYPART, EV_CONN, EV_INIT, EV_CALCROUTE, EV_CULL, EV_GOAL,
       EV_SQDIST, EV_WIDTH, EV_MOVE, EV_UPDGOAL, EV_ASSIGN, EV_EQ, EV_SIZE, EV_SMALL, EV_NORM };
enum { MAXEV = 400 };
static SEvent LogR[MAXEV], LogM[MAXEV]; static int NR, NM;
static SEvent* Cur; static int* CurN;
static unsigned char* SelfR; static unsigned char* SelfM; static unsigned char* SelfCur;
static unsigned long Tr(const void* p)          // pointer as a stable token
{
    if (!p) return 0;
    if (SelfCur && (const unsigned char*)p >= SelfCur && (const unsigned char*)p < SelfCur + 0xC4) return 0x10000000ul + (unsigned long)((const unsigned char*)p - SelfCur);
    int id = NodeId(p); if (id) return 0x20000000ul + id;
    return (unsigned long)p;
}
static unsigned long FB(float f) { unsigned long u; memcpy(&u, &f, 4); return u; }
static SEvent& Ev(int kind)
{
    static SEvent dummy; if (*CurN >= MAXEV) { ++*CurN; return dummy; }
    SEvent& e = Cur[(*CurN)++]; memset(&e, 0, sizeof(e)); e.kind = kind; return e;
}
static void PF(SEvent& e, int i, float f) { e.p[i] = FB(f); e.isf[i] = 1; }

// ---------------- per-run script ----------------
struct SScript {
    float radius; int nearestIdx[4]; bool action; bool anyPart; V2 anyPartOut; bool initPartway, initGot;
    bool crPartway, crGot, crTouch; int crCount; WP crRoute[4]; bool cullResult; V3 moveOut;
} S;
static int NearestCalls, Seq;

// interface: slot 6 PeekNavigatorRadius(bool), slot 16 GetNavigationLevelAt(const C3DVector&)
extern "C" float __fastcall RadiusStub(void* self, void*, bool b) { SEvent& e = Ev(EV_RADIUS); e.p[0] = Tr(self); e.p[1] = b; return S.radius; }
extern "C" long __fastcall LevelStub(void* self, void*, const V3* pos)
{
    SEvent& e = Ev(EV_LEVEL); e.p[0] = Tr(self); PF(e, 1, pos->X); PF(e, 2, pos->Y); PF(e, 3, pos->Z);
    int x = (int)floor(pos->X / CELL), y = (int)floor(pos->Y / CELL);
    if (x < 0 || x >= GW || y < 0 || y >= GH) return 0;
    return LayerAt[y][x];
}
static void* IfVtbl[32]; static void* IfObj[1] = { IfVtbl };
static unsigned char Manager[16];
extern "C" const Node* __fastcall NodeAtStub(void* mgr, void*, const V2* pos, unsigned char layer)
{
    SEvent& e = Ev(EV_NODEAT); e.p[0] = Tr(mgr); PF(e, 1, pos->X); PF(e, 2, pos->Y); e.p[3] = layer;
    int x = (int)floor(pos->X / CELL), y = (int)floor(pos->Y / CELL);
    if (layer >= GL || x < 0 || x >= GW || y < 0 || y >= GH || Hole[layer][y][x]) return 0;
    return &World[layer][y][x];
}
extern "C" bool __fastcall ActionStub(void* mgr, void*, const V2* pos) { SEvent& e = Ev(EV_ACTION); e.p[0] = Tr(mgr); PF(e, 1, pos->X); PF(e, 2, pos->Y); return S.action; }
static const Node* NodeByIdx(int i) { if (i < 0) return 0; int x = i % GW, y = (i / GW) % GH, l = (i / (GW * GH)) % GL; return &World[l][y][x]; }
extern "C" const Node* __fastcall NearestStub(void* self, void*, const Node* a, const Node* b, const V3* pos, float d, const void* mgr)
{
    SEvent& e = Ev(EV_NEAREST); e.p[0] = Tr(self); e.p[1] = Tr(a); e.p[2] = Tr(b); PF(e, 3, pos->X); PF(e, 4, pos->Y); PF(e, 5, pos->Z); PF(e, 6, d); e.p[7] = Tr(mgr);
    return NodeByIdx(S.nearestIdx[(NearestCalls++) & 3]);
}
extern "C" bool __fastcall AnyPartStub(void* self, void*, const Node* n, float d, const V3* pos, V2* out)
{
    SEvent& e = Ev(EV_ANYPART); e.p[0] = Tr(self); e.p[1] = Tr(n); PF(e, 2, d); PF(e, 3, pos->X); PF(e, 4, pos->Y); PF(e, 5, pos->Z); e.p[6] = out ? 1 : 0;
    PF(e, 7, out->X); PF(e, 8, out->Y);
    *out = S.anyPartOut; return S.anyPart;
}
extern "C" bool __fastcall ConnStub(const Region* a, void*, const Region* b)
{
    SEvent& e = Ev(EV_CONN); e.p[0] = (unsigned long)(a - Regions); e.p[1] = (unsigned long)(b - Regions); return Connected[a->group][b->group];
}
extern "C" void* __fastcall BlockAssign(void* self, void*, const void* other)
{
    SEvent& e = Ev(EV_ASSIGN); e.p[0] = Tr(self); e.p[1] = Tr(other); memmove(self, other, 0x10); return self;
}
extern "C" bool __fastcall NodeEq(const Node* a, void*, const Node* b)
{
    SEvent& e = Ev(EV_EQ); e.p[0] = Tr(a); e.p[1] = Tr(b);
    return a->NavLayer == b->NavLayer && a->Position.X == b->Position.X && a->Position.Y == b->Position.Y;
}
extern "C" float __fastcall SizeStub(const Node* self, void*) { SEvent& e = Ev(EV_SIZE); e.p[0] = Tr(self); return (float)CELL; }
extern "C" float __fastcall SmallestStub() { Ev(EV_SMALL); return 2.0f; }
extern "C" V2* __fastcall NormStub(V2* v, void*)
{
    SEvent& e = Ev(EV_NORM); PF(e, 0, v->X); PF(e, 1, v->Y);
    float m = (float)sqrt(v->X * v->X + v->Y * v->Y); if (m > 0) { v->X /= m; v->Y /= m; } return v;
}
extern "C" float __fastcall SqDistStub(const V2* line, void*, const V2* p)
{
    SEvent& e = Ev(EV_SQDIST); PF(e, 0, line[0].X); PF(e, 1, line[0].Y); PF(e, 2, line[1].X); PF(e, 3, line[1].Y); PF(e, 4, p->X); PF(e, 5, p->Y);
    float dx = p->X - line[1].X, dy = p->Y - line[1].Y; return dx * dx + dy * dy;   // distance to the segment's end (test data)
}
extern "C" void __fastcall WidthStub(void* self, void*, const V3* pos) { SEvent& e = Ev(EV_WIDTH); e.p[0] = Tr(self); PF(e, 1, pos->X); PF(e, 2, pos->Y); PF(e, 3, pos->Z); }
extern "C" void __fastcall GoalStub(void* agent, void*, const V2* a, const V2* b) { SEvent& e = Ev(EV_GOAL); e.p[0] = Tr(agent); PF(e, 1, a->X); PF(e, 2, a->Y); PF(e, 3, b->X); PF(e, 4, b->Y); }
extern "C" void __fastcall UpdGoalStub(void* self, void*, const V3* pos, V2 a, V2 b)
{ SEvent& e = Ev(EV_UPDGOAL); e.p[0] = Tr(self); PF(e, 1, pos->X); PF(e, 2, pos->Y); PF(e, 3, a.X); PF(e, 4, a.Y); PF(e, 5, b.X); PF(e, 6, b.Y); }
extern "C" void __fastcall MoveStub(void* self, void*, const V3* pos, float speed, float prox, V3* out, bool a, bool b, bool c, bool d)
{
    SEvent& e = Ev(EV_MOVE); e.p[0] = Tr(self); PF(e, 1, pos->X); PF(e, 2, pos->Y); PF(e, 3, speed); PF(e, 4, prox); e.p[5] = out ? 1 : 0;
    e.p[6] = a; e.p[7] = b; e.p[8] = c; e.p[9] = d; *out = S.moveOut;
}
// ---------------- navigator layout ----------------
enum { O_IF = 0x14, O_ONROUTE = 0x18, O_INTERRUPTED = 0x19, O_MGR = 0x1C, O_AGENT = 0x20, O_BLOCKSTART = 0x30, O_BLOCKEND = 0x40,
       O_BLOCKASKED = 0x50, O_GOALNODE = 0x6C, O_GOALPOS = 0x70, O_ASKED = 0x78, O_POSASKED = 0x7C, O_LASTNAV = 0x88, O_ROUTE = 0x8C,
       O_CULLFRONT = 0x90, O_CULLBACK = 0x94, O_LASTWP = 0x98, O_LASTKNOWNPOS = 0xA8, O_LASTKNOWNSPEED = 0xB4, O_HEADING = 0xBC,
       O_GOTROUTE = 0xBE, O_CULLED = 0xC0, O_PARTWAY = 0xC1, NAV_SIZE = 0xC4 };
template <class T> static T& At(unsigned char* o, int off) { return *(T*)(o + off); }
static LNode* Sent(unsigned char* o) { return At<LNode*>(o, O_ROUTE); }
static void PushBack(unsigned char* o, const WP& w)
{ LNode* s = Sent(o); LNode* n = (LNode*)malloc(0x14); n->data = w; n->next = s; n->prev = s->prev; s->prev->next = n; s->prev = n; }
static void PushFront(unsigned char* o, const WP& w)
{ LNode* s = Sent(o); LNode* n = (LNode*)malloc(0x14); n->data = w; n->prev = s; n->next = s->next; s->next->prev = n; s->next = n; }
static void PopFront(unsigned char* o) { LNode* s = Sent(o); LNode* n = s->next; s->next = n->next; n->next->prev = s; free(n); }
static int Size(unsigned char* o) { int k = 0; for (LNode* n = Sent(o)->next; n != Sent(o); n = n->next) ++k; return k; }
static WP& Front(unsigned char* o) { return Sent(o)->next->data; }
static WP& Back(unsigned char* o) { return Sent(o)->prev->data; }
static WP& Second(unsigned char* o) { return Sent(o)->next->next->data; }
static void FreeList(unsigned char* o) { while (Size(o)) PopFront(o); free(Sent(o)); }

extern "C" void __fastcall UpdLastStub(void* self, void*, const V3* pos) { SEvent& e = Ev(EV_UPDLAST); e.p[0] = Tr(self); PF(e, 1, pos->X); PF(e, 2, pos->Y); PF(e, 3, pos->Z); }
extern "C" void __fastcall InitStub(void* self, void*, const Node* s, const Node* en, const V2* cur, const V2* dest, const Node* endn, const V3* dp,
                                    bool avoid, bool hp, float lod)
{
    SEvent& e = Ev(EV_INIT); e.p[0] = Tr(s); e.p[1] = Tr(en); PF(e, 2, cur->X); PF(e, 3, cur->Y); PF(e, 4, dest->X); PF(e, 5, dest->Y);
    e.p[6] = Tr(endn); e.p[7] = Tr(dp); e.p[8] = (avoid ? 1 : 0) | (hp ? 2 : 0); PF(e, 9, lod);
    At<unsigned char>((unsigned char*)self, O_PARTWAY) = S.initPartway; At<unsigned char>((unsigned char*)self, O_GOTROUTE) = S.initGot;
}
extern "C" bool __fastcall CalcRouteStub(void* self, void*, bool ignore)
{
    SEvent& e = Ev(EV_CALCROUTE); e.p[0] = Tr(self); e.p[1] = ignore;
    unsigned char* o = (unsigned char*)self;
    if (S.crTouch) At<unsigned char>(o, O_PARTWAY) = S.crPartway;
    At<unsigned char>(o, O_GOTROUTE) = S.crGot;
    if (S.crGot && !At<unsigned char>(o, O_PARTWAY)) { while (Size(o)) PopFront(o); for (int i = 0; i < S.crCount; ++i) PushBack(o, S.crRoute[i]); }
    return S.crGot;
}
extern "C" void __fastcall CullStub(void* self, void*, long n) { SEvent& e = Ev(EV_CULL); e.p[0] = Tr(self); e.p[1] = (unsigned long)n; At<unsigned char>((unsigned char*)self, O_CULLED) = S.cullResult; }
static void* NavVtbl[32];
static unsigned char Agent[0x70];
enum { A_BESTL = 0x40, A_BESTR = 0x44, A_NEXTL = 0x48, A_NEXTR = 0x4C, A_PROX = 0x50 };

extern "C" int __fastcall CallRow(void* self, void*, const V3* cur, const V3* dest, float prox, float speed, V3* out, bool a, bool b, bool c, bool d,
                                  bool e, bool f, bool g, float lod);

// ---------------- model (from the source statements) ----------------
static const Node* M_GetNodeAt(unsigned char* o, const V3& pos)
{
    V2 p; p.X = pos.X; p.Y = pos.Y;
    long layer = LevelStub(At<void*>(o, O_IF), 0, &pos);
    return NodeAtStub(At<void*>(o, O_MGR), 0, &p, (unsigned char)layer);
}
static bool M_IsEqual(const Node* a, const Node* b) { if (a == b) return true; if (!a || !b) return false; return NodeEq(a, 0, b); }
static bool M_Navigable(const Node* n) { return !n->Blocked; }
static long M_Type(const Node* n) { return ((long (__fastcall*)(const Node*, void*))n->vptr[6])(n, 0); }
static bool M_Neighbour(const Node* n, const Node* o) { return ((bool (__fastcall*)(const Node*, void*, const Node*))n->vptr[4])(n, 0, o); }
static const Region* M_Region(const Node* n) { return ((const Region* (__fastcall*)(const Node*, void*))n->vptr[7])(n, 0); }
static unsigned char M_Pref(const Node* n) { return ((unsigned char (__fastcall*)(const Node*, void*))n->vptr[8])(n, 0); }
static long Sign(float v) { return v < 0.0f ? -1 : 1; }
static float Dist2(V2 a, V2 b) { float dx = b.X - a.X; float dy = b.Y - a.Y; dy *= dy; dy += dx * dx; return dy; }
static bool M_IsRouteClear(unsigned char* o, const WP& from, const WP& to, bool test_layers)
{
    void* mgr = At<void*>(o, O_MGR);
    if (test_layers && from.NavLayer != to.NavLayer) return false;
    const Node* from_node = NodeAtStub(mgr, 0, &from.Position, from.NavLayer);
    const Node* to_node = NodeAtStub(mgr, 0, &to.Position, to.NavLayer);
    if (!from_node || !to_node) return false;
    if (!(M_Navigable(from_node) && M_Navigable(to_node))) return false;
    int a1 = M_Pref(from_node), a2 = M_Pref(to_node);
    int best_cost = a1 < a2 ? M_Pref(from_node) : M_Pref(to_node);   // the two-evaluation min macro
    if (M_IsEqual(from_node, to_node)) return true;
    V2 direction; direction.X = to.Position.X - from.Position.X; direction.Y = to.Position.Y - from.Position.Y;
    if (!(direction.X * direction.X + direction.Y * direction.Y > g_Delta * g_Delta)) return true;
    NormStub(&direction, 0);
    bool horizontal = false, vertical = false;
    if (fabs(direction.X) < g_Delta || fabs(fabs(direction.Y) - 1.0f) < g_Delta) vertical = true;
    if (fabs(direction.Y) < g_Delta || fabs(fabs(direction.X) - 1.0f) < g_Delta) horizontal = true;
    V2 normal; normal.X = direction.Y; normal.Y = -direction.X;
    float s = SmallestStub() * 0.45f; normal.X *= s; normal.Y *= s;
    V2 start1 = { from.Position.X + normal.X, from.Position.Y + normal.Y }, end1 = { to.Position.X + normal.X, to.Position.Y + normal.Y };
    V2 start2 = { from.Position.X - normal.X, from.Position.Y - normal.Y }, end2 = { to.Position.X - normal.X, to.Position.Y - normal.Y };
    long x_sign = Sign(direction.X), y_sign = Sign(direction.Y);
    for (unsigned long c1 = 1; c1 <= 2; ++c1) {
        const Node* current_node; V2 start_pos;
        if (c1 == 1) { current_node = NodeAtStub(mgr, 0, &start1, from.NavLayer); to_node = NodeAtStub(mgr, 0, &end1, from.NavLayer); start_pos = start1; }
        else { current_node = NodeAtStub(mgr, 0, &start2, from.NavLayer); to_node = NodeAtStub(mgr, 0, &end2, from.NavLayer); start_pos = start2; }
        if (!current_node || !to_node || !M_Navigable(current_node) || !M_Navigable(to_node) || M_Pref(current_node) > best_cost || M_Pref(to_node) > best_cost)
            return false;
        while (!M_IsEqual(current_node, to_node)) {
            if (!current_node || !M_Navigable(current_node) || M_Pref(current_node) > best_cost) return false;
            float vert_edge = SizeStub(current_node, 0) * x_sign * 0.5f + current_node->Position.X;
            float hori_edge = SizeStub(current_node, 0) * y_sign * 0.5f + current_node->Position.Y;
            V2 new_point = { 0, 0 };
            if (!vertical) { float lambda = (vert_edge - start_pos.X) / direction.X; new_point.X = vert_edge; new_point.Y = lambda * direction.Y + start_pos.Y; }
            if (!horizontal) {
                float lambda = (hori_edge - start_pos.Y) / direction.Y; V2 pp = { lambda * direction.X + start_pos.X, hori_edge };
                if (vertical || Dist2(start_pos, pp) < Dist2(start_pos, new_point)) new_point = pp;
            }
            float st = SmallestStub(); new_point.X += direction.X * st * 0.125f; new_point.Y += direction.Y * st * 0.125f;
            const Node* old_node = current_node;
            current_node = NodeAtStub(mgr, 0, &new_point, from.NavLayer);
            if (M_IsEqual(current_node, old_node)) return false;
        }
    }
    return true;
}
static void M_Goal(unsigned char* o)
{ GoalStub(At<void*>(o, O_AGENT), 0, &Front(o).Position, Size(o) > 1 ? &Second(o).Position : &Front(o).Position); }
static int Model(unsigned char* o, const V3& cur, const V3& dest, float proximity, float current_speed, V3& resulting_vector, bool stop_within, bool avoid_dyn,
                 bool avoid_static, bool reduce, bool fail_dyn, bool high_priority, bool ignore_pref, float lod)
{
    unsigned char* ag = At<unsigned char*>(o, O_AGENT);
    if (At<unsigned char>(o, O_INTERRUPTED)) { At<void*>(o, O_LASTNAV) = 0; UpdLastStub(o, 0, &cur); }
    if (ag[0x63]) { At<unsigned long>(ag, 0x54) = 0; ag[0x62] = 0; ag[0x61] = 1; ag[0x63] = 0; At<unsigned char>(o, O_INTERRUPTED) = 1; }
    At<V3>(o, O_LASTKNOWNPOS) = cur; At<float>(o, O_LASTKNOWNSPEED) = current_speed;
    float prox_and_rad_sq = RadiusStub(At<void*>(o, O_IF), 0, false);
    prox_and_rad_sq += proximity;
    resulting_vector.X = resulting_vector.Y = resulting_vector.Z = 0.0f;
    prox_and_rad_sq = prox_and_rad_sq * prox_and_rad_sq;
    float ddx = dest.X - cur.X, ddy = dest.Y - cur.Y, ddz = dest.Z - cur.Z;
    float initial = ddx * ddx + ddy * ddy + ddz * ddz;
    At<unsigned char>(o, O_HEADING) = 0;
    if (proximity == g_Delta) { V2 d = { dest.X, dest.Y }; At<unsigned char>(o, O_HEADING) = ActionStub(At<void*>(o, O_MGR), 0, &d); prox_and_rad_sq = g_Delta; }
    if (!stop_within && initial <= prox_and_rad_sq) { At<unsigned char>(o, O_ONROUTE) = 0; return 0; }
    const Node* end_node = M_GetNodeAt(o, dest);
    if (!end_node) { At<unsigned char>(o, O_ONROUTE) = 0; return 1; }
    if (M_Type(end_node) == 2) { BlockAssign(o + O_BLOCKEND, 0, end_node); end_node = (const Node*)(o + O_BLOCKEND); }
    bool reset_navigation = false;
    if (At<unsigned char>(o, O_INTERRUPTED) || !At<unsigned char>(o, O_ONROUTE)) reset_navigation = true;
    else if (Size(o) && !At<unsigned char>(o, O_HEADING)) {
        const Node* asked = At<const Node*>(o, O_ASKED);
        if (!M_IsEqual(end_node, asked)) {
            if (M_Navigable(end_node)) {
                if (M_Neighbour(end_node, asked)) {
                    WP fw; fw.Position.X = dest.X; fw.Position.Y = dest.Y; fw.NavLayer = end_node->NavLayer; fw.Removable = 1;
                    if (!M_IsRouteClear(o, Back(o), fw, true)) { WP c; c.Position = end_node->Position; c.NavLayer = end_node->NavLayer; c.Removable = 1; PushBack(o, c); }
                    PushBack(o, fw);
                    At<const Node*>(o, O_ASKED) = end_node; At<V2>(o, O_GOALPOS).X = dest.X; At<V2>(o, O_GOALPOS).Y = dest.Y; At<V3>(o, O_POSASKED) = dest;
                } else reset_navigation = true;
            } else {
                const Node* goal = At<const Node*>(o, O_GOALNODE);
                const Node* near_node = NearestStub(o, 0, goal, end_node, &dest, SizeStub(goal, 0), At<void*>(o, O_MGR));
                if (M_IsEqual(near_node, goal)) {
                    WP w; w.Position.X = dest.X; w.Position.Y = dest.Y; w.NavLayer = goal->NavLayer; w.Removable = 1; PushBack(o, w);
                    if (M_Type(end_node) == 2) { BlockAssign(o + O_BLOCKASKED, 0, end_node); At<const Node*>(o, O_ASKED) = (const Node*)(o + O_BLOCKASKED); }
                    else At<const Node*>(o, O_ASKED) = end_node;
                    At<V2>(o, O_GOALPOS).X = dest.X; At<V2>(o, O_GOALPOS).Y = dest.Y; At<V3>(o, O_POSASKED) = dest;
                } else reset_navigation = true;
            }
        } else if (M_Navigable(end_node)) {
            V2 new_dest_pos = { dest.X, dest.Y };
            WP& last = Back(o);
            if (!(fabs(last.Position.X - new_dest_pos.X) <= g_Delta && fabs(last.Position.Y - new_dest_pos.Y) <= g_Delta)) {
                V2 old_dest_pos = last.Position;
                last.Position = new_dest_pos; At<V2>(o, O_GOALPOS) = new_dest_pos;
                if (Size(o) == 1) UpdGoalStub(o, 0, &cur, old_dest_pos, new_dest_pos);
            }
        }
    }
    reset_navigation = !(!reset_navigation && M_IsEqual(end_node, At<const Node*>(o, O_ASKED)) &&
                         Dist2(*(V2*)&dest, At<V2>(o, O_GOALPOS)) <= prox_and_rad_sq);
    const Node* start_node = M_GetNodeAt(o, cur);
    if (!start_node) { At<unsigned char>(o, O_ONROUTE) = 0; return 1; }
    if (M_Type(start_node) == 2) { BlockAssign(o + O_BLOCKSTART, 0, start_node); start_node = (const Node*)(o + O_BLOCKSTART); }
    if (stop_within && M_Navigable(end_node) && proximity * proximity >= initial) {
        WP a; a.Position.X = cur.X; a.Position.Y = cur.Y; a.NavLayer = start_node->NavLayer; a.Removable = 1;
        WP b; b.Position.X = dest.X; b.Position.Y = dest.Y; b.NavLayer = end_node->NavLayer; b.Removable = 1;
        if (M_IsRouteClear(o, a, b, false)) { At<unsigned char>(o, O_ONROUTE) = 0; return 0; }
    }
    if (!reset_navigation && Size(o)) reset_navigation = NodeAtStub(At<void*>(o, O_MGR), 0, &Front(o).Position, Front(o).NavLayer) == 0;
    if (reset_navigation) {
        At<const Node*>(o, O_ASKED) = 0; At<unsigned char>(o, O_INTERRUPTED) = 0;
        const Node* init_start = start_node; const Node* init_end = end_node; V2 init_dest = { dest.X, dest.Y };
        if (avoid_static) {
            if (!M_Navigable(end_node)) {
                if (!At<unsigned char>(o, O_HEADING) && proximity <= g_Delta) { At<unsigned char>(o, O_ONROUTE) = 0; return 1; }
                const Node* near_node = NearestStub(o, 0, start_node, end_node, &dest, proximity, At<void*>(o, O_MGR));
                if (!near_node) { At<unsigned char>(o, O_ONROUTE) = 0; return 1; }
                if (At<unsigned char>(o, O_HEADING)) init_end = near_node;
                else {
                    float r = RadiusStub(At<void*>(o, O_IF), 0, false);
                    if (!AnyPartStub(o, 0, near_node, r + proximity, &dest, &init_dest)) { At<unsigned char>(o, O_ONROUTE) = 0; return 1; }
                    init_end = near_node;
                }
            }
            if (!M_Navigable(start_node)) {
                if (!At<const Node*>(o, O_LASTNAV)) { At<unsigned char>(o, O_ONROUTE) = 0; return 1; }
                init_start = At<const Node*>(o, O_LASTNAV);
            }
            const Region* r1 = M_Region(init_start); const Region* r2 = M_Region(init_end);
            if (!ConnStub(r1, 0, r2)) { At<unsigned char>(o, O_ONROUTE) = 0; return 1; }
        }
        V2 c2 = { cur.X, cur.Y };
        InitStub(o, 0, init_start, init_end, &c2, &init_dest, end_node, &dest, avoid_static, high_priority, lod);
    }
    if (At<unsigned char>(o, O_PARTWAY)) {
        At<unsigned char>(o, O_PARTWAY) = 0;
        CalcRouteStub(o, 0, ignore_pref);
        if (At<unsigned char>(o, O_PARTWAY)) return 2;
        if (!At<unsigned char>(o, O_GOTROUTE)) { At<unsigned char>(o, O_ONROUTE) = 0; return 1; }
        WP f; f.Position.X = cur.X; f.Position.Y = cur.Y; f.NavLayer = Front(o).NavLayer; f.Removable = 1; PushFront(o, f);
        WP b; b.Position = At<V2>(o, O_GOALPOS); b.NavLayer = end_node->NavLayer; b.Removable = 1; PushBack(o, b);
        At<LNode*>(o, O_CULLFRONT) = Sent(o)->next; At<LNode*>(o, O_CULLBACK) = Sent(o)->prev;
    } else if (!At<unsigned char>(o, O_GOTROUTE)) { At<unsigned char>(o, O_ONROUTE) = 0; return 1; }
    if (!At<unsigned char>(o, O_CULLED)) {
        CullStub(o, 0, 300);
        if (!At<unsigned char>(o, O_CULLED)) return 2;
        M_Goal(o);
    }
    while (Size(o) > 1) {
        V2 line[2] = { Front(o).Position, Second(o).Position };
        if (At<float>(ag, A_PROX) == 0.0f) At<float>(ag, A_PROX) = current_speed;
        V2 here = { cur.X, cur.Y };
        float d = SqDistStub(line, 0, &here);
        float pr = At<float>(ag, A_PROX);
        if (d > pr * pr) break;
        At<V2>(o, O_LASTWP) = Front(o).Position;
        PopFront(o);
        WidthStub(o, 0, &cur);
        M_Goal(o);
    }
    V2 c2 = { cur.X, cur.Y };
    if (!Size(o) || (!stop_within && Size(o) == 1 && Dist2(c2, Front(o).Position) <= g_Delta)) { At<unsigned char>(o, O_ONROUTE) = 0; return 0; }
    bool better = At<float>(ag, A_BESTL) > At<float>(ag, A_NEXTL) || At<float>(ag, A_BESTR) > At<float>(ag, A_NEXTR);
    if (Size(o) > 1 && Front(o).Removable && !better) {
        WP a; a.Position = c2; a.NavLayer = Front(o).NavLayer; a.Removable = 1;
        if (M_IsRouteClear(o, a, Second(o), true)) {
            PopFront(o); At<V2>(o, O_LASTWP) = c2; WidthStub(o, 0, &cur); M_Goal(o);
        }
    }
    MoveStub(o, 0, &cur, current_speed, proximity, &resulting_vector, stop_within, avoid_dyn, avoid_static, reduce);
    if (ag[0x63] && fail_dyn) return 1;
    return 2;
}

// ---------------- harness ----------------
static unsigned char NavR[NAV_SIZE], NavM[NAV_SIZE], AgentM[0x70], AgentInit[0x70];
static unsigned long R32() { return ((unsigned long)rand() << 16) ^ (unsigned long)rand(); }
static float Rf(float lo, float hi) { return lo + (hi - lo) * (float)(rand() % 10000) / 10000.0f; }
static int Fails, Paths[16];
static int ListIndex(unsigned char* o, LNode* p) { int k = 0; for (LNode* n = Sent(o)->next; ; n = n->next, ++k) { if (n == p) return k; if (n == Sent(o)) return -1; } }
static unsigned long PtrTok(unsigned char* o, const void* p)
{
    if (!p) return 0;
    if ((const unsigned char*)p >= o && (const unsigned char*)p < o + NAV_SIZE) return 0x10000000ul + (unsigned long)((const unsigned char*)p - o);
    int id = NodeId(p); if (id) return 0x20000000ul + id;
    return (unsigned long)p;
}
static bool FEq(unsigned long a, unsigned long b)
{
    float x, y; memcpy(&x, &a, 4); memcpy(&y, &b, 4);
    if (a == b) return true; return fabs(x - y) <= 1e-5 * (fabs(x) > 1 ? fabs(x) : 1);
}
static void RandomWorld()
{
    for (int l = 0; l < GL; ++l) for (int y = 0; y < GH; ++y) for (int x = 0; x < GW; ++x) {
        Node& n = World[l][y][x]; n.vptr = NodeVtbl; n.Position.X = x * CELL + CELL * 0.5f; n.Position.Y = y * CELL + CELL * 0.5f;
        n.Level = 3; n.NavLayer = (unsigned char)l; n.Leaf = 1;
        n.Blocked = (rand() % 6) == 0; Type[l][y][x] = n.Blocked ? ((rand() % 4) ? 2 : 1) : ((rand() % 10) ? 1 : 2);
        Pref[l][y][x] = (unsigned char)(rand() % 3); Group[l][y][x] = (unsigned char)(rand() % 3); Hole[l][y][x] = (rand() % 25) == 0;
    }
    for (int y = 0; y < GH; ++y) for (int x = 0; x < GW; ++x) LayerAt[y][x] = (unsigned char)((rand() % 5) == 0);
    for (int i = 0; i < 4; ++i) { Regions[i].group = i; for (int j = 0; j < 4; ++j) Connected[i][j] = i == j || (rand() % 3) == 0; }
}
static V2 RandPos() { V2 p = { Rf(0.3f, 59.7f), Rf(0.3f, 59.7f) }; return p; }
static const Node* RandNode() { return NodeByIdx(rand() % (GW * GH * GL)); }
static bool RunCase(const char* name, unsigned long seed, int bias)
{
    srand(seed); CurCase = name;
    RandomWorld();
    // navigator
    memset(NavR, 0, sizeof(NavR));
    for (int i = 0; i < NAV_SIZE; ++i) NavR[i] = (unsigned char)rand();
    At<void**>(NavR, 0) = NavVtbl; At<void*>(NavR, O_IF) = IfObj; At<void*>(NavR, O_MGR) = Manager; At<void*>(NavR, O_AGENT) = Agent;
    At<unsigned char>(NavR, O_ONROUTE) = (rand() % 4) != 0; At<unsigned char>(NavR, O_INTERRUPTED) = (rand() % 5) == 0;
    At<unsigned char>(NavR, O_HEADING) = rand() & 1; At<unsigned char>(NavR, O_GOTROUTE) = (rand() % 5) != 0;
    At<unsigned char>(NavR, O_CULLED) = (rand() % 3) != 0; At<unsigned char>(NavR, O_PARTWAY) = (rand() % 4) == 0;
    const Node* goal = RandNode(); At<const Node*>(NavR, O_GOALNODE) = goal;
    At<V2>(NavR, O_GOALPOS) = RandPos();
    LNode* s = (LNode*)malloc(0x14); memset(s, 0, 0x14); s->next = s->prev = s; At<LNode*>(NavR, O_ROUTE) = s;   // the sentinel's data is read when the route is empty
    int nwp = rand() % 5;
    V3 cur = { Rf(0.3f, 59.7f), Rf(0.3f, 59.7f), Rf(-5, 5) }, dest = { Rf(0.3f, 59.7f), Rf(0.3f, 59.7f), Rf(-5, 5) };
    if (bias == 1) { dest.X = cur.X + Rf(-12, 12); dest.Y = cur.Y + Rf(-12, 12); if (dest.X < 0.3f) dest.X = 0.3f; if (dest.Y < 0.3f) dest.Y = 0.3f; if (dest.X > 59.7f) dest.X = 59.7f; if (dest.Y > 59.7f) dest.Y = 59.7f; }
    for (int i = 0; i < nwp; ++i) { WP w; w.Position = RandPos(); w.NavLayer = (unsigned char)((rand() % 6) == 0); w.Removable = (unsigned char)(rand() & 1); PushBack(NavR, w); }
    if (nwp && (rand() % 3) == 0) { WP& f = Front(NavR); f.Position.X = cur.X + Rf(-8, 8); f.Position.Y = cur.Y + Rf(-8, 8); }
    if (nwp && (rand() % 3) == 0) { Back(NavR).Position.X = dest.X; Back(NavR).Position.Y = dest.Y; }
    At<LNode*>(NavR, O_CULLFRONT) = s->next; At<LNode*>(NavR, O_CULLBACK) = s->prev;
    // asked node: the dest's node, a neighbour, a random node, NULL, or the blocked copy
    int ak = rand() % 6;
    {
        int x = (int)floor(dest.X / CELL), y = (int)floor(dest.Y / CELL), l = LayerAt[y][x];
        const Node* asked = &World[l][y][x];
        if (ak == 1) { int nx = x + (rand() % 3) - 1, ny = y + (rand() % 3) - 1; if (nx >= 0 && nx < GW && ny >= 0 && ny < GH) asked = &World[l][ny][nx]; }
        else if (ak == 2) asked = RandNode(); else if (ak == 3) asked = 0;
        else if (ak == 4) { memcpy(NavR + O_BLOCKASKED, asked, 0x10); asked = (const Node*)(NavR + O_BLOCKASKED); }
        At<const Node*>(NavR, O_ASKED) = asked;
        if (rand() & 1) { At<V2>(NavR, O_GOALPOS).X = dest.X + Rf(-1, 1); At<V2>(NavR, O_GOALPOS).Y = dest.Y + Rf(-1, 1); }
    }
    At<const Node*>(NavR, O_LASTNAV) = (rand() % 3) ? RandNode() : 0;
    // agent
    for (int i = 0; i < 0x70; ++i) Agent[i] = (unsigned char)rand();
    Agent[0x63] = (rand() % 5) == 0;
    At<float>(Agent, A_PROX) = (rand() % 4) ? Rf(0.5f, 9.0f) : 0.0f;
    At<float>(Agent, A_BESTL) = Rf(0, 5); At<float>(Agent, A_BESTR) = Rf(0, 5); At<float>(Agent, A_NEXTL) = Rf(0, 5); At<float>(Agent, A_NEXTR) = Rf(0, 5);
    memcpy(AgentInit, Agent, sizeof(Agent));
    // script
    S.radius = Rf(0.2f, 2.0f);
    for (int i = 0; i < 4; ++i) S.nearestIdx[i] = (rand() % 4) == 0 ? -1 : ((rand() & 1) ? (int)(NodeId(goal) - 1) : rand() % (GW * GH * GL));
    S.action = rand() & 1; S.anyPart = (rand() % 4) != 0; S.anyPartOut = RandPos(); S.initPartway = (rand() % 3) != 0; S.initGot = rand() & 1;
    S.crPartway = (rand() % 4) == 0; S.crTouch = (rand() % 4) != 0; S.crGot = (rand() % 5) != 0; S.crCount = 1 + rand() % 4;
    for (int i = 0; i < 4; ++i) { S.crRoute[i].Position = RandPos(); S.crRoute[i].NavLayer = (unsigned char)((rand() % 6) == 0); S.crRoute[i].Removable = (unsigned char)(rand() & 1); }
    S.cullResult = (rand() % 4) != 0; S.moveOut.X = Rf(-1, 1); S.moveOut.Y = Rf(-1, 1); S.moveOut.Z = Rf(-1, 1);
    // parameters
    int pk = rand() % 6;
    float proximity = pk == 0 ? g_Delta : pk == 1 ? 0.0f : Rf(0.0f, 6.0f);
    float speed = Rf(0.5f, 6.0f), lod = Rf(0, 100);
    bool fl[8]; for (int i = 0; i < 8; ++i) fl[i] = rand() & 1;
    if (rand() & 1) fl[2] = true;                                   // avoid_static_obstacles more often
    if (bias == 2) {                                                // close / axis-aligned destination
        float dx = Rf(-3, 3), dy = Rf(-3, 3); int ax = rand() % 3;
        if (ax == 0) dx = 0.0f; else if (ax == 1) dy = 0.0f;
        dest.X = cur.X + dx; dest.Y = cur.Y + dy; dest.Z = cur.Z;
        if (dest.X < 0.3f || dest.X > 59.7f) dest.X = cur.X; if (dest.Y < 0.3f || dest.Y > 59.7f) dest.Y = cur.Y;
        if (rand() & 1) { fl[0] = true; proximity = Rf(3.0f, 8.0f); }
    }
    if (bias == 3) {                                                // exact ties: (radius + proximity)^2 == 25
        S.radius = 2.0f; proximity = 3.0f;
        cur.X = (float)(5 + rand() % 40); cur.Y = (float)(5 + rand() % 40); cur.Z = 0.0f;
        dest.X = cur.X + 3.0f; dest.Y = cur.Y + 4.0f; dest.Z = 0.0f;
        if (rand() & 1) { At<V2>(NavR, O_GOALPOS).X = dest.X + 3.0f; At<V2>(NavR, O_GOALPOS).Y = dest.Y + 4.0f; }
    }
    if (bias == 5) {                                                // a destination 4e-5 across a cell edge: a near-zero line between two nodes
        cur.X = 9.99998f; cur.Y = 15.3f; dest.X = 10.00002f; dest.Y = 15.3f; dest.Z = cur.Z; fl[0] = true; proximity = Rf(1.0f, 4.0f);
        LayerAt[1][0] = LayerAt[1][1] = 0;
        for (int x = 0; x < 2; ++x) { World[0][1][x].Blocked = 0; Hole[0][1][x] = false; Type[0][1][x] = 1; }
    }
    if (bias == 4) {                                                // one way point at the current position, route kept
        while (Size(NavR)) PopFront(NavR);
        WP w; w.Position.X = cur.X; w.Position.Y = cur.Y; w.NavLayer = 0; w.Removable = 1; PushBack(NavR, w);
        if (rand() & 1) { WP w2; w2.Position = RandPos(); w2.NavLayer = 0; w2.Removable = 1; PushFront(NavR, w2); }
        At<LNode*>(NavR, O_CULLFRONT) = Sent(NavR)->next; At<LNode*>(NavR, O_CULLBACK) = Sent(NavR)->prev;
        At<unsigned char>(NavR, O_ONROUTE) = 1; At<unsigned char>(NavR, O_INTERRUPTED) = 0; Agent[0x63] = 0; memcpy(AgentInit, Agent, sizeof(Agent));
        At<unsigned char>(NavR, O_CULLED) = 1; At<unsigned char>(NavR, O_PARTWAY) = 0; At<unsigned char>(NavR, O_GOTROUTE) = 1;
        int x = (int)floor(dest.X / CELL), y = (int)floor(dest.Y / CELL); At<const Node*>(NavR, O_ASKED) = &World[LayerAt[y][x]][y][x];
        At<V2>(NavR, O_GOALPOS).X = dest.X; At<V2>(NavR, O_GOALPOS).Y = dest.Y;
        if (rand() & 1) fl[0] = false;
    }
    // model copy: navigator with a deep-copied route, agent
    memcpy(NavM, NavR, sizeof(NavR));
    { LNode* ms = (LNode*)malloc(0x14); memset(ms, 0, 0x14); ms->next = ms->prev = ms; At<LNode*>(NavM, O_ROUTE) = ms;
      for (LNode* n = s->next; n != s; n = n->next) PushBack(NavM, n->data);
      At<LNode*>(NavM, O_CULLFRONT) = ms->next; At<LNode*>(NavM, O_CULLBACK) = ms->prev;
      if ((const unsigned char*)At<const Node*>(NavR, O_ASKED) == NavR + O_BLOCKASKED) At<const Node*>(NavM, O_ASKED) = (const Node*)(NavM + O_BLOCKASKED); }
    V3 outR = { 9, 9, 9 }, outM = { 9, 9, 9 };
    // row
    Cur = LogR; CurN = &NR; NR = 0; SelfCur = NavR; NearestCalls = 0;
    int rr = CallRow(NavR, 0, &cur, &dest, proximity, speed, &outR, fl[0], fl[1], fl[2], fl[3], fl[4], fl[5], fl[6], lod);
    memcpy(AgentM, Agent, sizeof(Agent)); memcpy(Agent, AgentInit, sizeof(Agent));
    unsigned char AgentRowAfter[0x70]; memcpy(AgentRowAfter, AgentM, sizeof(AgentM));
    // model (runs against the same agent buffer, reset to its initial bytes)
    At<void*>(NavM, O_AGENT) = Agent;
    Cur = LogM; CurN = &NM; NM = 0; SelfCur = NavM; NearestCalls = 0;
    int rm = Model(NavM, cur, dest, proximity, speed, outM, fl[0], fl[1], fl[2], fl[3], fl[4], fl[5], fl[6], lod);
    bool ok = true;
    if (rr != rm) { printf("FAIL %s: returned %d, model %d\n", name, rr, rm); ok = false; }
    if (NR != NM) { printf("FAIL %s: %d calls, model %d\n", name, NR, NM); ok = false; }
    for (int i = 0; ok && i < NR && i < MAXEV; ++i) {
        if (LogR[i].kind != LogM[i].kind) { printf("FAIL %s: call %d kind %d, model %d\n", name, i, LogR[i].kind, LogM[i].kind); ok = false; break; }
        for (int k = 0; k < 10; ++k) if (LogR[i].isf[k] ? !FEq(LogR[i].p[k], LogM[i].p[k]) : LogR[i].p[k] != LogM[i].p[k]) {
            printf("FAIL %s: call %d (kind %d) arg %d %08lX, model %08lX\n", name, i, LogR[i].kind, k, LogR[i].p[k], LogM[i].p[k]); ok = false; break; }
    }
    // navigator bytes: pointers translated, the route by content
    for (int off = 0; ok && off < NAV_SIZE; ) {
        if (off == O_ASKED || off == O_LASTNAV || off == O_GOALNODE) {
            if (PtrTok(NavR, At<void*>(NavR, off)) != PtrTok(NavM, At<void*>(NavM, off))) { printf("FAIL %s: pointer +0x%X differs\n", name, off); ok = false; }
            off += 4; continue;
        }
        if (off == O_ROUTE) { off += 4; continue; }
        if (off == O_CULLFRONT || off == O_CULLBACK) {
            if (ListIndex(NavR, At<LNode*>(NavR, off)) != ListIndex(NavM, At<LNode*>(NavM, off))) { printf("FAIL %s: iterator +0x%X differs\n", name, off); ok = false; }
            off += 4; continue;
        }
        if (off == O_AGENT) { off += 4; continue; }
        if (NavR[off] != NavM[off]) {
            float a, b; memcpy(&a, NavR + (off & ~3), 4); memcpy(&b, NavM + (off & ~3), 4);
            if (!((off >= O_GOALPOS && off < O_GOALPOS + 8) || (off >= O_POSASKED && off < O_POSASKED + 12) || (off >= O_LASTWP && off < O_LASTWP + 8)) || !FEq(*(unsigned long*)&a, *(unsigned long*)&b)) {
                printf("FAIL %s: navigator byte +0x%X row %02X model %02X\n", name, off, NavR[off], NavM[off]); ok = false; }
        }
        ++off;
    }
    if (ok) {
        int n1 = Size(NavR), n2 = Size(NavM);
        if (n1 != n2) { printf("FAIL %s: route %d way points, model %d\n", name, n1, n2); ok = false; }
        else for (LNode *a = Sent(NavR)->next, *b = Sent(NavM)->next; a != Sent(NavR); a = a->next, b = b->next)
            if (!FEq(FB(a->data.Position.X), FB(b->data.Position.X)) || !FEq(FB(a->data.Position.Y), FB(b->data.Position.Y)) ||
                a->data.NavLayer != b->data.NavLayer || a->data.Removable != b->data.Removable) { printf("FAIL %s: route way point differs\n", name); ok = false; break; }
    }
    if (ok && memcmp(AgentRowAfter, Agent, sizeof(Agent))) { printf("FAIL %s: steering agent differs\n", name); ok = false; }
    if (ok && memcmp(&outR, &outM, sizeof(outR))) { printf("FAIL %s: resulting_vector differs\n", name); ok = false; }
    Paths[rr & 3]++;
    for (int i = 0; i < NR && i < MAXEV; ++i) if (LogR[i].kind == EV_INIT) { Paths[4]++; break; }
    for (int i = 0; i < NR && i < MAXEV; ++i) if (LogR[i].kind == EV_CALCROUTE) { Paths[5]++; break; }
    { int k = 0; for (int i = 0; i < NR && i < MAXEV; ++i) if (LogR[i].kind == EV_SMALL) ++k; if (k) Paths[6]++; if (k > 2) Paths[7]++; }
    for (int i = 0; i < NR && i < MAXEV; ++i) if (LogR[i].kind == EV_UPDGOAL) { Paths[8]++; break; }
    for (int i = 0; i < NR && i < MAXEV; ++i) if (LogR[i].kind == EV_ASSIGN) { Paths[9]++; break; }
    for (int i = 0; i < NR && i < MAXEV; ++i) if (LogR[i].kind == EV_WIDTH) { Paths[10]++; break; }
    for (int i = 0; i < NR && i < MAXEV; ++i) if (LogR[i].kind == EV_NEAREST) { Paths[11]++; break; }
    for (int i = 0; i < NR && i < MAXEV; ++i) if (LogR[i].kind == EV_CULL) { Paths[12]++; break; }
    for (int i = 0; i < NR && i < MAXEV; ++i) if (LogR[i].kind == EV_MOVE) { Paths[13]++; break; }
    FreeList(NavR); FreeList(NavM);
    if (!ok) ++Fails;
    return ok;
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    for (int i = 0; i < 32; ++i) { NodeVtbl[i] = (void*)VirtualTrap; IfVtbl[i] = (void*)VirtualTrap; NavVtbl[i] = (void*)VirtualTrap; }
    NodeVtbl[4] = (void*)NbStub; NodeVtbl[6] = (void*)TypeStub; NodeVtbl[7] = (void*)RegionStub; NodeVtbl[8] = (void*)PrefStub;
    IfVtbl[6] = (void*)RadiusStub; IfVtbl[16] = (void*)LevelStub; NavVtbl[13] = (void*)UpdLastStub;
    int k = 0;
    for (int t = 0; t < 12; ++t) { char nm[32]; sprintf(nm, "targeted %d", t); RunCase(nm, 77 + t, t % 6); ++k; }
    for (int r = 0; r < 20000; ++r) {
        char name[32]; sprintf(name, "random %d", r);
        RunCase(name, 5000 + r, r % 6 == 0 ? 1 : r % 6 == 1 ? 2 : r % 6 == 2 ? 3 : r % 6 == 3 ? 4 : 0);
        if (Fails > 5) break;
    }
    printf("paths: reached %d cannot %d moving %d | init %d calcroute %d routeclear %d (multi-step %d) updgoal %d blockassign %d width %d nearest %d cull %d move %d\n",
           Paths[0], Paths[1], Paths[2], Paths[4], Paths[5], Paths[6], Paths[7], Paths[8], Paths[9], Paths[10], Paths[11], Paths[12], Paths[13]);
    if (Fails) { printf("00a1e5c0_TEST FAIL (%d)\n", Fails); return 1; }
    printf("00a1e5c0_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}