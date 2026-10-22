// Behaviour test for 0x008C1600 CCombatAction_StrikeResponseJackOfBladesIntoRush::GetValidRushPosition: runs the
// reconstructed body over stub worlds, things, defs and navigation; checks the GetValidPositions arguments (world map from
// world slot 12, the thing-search tools, Jack's position, rush-away distance + 2, false, no shape manager), the empty early
// out (false, def released, nothing freed), the two remove_if passes (near Jack with the rush-away distance, near the boss
// phase start with 21) and their erases, the random candidate order (seed * 0x24a1 + 0x24df, rotated right 13, modulo the
// remaining count), the navigation test from Jack's position with tolerance 0, the erase of rejected candidates, the copy of
// the accepted one into *out, and the final storage free and def release.
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __declspec(x)
#endif

static int g_failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++g_failures; } } while (0)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
static bool Same(const C3DVector& a, const C3DVector& b) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }
static float DistSq(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X, dy = a.Y - b.Y, dz = a.Z - b.Z;
    return dx * dx + dy * dy + dz * dz;
}

static int g_frees = 0;
static void* g_freed = 0;
static void TestFree(void* p) { ++g_frees; g_freed = p; std::free(p); }
#define free TestFree

struct CEmptyTag { CEmptyTag() { } };
static int g_copyCalls = 0;
static C3DVector* __fastcall CopyPositions(const C3DVector* first, const C3DVector* last, C3DVector* dest, const CEmptyTag&, long*)
{
    ++g_copyCalls;
    for (long n = (long)(last - first); n > 0; --n) {
        *dest = *first;
        ++first;
        ++dest;
    }
    return dest;
}
struct CPositionVector {
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CPositionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CPositionVector() { if (_M_start) free(_M_start); }
    C3DVector* begin() const { return _M_start; }
    C3DVector* end() const { return _M_finish; }
    long size() const { return (long)(_M_finish - _M_start); }
    C3DVector& operator[](long i) { return *(begin() + i); }
    C3DVector* erase(C3DVector* first, C3DVector* last)
    {
        C3DVector* newFinish = CopyPositions(last, _M_finish, first, CEmptyTag(), 0);
        _M_finish = newFinish;
        return first;
    }
    C3DVector* erase(C3DVector* pos)
    {
        if (pos + 1 != _M_finish)
            CopyPositions(pos + 1, _M_finish, pos, CEmptyTag(), 0);
        --_M_finish;
        return pos;
    }
};
struct CWorldMap { int Id; };
struct CThingSearchTools { int Id; };
struct CTCShapeManager { int Id; };

// GetValidPositions stub: records its arguments and hands back the scripted candidate list.
static C3DVector g_candidates[16];
static int g_candidateCount = 0;
static int g_gvpCalls = 0;
static const CWorldMap* g_gvpMap = 0;
static const CThingSearchTools* g_gvpSearch = 0;
static C3DVector g_gvpPos;
static float g_gvpRadius = 0.0f;
static bool g_gvpFlag = true;
static const CTCShapeManager* g_gvpShapes = (const CTCShapeManager*)1;
static void __fastcall GetValidPositions(CPositionVector& positions, const CWorldMap& map, const CThingSearchTools& search,
                                         const C3DVector& pos, float radius, bool flag, const CTCShapeManager* shapes)
{
    ++g_gvpCalls;
    g_gvpMap = &map; g_gvpSearch = &search; g_gvpPos = pos; g_gvpRadius = radius; g_gvpFlag = flag; g_gvpShapes = shapes;
    if (g_candidateCount > 0) {
        positions._M_start = (C3DVector*)std::malloc(sizeof(C3DVector) * g_candidateCount);
        for (int i = 0; i < g_candidateCount; ++i)
            positions._M_start[i] = g_candidates[i];
        positions._M_finish = positions._M_start + g_candidateCount;
        positions._M_end_of_storage = positions._M_finish;
    }
}
static unsigned long __fastcall GFROR13(unsigned long value)
{
    value &= 0xffffffffUL;
    return ((value >> 13) | (value << 19)) & 0xffffffffUL;
}
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
struct CWorld {
    CWorldMap          Map;
    CThingSearchTools  Tools;
    CThingSearchTools* PThingSearchTools;
    unsigned long      Seed;
    int                MapCalls;
    CWorld() : PThingSearchTools(&Tools), Seed(12345), MapCalls(0) { }
    virtual const CWorldMap& GetWorldMap() { ++MapCalls; return Map; }
    unsigned long* GetPRandomSeed() { return &Seed; }
};

