// Behaviour test for 0x00986210 CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable (links the REAL row object: the row body,
// its expanded GetTC / LowerBound, the scan-box arithmetic, the GFFloatToLongCeil expansion, the four filter constructions and the
// same-TU template GetPFirstThingInRoughArea<CFilter_Bind4<..>, const CThingAICreature> all run as compiled).
// The row's externals are defined here as recording doubles, with the shared headers (same opt-ins as the row) giving the exact
// signatures. The fixture objects are raw native-layout storage; every offset below is read by an instruction of the row object:
//   state group: +0x04 PBrain (0x00986219), brain +0x18 Creature (0x0098621C), +0x6B ThingReactingTo (0x009862E6, GetPItem receiver);
//   thing: +0x24 UsingComponents word 1 (HasTC(0x34) = bit 0x100000, 0x00986233), +0x44 Components vector {start,finish} of
//     {ETCInterfaceType, CTCBase*} pairs (0x00986245, LowerBound), +0x6C ThingTypeFlags (template 0x00986350; CThingAICreature
//     StatGetTypeFlags = 0x1F);
//   search tools: +0x0C AreaMaps {start,finish} (template 0x0098621D / 0x00986256);
//   thing map: +0x04 MapwhoLevelInfo start (stride 0x14, Extents.X at +4), +0x14 Cells start (stride 0x0C, each a vector start);
//   map node: +0 PMapwho, +4 PNext; mapwho: +0x04 ParentThing, +0x4C SearchID; NThingMap::LayerInfo stride 0x10, ScaleShift at +4.
// The doubles' semantics (IsNot: a different object; In2DRange: X/Y distance squared <= range squared) are the test's own model; the
// test checks the arguments the row passes (filter members, scan box, receivers, call order) and the result the row derives.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_GETSIGHTRADIUS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_X_THEN_YZ
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DExtentsI.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsCreatureType.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// The row's class, declared with the row's private member only (?CallingChildrenIsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ);
// its object is raw native-layout storage below.
class CAIStateGroup_OpinionReactionRoot {
    friend bool RunRow(CAIStateGroup_OpinionReactionRoot* p);
    bool CallingChildrenIsRunnable();
};

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; printf("FAIL %s (case %d)\n", what, k); } }

// ---- fixture storage ----------------------------------------------------------------------------------------------------------
enum { MAXTHINGS = 16, GRID = 16, TCI_PERCEIVE = 0x34 };
struct Pair { long id; void* tc; };
struct ThingFx { union { unsigned char b[0x100]; double align; } u; Pair comp[1]; C3DVector pos; int type; unsigned char perceive[0x40]; float sight; };
struct NodeFx { void* pmapwho; NodeFx* next; void* prev; void* cell; };
struct MapwhoFx { union { unsigned char b[0x54]; double align; } u; };
static ThingFx things[MAXTHINGS];
static MapwhoFx mapwhos[MAXTHINGS];
static NodeFx nodes[MAXTHINGS];
static int nthings;
static void* cells[GRID * GRID];            // CThingMapCell = { CMapwhoNode* PFirstNode }
static void* rows[1][3];                    // Cells: vector of one per-layer vector {start, finish, eos}
static long levels[1][5];                   // MapwhoLevelInfo: {Size, Extents.X, Extents.Y, ScaleShift, NoThings}
static union { unsigned char b[0x3c]; double align; } mapStore;
static NThingMap::CThingMap* mapList[1];
static union { unsigned char b[0x20]; double align; } toolsStore;
static union { unsigned char b[0xc0]; double align; } brainStore;
static union { unsigned char b[0x98]; double align; } rootStore;
static ThingFx* reacting;
static CAIStateGroup_OpinionReactionRoot* root;

template<class T> static void put(void* base, int off, T v) { memcpy((unsigned char*)base + off, &v, sizeof v); }
template<class T> static T get(const void* base, int off) { T v; memcpy(&v, (const unsigned char*)base + off, sizeof v); return v; }
static ThingFx* fxOf(const void* p) { for (int i = 0; i < MAXTHINGS; ++i) if ((const void*)&things[i] == p) return &things[i]; return 0; }
static ThingFx* fxOfPerceive(const void* p) { for (int i = 0; i < MAXTHINGS; ++i) if ((const void*)things[i].perceive == p) return &things[i]; return 0; }

