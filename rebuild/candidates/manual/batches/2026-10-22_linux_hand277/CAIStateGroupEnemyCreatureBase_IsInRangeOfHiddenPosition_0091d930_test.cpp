// Behaviour test for CAIStateGroup_EnemyCreatureBase::IsInRangeOfHiddenPosition @ 0x0091D930: furthest valid position, optional same-side filter.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CPositionVector {
    C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end_of_storage;
    CPositionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CPositionVector() { if (_M_start) free(_M_start); }
    C3DVector* begin() const { return _M_start; }
    C3DVector* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
};
struct CWorldMap { int unused; };
struct CThingSearchTools { int unused; };
struct CTCShapeManager;
static C3DVector g_pts[8]; static int g_npts = 0; static float g_radius = 0;
static void GetValidPositions(CPositionVector& positions, const CWorldMap& map, const CThingSearchTools& search,
                              const C3DVector& pos, float radius, bool flag, const CTCShapeManager* shapes)
{
    (void)map; (void)search; (void)pos; (void)flag; (void)shapes; g_radius = radius;
    if (g_npts == 0) return;
    positions._M_start = (C3DVector*)malloc(sizeof(C3DVector) * g_npts);
    memcpy(positions._M_start, g_pts, sizeof(C3DVector) * g_npts);
    positions._M_finish = positions._M_end_of_storage = positions._M_start + g_npts;
}
struct CThing { C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct CThingCreatureBase : CThing { };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CAIBrain { CThingCreatureBase* Creature; };
static CWorldMap g_map; static CThingSearchTools g_search;
struct CAIStateGroupBase {
    CAIBrain* PBrain;
    float GetHidingRange() { return 20.0f; }
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorldMap* PeekWorldMap() { return &g_map; }
    CThingSearchTools* PeekThingSearchTools() { return &g_search; }
};
struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CBaseIntelligentPointer PTargetThing;
    bool IsInRangeOfHiddenPosition(C3DVector* out, bool sameSideOfTarget);
};
bool CAIStateGroup_EnemyCreatureBase::IsInRangeOfHiddenPosition(C3DVector* out, bool sameSideOfTarget)
{
    CPositionVector positions;
    GetValidPositions(positions, *PeekWorldMap(), *PeekThingSearchTools(), GetCreature()->GetPosition(), GetHidingRange(),
                      true, 0);
    bool found = false;
    if (!positions.empty()) {
        C3DVector myPos = GetCreature()->GetPosition();
        float bestDistanceSquared = 0.0f;
        C3DVector bestPos;
        for (C3DVector* it = positions.begin(); it != positions.end(); ++it) {
            C3DVector candidate = *it;
            float dx = myPos.X - candidate.X;
            float dy = myPos.Y - candidate.Y;
            float dz = myPos.Z - candidate.Z;
            float distanceSquared = dz * dz + dy * dy + dx * dx;
            if (distanceSquared > bestDistanceSquared) {
                if (!sameSideOfTarget) {
                    bestDistanceSquared = distanceSquared;
                    bestPos = candidate;
                    found = true;
                } else {
                    C3DVector targetPos = PTargetThing.GetPItem()->GetPosition();
                    C3DVector toMe;
                    toMe.X = myPos.X - targetPos.X;
                    toMe.Y = myPos.Y - targetPos.Y;
                    toMe.Z = myPos.Z - targetPos.Z;
                    C3DVector toCandidate;
                    toCandidate.X = candidate.X - targetPos.X;
                    toCandidate.Y = candidate.Y - targetPos.Y;
                    toCandidate.Z = candidate.Z - targetPos.Z;
                    if (toMe.Z * toCandidate.Z + toMe.Y * toCandidate.Y + toMe.X * toCandidate.X > 0.0f) {
                        bestDistanceSquared = distanceSquared;
                        bestPos = candidate;
                        found = true;
                    }
                }
            }
        }
        if (found) {
            *out = bestPos;
            return true;
        }
    }
    return false;
}

int main()
{
    int bad = 0;
    CThingCreatureBase me; me.pos = C3DVector(0, 0, 0);
    CThing target; target.pos = C3DVector(10, 0, 0);
    CAIBrain brain; brain.Creature = &me;
    CAIStateGroup_EnemyCreatureBase sg; sg.PBrain = &brain; sg.PTargetThing.m_pItem = &target;
    C3DVector out(-1, -1, -1);
    g_npts = 0;
    if (sg.IsInRangeOfHiddenPosition(&out, false) || out.X != -1) ++bad;
    g_npts = 3; g_pts[0] = C3DVector(1, 0, 0); g_pts[1] = C3DVector(20, 0, 0); g_pts[2] = C3DVector(-5, 0, 0);
    if (!sg.IsInRangeOfHiddenPosition(&out, false) || out.X != 20 || g_radius != 20.0f) ++bad;
    /* same side as the creature relative to the target (x < 10): -5 qualifies, 20 does not, 1 is nearer */
    if (!sg.IsInRangeOfHiddenPosition(&out, true) || out.X != -5) ++bad;
    g_npts = 1; g_pts[0] = C3DVector(30, 0, 0);
    out = C3DVector(-1, -1, -1);
    if (sg.IsInRangeOfHiddenPosition(&out, true) || out.X != -1) ++bad;
    g_pts[0] = C3DVector(0, 0, 0);
    if (sg.IsInRangeOfHiddenPosition(&out, false)) ++bad;
    if (bad == 0) printf("0091d930_TEST PASS\n"); else printf("0091d930_TEST FAIL %d\n", bad);
    return 0;
}