struct CTCBase { };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry  entries[8];
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};
struct CDefBase {
    int  Destroyed;
    long RefCount;
    CDefBase() : Destroyed(0), RefCount(0) { }
    virtual ~CDefBase() { }
    virtual void Destroy() { ++Destroyed; }
};
struct CJackOfBladesBattleDef : CDefBase {
    float RushAwayDistance;
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    CJackOfBladesBattleDef* Battle;
    bool GetSubDef(CDefPointer<CJackOfBladesBattleDef>* out)
    {
        if (!Battle)
            return false;
        ++Battle->RefCount;
        out->Object = Battle;
        return true;
    }
};
struct CTCBoss : CTCBase {
    C3DVector PhaseStartPos;
};
struct NavCall { C3DVector From, To; float Tolerance; };
struct CTCCreatureNavigation : CTCBase {
    C3DVector Reachable;
    int       Calls;
    NavCall   Log[16];
    CTCCreatureNavigation() : Calls(0) { }
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance)
    {
        if (Calls < 16) { Log[Calls].From = from; Log[Calls].To = to; Log[Calls].Tolerance = tolerance; }
        ++Calls;
        return Same(to, Reachable);
    }
};
struct CThing {
    unsigned long   m_presentBits[9];
    CInterfaceMap   m_interfaces;
    CDefClassBase*  m_pDef;
    C3DVector       Position;
    int             PositionCalls;
    CThing() : m_pDef(0), Position(0.0f, 0.0f, 0.0f), PositionCalls(0)
    {
        std::memset(m_presentBits, 0, sizeof(m_presentBits));
        m_interfaces.first = m_interfaces.entries;
        m_interfaces.last = m_interfaces.entries;
    }
    void AddInterface(long type, CTCBase* component)
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        CInterfaceMap::SEntry* it = m_interfaces.last++;
        it->type = type;
        it->component = component;
    }
    const C3DVector& GetPosition() const { ++const_cast<CThing*>(this)->PositionCalls; return Position; }
    CDefClassBase* GetDef() const { return m_pDef; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

// remove_if stubs: drop the positions strictly within Distance of Pos (keeps order, returns the new end).
static int g_removeNear = 0, g_removePhase = 0;
static float g_nearDistance = 0.0f, g_phaseDistance = 0.0f;
static C3DVector g_nearPos, g_phasePos;
struct CDistancePredicate {
    float     Distance;
    C3DVector Pos;
    CDistancePredicate(float distance, const C3DVector& pos) : Distance(distance), Pos(pos) { }
    bool operator()(const C3DVector& p) const { return DistSq(p, Pos) < Distance * Distance; }
};
struct CPhaseDistancePredicate {
    float     Distance;
    C3DVector Pos;
    CPhaseDistancePredicate(float distance, const C3DVector& pos) : Distance(distance), Pos(pos) { }
    bool operator()(const C3DVector& p) const { return DistSq(p, Pos) < Distance * Distance; }
};
static C3DVector* __fastcall RemoveIf(C3DVector* first, C3DVector* last, CDistancePredicate pred)
{
    ++g_removeNear; g_nearDistance = pred.Distance; g_nearPos = pred.Pos;
    C3DVector* out = first;
    for (; first != last; ++first)
        if (!pred(*first)) *out++ = *first;
    return out;
}
static C3DVector* __fastcall RemoveIf(C3DVector* first, C3DVector* last, CPhaseDistancePredicate pred)
{
    ++g_removePhase; g_phaseDistance = pred.Distance; g_phasePos = pred.Pos;
    C3DVector* out = first;
    for (; first != last; ++first)
        if (!pred(*first)) *out++ = *first;
    return out;
}

class CCombatActionBase {
public:
    CBaseIntelligentPointer PCreature;
    CWorld*                 World;
    int                     WorldCalls;
    CCombatActionBase() : World(0), WorldCalls(0) { }
    CWorld* GetPWorld() { ++WorldCalls; return World; }
};
class CCombatAction_StrikeResponseJackOfBladesIntoRush : public CCombatActionBase {
public:
    bool GetValidRushPosition(C3DVector* out);
};

// ---- reconstructed body (src/008c1600.cpp) ----
bool CCombatAction_StrikeResponseJackOfBladesIntoRush::GetValidRushPosition(C3DVector* out)
{
    CDefPointer<CJackOfBladesBattleDef> def;
    PCreature.GetPItem()->GetDef()->GetSubDef(&def);
    CTCBoss* boss;
    PCreature.GetPItem()->GetInterface(&boss, 0xad);
    CPositionVector positions;
    CWorld* world = GetPWorld();
    CThing* creature = PCreature.GetPItem();
    const CThingSearchTools& search = *GetPWorld()->PThingSearchTools;
    GetValidPositions(positions, world->GetWorldMap(), search, creature->GetPosition(), def->RushAwayDistance + 2.0f, false, 0);
    if (positions.size() == 0)
        return false;
    C3DVector* last = positions.end();
    CThing* me = PCreature.GetPItem();
    float distance = def->RushAwayDistance;
    CDistancePredicate tooClose(distance, me->GetPosition());
    positions.erase(RemoveIf(positions.begin(), positions.end(), tooClose), last);
    positions.erase(RemoveIf(positions.begin(), positions.end(), CPhaseDistancePredicate(21.0f, boss->PhaseStartPos)),
                    positions.end());
    bool found = false;
    CTCCreatureNavigation* navigation;
    PCreature.GetPItem()->GetInterface(&navigation, 7);
    while (!found && positions.size() != 0) {
        unsigned long count = positions.size();
        unsigned long random = GFRandom(GetPWorld()->GetPRandomSeed());
        unsigned long index = count ? random % count : 0;
        CThing* self = PCreature.GetPItem();
        if (navigation->IsAbleToNavigateFromPosToPos(self->GetPosition(), positions[index], 0.0f)) {
            *out = positions[index];
            found = true;
        } else {
            positions.erase(positions.begin() + index);
        }
    }
    return found;
}
// ---- end body ----

static void Reset()
{
    g_frees = 0; g_freed = 0; g_copyCalls = 0; g_candidateCount = 0; g_gvpCalls = 0;
    g_gvpMap = 0; g_gvpSearch = 0; g_gvpRadius = 0.0f; g_gvpFlag = true; g_gvpShapes = (const CTCShapeManager*)1;
    g_removeNear = 0; g_removePhase = 0; g_nearDistance = 0.0f; g_phaseDistance = 0.0f;
}

struct Rig {
    CThing                  jack;
    CDefClassBase           defClass;
    CJackOfBladesBattleDef  battle;
    CTCBoss                 boss;
    CTCCreatureNavigation   nav;
    CWorld                  world;
    CCombatAction_StrikeResponseJackOfBladesIntoRush action;
    Rig()
    {
        battle.RushAwayDistance = 10.0f;
        defClass.Battle = &battle;
        jack.m_pDef = &defClass;
        jack.Position = C3DVector(0.0f, 0.0f, 0.0f);
        boss.PhaseStartPos = C3DVector(100.0f, 0.0f, 0.0f);
        nav.Reachable = C3DVector(-999.0f, -999.0f, -999.0f);
        jack.AddInterface(7, &nav);
        jack.AddInterface(0xad, &boss);
        action.PCreature.m_pItem = &jack;
        action.World = &world;
    }
};

static unsigned long NextIndex(unsigned long* seed, unsigned long count)
{
    unsigned long r = GFRandom(seed);
    return count ? r % count : 0;
}

int main()
{
    // 1: no valid positions -> false, out untouched, nothing freed, def released.
    {
        Reset();
        Rig rig;
        C3DVector out(7.0f, 8.0f, 9.0f);
        bool found = rig.action.GetValidRushPosition(&out);
        CHECK(!found);
        CHECK(Same(out, C3DVector(7.0f, 8.0f, 9.0f)));
        CHECK(g_gvpCalls == 1);
        CHECK(g_gvpMap == &rig.world.Map && g_gvpSearch == &rig.world.Tools && rig.world.MapCalls == 1);
        CHECK(Same(g_gvpPos, rig.jack.Position) && g_gvpRadius == 12.0f && g_gvpFlag == false && g_gvpShapes == 0);
        CHECK(g_frees == 0 && g_removeNear == 0 && g_removePhase == 0);
        CHECK(rig.battle.RefCount == 0 && rig.battle.Destroyed == 1);
        CHECK(rig.nav.Calls == 0);
    }
    // 2: filters remove the near-Jack and near-phase-start positions; random picks until the reachable one.
    {
        Reset();
        Rig rig;
        g_candidates[0] = C3DVector(3.0f, 0.0f, 0.0f);     // within 10 of Jack -> removed
        g_candidates[1] = C3DVector(30.0f, 0.0f, 0.0f);
        g_candidates[2] = C3DVector(95.0f, 0.0f, 0.0f);    // within 21 of the phase start -> removed
        g_candidates[3] = C3DVector(0.0f, 40.0f, 0.0f);
        g_candidates[4] = C3DVector(0.0f, 0.0f, 50.0f);
        g_candidates[5] = C3DVector(-60.0f, 0.0f, 0.0f);
        g_candidateCount = 6;
        rig.nav.Reachable = C3DVector(0.0f, 0.0f, 50.0f);
        // Expected walk over the four survivors.
        C3DVector remaining[4] = { g_candidates[1], g_candidates[3], g_candidates[4], g_candidates[5] };
        unsigned long count = 4, seed = rig.world.Seed;
        int expectedCalls = 0;
        C3DVector tried[8];
        for (;;) {
            unsigned long index = NextIndex(&seed, count);
            tried[expectedCalls++] = remaining[index];
            if (Same(remaining[index], rig.nav.Reachable))
                break;
            for (unsigned long i = index; i + 1 < count; ++i)
                remaining[i] = remaining[i + 1];
            --count;
        }
        C3DVector out(7.0f, 8.0f, 9.0f);
        bool found = rig.action.GetValidRushPosition(&out);
        CHECK(found);
        CHECK(Same(out, C3DVector(0.0f, 0.0f, 50.0f)));
        CHECK(g_removeNear == 1 && g_nearDistance == 10.0f && Same(g_nearPos, rig.jack.Position));
        CHECK(g_removePhase == 1 && g_phaseDistance == 21.0f && Same(g_phasePos, rig.boss.PhaseStartPos));
        CHECK(rig.nav.Calls == expectedCalls);
        for (int i = 0; i < expectedCalls && i < 16; ++i) {
            CHECK(Same(rig.nav.Log[i].To, tried[i]));
            CHECK(Same(rig.nav.Log[i].From, rig.jack.Position) && rig.nav.Log[i].Tolerance == 0.0f);
        }
        CHECK(rig.world.Seed == seed);
        CHECK(g_frees == 1);
        CHECK(rig.battle.RefCount == 0 && rig.battle.Destroyed == 1);
    }
    // 3: nothing reachable -> every survivor tried once and erased, false, out untouched.
    {
        Reset();
        Rig rig;
        g_candidates[0] = C3DVector(30.0f, 0.0f, 0.0f);
        g_candidates[1] = C3DVector(0.0f, 40.0f, 0.0f);
        g_candidates[2] = C3DVector(2.0f, 2.0f, 0.0f);     // near Jack -> removed
        g_candidateCount = 3;
        C3DVector out(7.0f, 8.0f, 9.0f);
        bool found = rig.action.GetValidRushPosition(&out);
        CHECK(!found);
        CHECK(Same(out, C3DVector(7.0f, 8.0f, 9.0f)));
        CHECK(rig.nav.Calls == 2);
        CHECK(!Same(rig.nav.Log[0].To, rig.nav.Log[1].To));
        CHECK(g_frees == 1);
        CHECK(rig.battle.RefCount == 0 && rig.battle.Destroyed == 1);
    }
    // 4: everything filtered away -> the loop never runs.
    {
        Reset();
        Rig rig;
        g_candidates[0] = C3DVector(1.0f, 0.0f, 0.0f);
        g_candidates[1] = C3DVector(100.0f, 5.0f, 0.0f);
        g_candidateCount = 2;
        C3DVector out(7.0f, 8.0f, 9.0f);
        bool found = rig.action.GetValidRushPosition(&out);
        CHECK(!found && rig.nav.Calls == 0 && g_frees == 1);
        CHECK(g_removeNear == 1 && g_removePhase == 1);
    }
    if (g_failures == 0)
        std::printf("008c1600_TEST PASS\n");
    return g_failures == 0 ? 0 : 1;
}