// ---- recording -----------------------------------------------------------------------------------------------------------------
enum { MAXLOG = 64 };
struct FilterCall { char kind; const void* thing; const void* filterThing; C3DVector pos; float range; };
static FilterCall calls[MAXLOG]; static int ncalls;
static int nCreatureType, nSight, nPos, nPItem, nPeek, nMaps, nClip;
static const void* sightThis; static const void* peekThis; static const void* pitemThis;
static C2DBoxI mapsArea, clipArea; static const void* clipMap;

ECreatureType CThingCreatureBase::GetCreatureType() const
{
    ++nCreatureType;
    ThingFx* fx = fxOf(this);
    if (!fx) { ++failures; printf("FAIL GetCreatureType receiver\n"); return NOT_HUMAN; }
    return (ECreatureType)fx->type;
}
float CTCPerceiveThing::GetSightRadius() const
{
    ++nSight; sightThis = this;
    ThingFx* fx = fxOfPerceive(this);
    if (!fx) { ++failures; printf("FAIL GetSightRadius receiver\n"); return 0.0f; }
    return fx->sight;
}
const C3DVector& CThing::GetPos() const
{
    ++nPos;
    ThingFx* fx = fxOf(this);
    if (!fx) { ++failures; printf("FAIL GetPos receiver\n"); return things[0].pos; }
    return fx->pos;
}
CBaseObject* CBaseIntelligentPointer::GetPItem() const
{
    ++nPItem; pitemThis = this;
    return (CBaseObject*)(void*)reacting;
}
const CThingSearchTools& CAIStateGroupBase::PeekThingSearchTools() const
{
    ++nPeek; peekThis = this;
    return *(const CThingSearchTools*)(const void*)toolsStore.b;
}
void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI& area, std::vector<NThingMap::CThingMap*>& maps) const
{
    ++nMaps; mapsArea = area;
    check((const void*)&maps == (const void*)(toolsStore.b + 0x0c), "GetAllThingMapsInArea fills this->AreaMaps", -1);
    put<void*>(&maps, 0, mapList); put<void*>(&maps, 4, mapList + 1); put<void*>(&maps, 8, mapList + 1);
}
C2DBoxI CThingSearchTools::ClipAndConvertWorldMapAreaToLocalArea(const NThingMap::CThingMap& map, const C2DBoxI& area) const
{
    ++nClip; clipArea = area; clipMap = &map;
    C2DBoxI r;
    r.TLX = area.TLX < 0 ? 0 : (area.TLX > GRID ? GRID : area.TLX);
    r.TLY = area.TLY < 0 ? 0 : (area.TLY > GRID ? GRID : area.TLY);
    r.BRX = area.BRX < r.TLX ? r.TLX : (area.BRX > GRID ? GRID : area.BRX);
    r.BRY = area.BRY < r.TLY ? r.TLY : (area.BRY > GRID ? GRID : area.BRY);
    return r;
}
unsigned long CThingSearchTools::NoLookups = 0;
unsigned long CThingSearchTools::NoThingsScanned = 0;
unsigned long CThingSearchTools::SearchID = 0;
long NThingMap::MaxLayers = 1;
static NThingMap::CLayerInfo MakeLayer0() { NThingMap::CLayerInfo l; memset(&l, 0, sizeof l); l.Size = 1; l.ScaleShift = 0; return l; }
const NThingMap::CLayerInfo NThingMap::LayerInfo[1] = { MakeLayer0() };

bool CThingFilter_IsNot::operator()(const CThing& t) const
{
    if (ncalls < MAXLOG) { FilterCall& c = calls[ncalls++]; c.kind = 'N'; c.thing = &t; c.filterThing = &Thing; c.range = 0; }
    return &t != &Thing;
}
bool CThingFilter_In2DRange::operator()(const CThing& t) const
{
    if (ncalls < MAXLOG) { FilterCall& c = calls[ncalls++]; c.kind = 'R'; c.thing = &t; c.filterThing = 0; c.pos = Pos; c.range = Range; }
    ThingFx* fx = fxOf(&t);
    float dx = fx->pos.X - Pos.X, dy = fx->pos.Y - Pos.Y;
    return dx * dx + dy * dy <= Range * Range;
}

// ---- fixture building ----------------------------------------------------------------------------------------------------------
static void ResetWorld()
{
    memset(things, 0, sizeof things); memset(mapwhos, 0, sizeof mapwhos); memset(nodes, 0, sizeof nodes); memset(cells, 0, sizeof cells);
    nthings = 0;
    memset(mapStore.b, 0, sizeof mapStore.b); memset(toolsStore.b, 0, sizeof toolsStore.b);
    levels[0][0] = GRID * GRID; levels[0][1] = GRID; levels[0][2] = GRID; levels[0][3] = 0; levels[0][4] = 0;
    rows[0][0] = cells; rows[0][1] = cells + GRID * GRID; rows[0][2] = rows[0][1];
    put<void*>(mapStore.b, 0x04, levels); put<void*>(mapStore.b, 0x08, levels + 1); put<void*>(mapStore.b, 0x0c, levels + 1);
    put<void*>(mapStore.b, 0x14, rows); put<void*>(mapStore.b, 0x18, rows + 1); put<void*>(mapStore.b, 0x1c, rows + 1);
    mapList[0] = (NThingMap::CThingMap*)(void*)mapStore.b;
}
// A creature-typed thing (flags 0x1F) with a perception TC, standing at (x, y, z); `inMap` links it into its cell's node list (head).
static ThingFx* AddThing(float x, float y, float z, int type, float sight, bool inMap, unsigned long flags = 0x1f)
{
    ThingFx* t = &things[nthings];
    put<unsigned long>(t->u.b, 0x24, 1ul << (TCI_PERCEIVE - 32));
    t->comp[0].id = TCI_PERCEIVE; t->comp[0].tc = t->perceive;
    put<void*>(t->u.b, 0x44, t->comp); put<void*>(t->u.b, 0x48, t->comp + 1); put<void*>(t->u.b, 0x4c, t->comp + 1);
    put<unsigned long>(t->u.b, 0x6c, flags);
    t->pos.X = x; t->pos.Y = y; t->pos.Z = z; t->type = type; t->sight = sight;
    if (inMap) {
        int cx = (int)x, cy = (int)y;
        MapwhoFx* m = &mapwhos[nthings]; NodeFx* n = &nodes[nthings];
        put<void*>(m->u.b, 0x04, t); put<unsigned long>(m->u.b, 0x4c, 0);
        n->pmapwho = m; n->next = (NodeFx*)cells[cy * GRID + cx]; cells[cy * GRID + cx] = n;
    }
    ++nthings;
    return t;
}
static void Bind(ThingFx* creature, ThingFx* target)
{
    memset(brainStore.b, 0, sizeof brainStore.b); memset(rootStore.b, 0, sizeof rootStore.b);
    put<void*>(brainStore.b, 0x18, creature);
    put<void*>(rootStore.b, 0x04, brainStore.b);
    reacting = target;
    root = (CAIStateGroup_OpinionReactionRoot*)(void*)rootStore.b;
    ncalls = nCreatureType = nSight = nPos = nPItem = nPeek = nMaps = nClip = 0;
    sightThis = peekThis = pitemThis = clipMap = 0;
    mapsArea.TLX = mapsArea.TLY = mapsArea.BRX = mapsArea.BRY = -999;
}
// Dirty the stack the row's frame will reuse, so an uninitialised out pointer would not read as null.
static void Dirty() { unsigned char junk[4096]; memset(junk, 0x5a, sizeof junk); if (junk[7] == 1) printf("%d", junk[9]); }
bool RunRow(CAIStateGroup_OpinionReactionRoot* p) { Dirty(); return p->CallingChildrenIsRunnable(); }

// ---- reference model -----------------------------------------------------------------------------------------------------------
static long Ceil(float f) { long r = (long)f; if ((float)r < f) ++r; return r; }   // GFFloatToLongCeil for the finite test values
static bool In2D(const ThingFx* t, const C3DVector& c, float r) { float dx = t->pos.X - c.X, dy = t->pos.Y - c.Y; return dx * dx + dy * dy <= r * r; }
static float Dist(const C3DVector& a, const C3DVector& b)
{ float dx = a.X - b.X, dy = a.Y - b.Y, dz = a.Z - b.Z; return (float)sqrt((double)(dy * dy + dz * dz + dx * dx)); }
static bool Model(ThingFx* creature, ThingFx* target)
{
    if (creature->type == HUMAN_CHILD) return false;
    long r = Ceil(creature->sight);
    long cx = (long)creature->pos.X, cy = (long)creature->pos.Y;
    long tlx = cx - r, tly = cy - r, brx = cx + r, bry = cy + r;
    tlx = tlx < 0 ? 0 : (tlx > GRID ? GRID : tlx); tly = tly < 0 ? 0 : (tly > GRID ? GRID : tly);
    brx = brx < tlx ? tlx : (brx > GRID ? GRID : brx); bry = bry < tly ? tly : (bry > GRID ? GRID : bry);
    float d = Dist(creature->pos, target->pos);
    for (long y = tly; y < bry; ++y)
        for (long x = tlx; x < brx; ++x)
            for (NodeFx* n = (NodeFx*)cells[y * GRID + x]; n; n = n->next) {
                ThingFx* t = (ThingFx*)get<void*>(n->pmapwho, 4);
                if ((get<unsigned long>(t->u.b, 0x6c) & 0x1f) != 0x1f) continue;
                if (t != creature && t->type == HUMAN_CHILD && In2D(t, creature->pos, creature->sight) && In2D(t, target->pos, d))
                    return true;
            }
    return false;
}

// Argument checks common to every case that reaches the search.
static void CheckSearchArgs(ThingFx* creature, ThingFx* target, int k)
{
    long r = Ceil(creature->sight), cx = (long)creature->pos.X, cy = (long)creature->pos.Y;
    check(nSight == 2 && sightThis == creature->perceive, "GetSightRadius twice on the creature's perception TC", k);
    check(nPItem == 2 && pitemThis == rootStore.b + 0x6b, "ThingReactingTo (+0x6B) dereferenced twice", k);
    check(nPeek == 1 && peekThis == rootStore.b, "PeekThingSearchTools on this", k);
    check(nMaps == 1 && mapsArea.TLX == cx - r && mapsArea.TLY == cy - r && mapsArea.BRX == cx + r && mapsArea.BRY == cy + r,
          "scan box = creature position truncated +- ceil(sight)", k);
    check(nClip == 1 && clipMap == mapStore.b && clipArea.TLX == mapsArea.TLX && clipArea.BRY == mapsArea.BRY, "clip on the same box", k);
    float d = Dist(creature->pos, target->pos);
    for (int i = 0; i < ncalls; ++i)
        if (calls[i].kind == 'N') check(calls[i].filterThing == creature, "IsNot filter holds the creature", k);
    // Call order per scanned candidate: IsNot, [creature type], In2DRange(creature, sight), In2DRange(target, distance).
    int i = 0;
    while (i < ncalls) {
        check(calls[i].kind == 'N', "IsNot is the first filter", k);
        const void* cand = calls[i].thing; ++i;
        if (i < ncalls && calls[i].kind == 'R' && calls[i].thing == cand) {
            const FilterCall& a = calls[i];
            check(a.pos.X == creature->pos.X && a.pos.Y == creature->pos.Y && a.pos.Z == creature->pos.Z, "3rd filter centre = creature position", k);
            check(a.range == creature->sight, "3rd filter range = sight radius (not its ceiling)", k);
            ++i;
            if (i < ncalls && calls[i].kind == 'R' && calls[i].thing == cand) {
                const FilterCall& b = calls[i];
                check(b.pos.X == target->pos.X && b.pos.Y == target->pos.Y && b.pos.Z == target->pos.Z, "4th filter centre = reacted-to position", k);
                check(fabs(b.range - d) <= 1e-5f * (d + 1.0f), "4th filter range = |creature - reacted-to|", k);
                ++i;
            }
        }
    }
}

static int nfound, nnot;
static void Case(ThingFx* creature, ThingFx* target, int k, int expect = -1)
{
    Bind(creature, target);
    bool want = Model(creature, target);
    if (expect >= 0) check(want == (expect != 0), "model expectation", k);
    bool got = RunRow(root);
    check(got == want, "result", k);
    if (got) ++nfound; else ++nnot;
    check(nCreatureType >= 1, "creature type read", k);
    if (creature->type == HUMAN_CHILD) {
        check(nCreatureType == 1 && nSight == 0 && nPeek == 0 && nMaps == 0 && nPos == 0 && nPItem == 0, "child creature: nothing else called", k);
    } else {
        CheckSearchArgs(creature, target, k);
    }
}

static unsigned long seed = 12345;
static unsigned long Rnd() { seed = seed * 1103515245ul + 12345ul; return (seed >> 8) & 0xffffff; }
static float RndF(float lo, float hi) { return lo + (hi - lo) * (float)(Rnd() % 1000) / 1000.0f; }
static float Near(float c, float d) { float v = c + RndF(-d, d); return v < 0.0f ? 0.0f : (v > 15.99f ? 15.99f : v); }

int main()
{
    int k = 0;
    // 1. The creature is a child: false, nothing but the type read (each of the other types reaches the search).
    ResetWorld();
    { ThingFx* c = AddThing(8.5f, 8.5f, 0, HUMAN_CHILD, 3.0f, true); ThingFx* t = AddThing(9.5f, 8.5f, 0, NOT_HUMAN, 1.0f, false);
      AddThing(8.5f, 9.5f, 0, HUMAN_CHILD, 1.0f, true); Case(c, t, ++k, 0); }
    for (int ty = NOT_HUMAN; ty <= HUMAN_ELDERLY; ++ty) {
        if (ty == HUMAN_CHILD) continue;
        ResetWorld();
        ThingFx* c = AddThing(8.5f, 8.5f, 0, ty, 3.0f, true); ThingFx* t = AddThing(10.5f, 8.5f, 0, NOT_HUMAN, 1.0f, false);
        AddThing(9.5f, 8.5f, 0, HUMAN_CHILD, 1.0f, true);
        Case(c, t, ++k, 1);
    }
    // 2. Only the creature itself in range (IsNot), adult / elderly / not-human neighbours (creature type), none at all.
    ResetWorld(); { ThingFx* c = AddThing(8.5f, 8.5f, 0, HUMAN_ADULT, 3.0f, true); ThingFx* t = AddThing(9.0f, 8.5f, 0, NOT_HUMAN, 0, false); Case(c, t, ++k, 0); }
    for (int ty = NOT_HUMAN; ty <= HUMAN_ELDERLY; ++ty) {
        ResetWorld(); ThingFx* c = AddThing(8.5f, 8.5f, 0, HUMAN_ADULT, 3.0f, true); ThingFx* t = AddThing(10.5f, 8.5f, 0, NOT_HUMAN, 0, false);
        AddThing(9.5f, 8.5f, 0, ty, 0, true); Case(c, t, ++k, ty == HUMAN_CHILD);
    }
    ResetWorld(); { ThingFx* c = AddThing(8.5f, 8.5f, 0, HUMAN_ADULT, 3.0f, false); ThingFx* t = AddThing(9.0f, 8.5f, 0, NOT_HUMAN, 0, false); Case(c, t, ++k, 0); }
    // A child not typed as a creature (flags 0x0F) is skipped by the template.
    ResetWorld(); { ThingFx* c = AddThing(8.5f, 8.5f, 0, HUMAN_ADULT, 3.0f, true); ThingFx* t = AddThing(10.5f, 8.5f, 0, NOT_HUMAN, 0, false);
      AddThing(9.5f, 8.5f, 0, HUMAN_CHILD, 0, true, 0x0f); Case(c, t, ++k, 0); }
    // 3. Sight filter boundary: child exactly at the sight radius (in), just beyond (out); radius fractional (ceil box) and whole.
    float sights[] = { 2.0f, 2.25f, 2.75f, 3.0f, 1.0f, 0.5f };
    for (unsigned si = 0; si < sizeof sights / sizeof sights[0]; ++si) {
        float s = sights[si];
        for (int side = 0; side < 3; ++side) {
            ResetWorld();
            ThingFx* c = AddThing(8.25f, 8.75f, 1.0f, HUMAN_ELDERLY, s, true);
            ThingFx* t = AddThing(8.25f + 6.0f, 8.75f, 0, NOT_HUMAN, 0, false);   // far: the distance filter passes
            float dx = side == 0 ? s : (side == 1 ? s + 0.25f : -s);
            AddThing(8.25f + dx, 8.75f, 5.0f, HUMAN_CHILD, 0, true);
            Case(c, t, ++k);
        }
    }
    // 4. Distance filter: child nearer the reacted-to thing than the creature is (in), farther (out), equal (in).
    for (int side = 0; side < 3; ++side) {
        ResetWorld();
        ThingFx* c = AddThing(8.5f, 8.5f, 0, HUMAN_ADULT, 4.0f, true);
        ThingFx* t = AddThing(11.5f, 8.5f, 0, NOT_HUMAN, 0, false);            // distance 3
        // side 0: 2 from t (in); side 1: 4 from t (out); side 2: (11.5, 11.5) exactly 3 from t (in) and sqrt(18) = 4.24 from the creature
        // (beyond sight 4: out). side 3 below: exactly 3 from t and inside sight.
        float x = side == 0 ? 9.5f : (side == 1 ? 7.5f : 11.5f), y = side == 2 ? 11.5f : 8.5f;
        AddThing(x, y, 0, HUMAN_CHILD, 0, true);
        Case(c, t, ++k, side == 0);
    }
    { // side 3: a child on the creature's own spot: exactly the creature's distance (3) from t, so the inclusive filter passes.
        ResetWorld();
        ThingFx* c = AddThing(8.5f, 8.5f, 0, HUMAN_ADULT, 4.0f, true);
        ThingFx* t = AddThing(11.5f, 8.5f, 0, NOT_HUMAN, 0, false);            // distance 3
        AddThing(8.5f, 8.5f, 0, HUMAN_CHILD, 0, true);
        Case(c, t, ++k, 1);
    }
    { // Z enters the distance: creature and target apart only in Z.
        ResetWorld();
        ThingFx* c = AddThing(8.5f, 8.5f, 0, HUMAN_ADULT, 4.0f, true);
        ThingFx* t = AddThing(8.5f, 8.5f, 2.0f, NOT_HUMAN, 0, false);
        AddThing(10.0f, 8.5f, 0, HUMAN_CHILD, 0, true);                             // 1.5 in 2D from t: within 2 (3-D distance)
        Case(c, t, ++k, 1);
    }
    // 5. The box: a child just inside each edge of ceil(sight) cells (found by the scan; the sight filter decides), and the creature
    //    near the map edge (negative box corners clipped).
    for (int e = 0; e < 4; ++e) {
        ResetWorld();
        ThingFx* c = AddThing(8.9f, 8.1f, 0, HUMAN_ADULT, 2.5f, true);
        ThingFx* t = AddThing(8.9f, 8.1f, 0.5f, NOT_HUMAN, 0, false);
        float px = e == 0 ? 6.4f : (e == 1 ? 10.95f : 8.9f), py = e == 2 ? 6.0f : (e == 3 ? 10.6f : 8.1f);
        AddThing(px, py, 0, HUMAN_CHILD, 0, true);
        Case(c, t, ++k);
    }
    ResetWorld(); { ThingFx* c = AddThing(0.5f, 0.5f, 0, HUMAN_ADULT, 3.0f, true); ThingFx* t = AddThing(2.5f, 0.5f, 0, NOT_HUMAN, 0, false);
      AddThing(1.5f, 0.5f, 0, HUMAN_CHILD, 0, true); Case(c, t, ++k, 1); }
    // 6. Several candidates: the first to pass all four filters ends the search.
    ResetWorld(); { ThingFx* c = AddThing(8.5f, 8.5f, 0, HUMAN_ADULT, 3.0f, true); ThingFx* t = AddThing(11.0f, 8.5f, 0, NOT_HUMAN, 0, false);
      AddThing(7.5f, 7.5f, 0, HUMAN_ADULT, 0, true); AddThing(6.0f, 8.5f, 0, HUMAN_CHILD, 0, true); AddThing(9.5f, 9.5f, 0, HUMAN_CHILD, 0, true);
      AddThing(10.5f, 10.5f, 0, HUMAN_CHILD, 0, true); Case(c, t, ++k, 1); }
    // 7. Random worlds against the model.
    for (int r = 0; r < 1500; ++r) {
        ResetWorld();
        static const int types[4] = { NOT_HUMAN, HUMAN_CHILD, HUMAN_ADULT, HUMAN_ELDERLY };
        float cx = RndF(0.0f, 15.99f), cy = RndF(0.0f, 15.99f);
        ThingFx* c = AddThing(cx, cy, RndF(-2, 2), types[Rnd() % 8 == 0 ? 1 : (Rnd() % 2 ? 2 : (Rnd() % 2 ? 3 : 0))], RndF(0.25f, 5.0f), Rnd() % 4 != 0);
        ThingFx* t = AddThing(Near(cx, 6.0f), Near(cy, 6.0f), RndF(-2, 2), NOT_HUMAN, 0, Rnd() % 2 == 0);
        int n = (int)(Rnd() % 8);
        for (int i = 0; i < n; ++i)
            AddThing(Near(cx, 5.0f), Near(cy, 5.0f), RndF(-2, 2), types[Rnd() % 3 == 0 ? 1 : Rnd() % 4], 0, true, Rnd() % 10 == 0 ? 0x0f : 0x1f);
        Case(c, t, 1000 + r);
    }
    check(nfound > 100 && nnot > 100, "both results covered", 0);
    if (failures) { printf("00986210_TEST FAIL %d (found %d, not %d)\n", failures, nfound, nnot); return 1; }
    printf("00986210_TEST PASS (%d targeted, 1500 random; found %d, not %d)\n", k, nfound, nnot);
    return 0;
}
