// Test for 0x0076E5F0 CTCWeaponTrail::GetLastWeaponTrailPoints(vector&, long) const: links the source object; an index
// below NoTrails hands that trail (a negative one the first trail) to the private list overload (stubbed here) and
// returns its answer; an index at or past NoTrails returns false without calling it.
#include <stdio.h>
class C3DVector { public: float X, Y, Z; };
class CEnginePrimitiveWeaponTrail { public: class CWeaponTrailEntry; };
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list { public: void* _M_node; };
}
typedef std::vector<std::pair<C3DVector, C3DVector> > CPointPairVector;
typedef std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> CWeaponTrailList;
static const void* g_trail;
static const void* g_points;
static int g_calls;
class CTCWeaponTrail {
public:
    void* __vftable; void* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    long NoTrails; long NoProjectileTrails;
    unsigned char _pad_0x14[0x48 - 0x14];
    std::vector<CWeaponTrailList> Trail;
    bool GetLastWeaponTrailPoints(CPointPairVector& points, long trail_index) const;
private:
    bool GetLastWeaponTrailPoints(CPointPairVector& points, const CWeaponTrailList& trail) const;
};
bool CTCWeaponTrail::GetLastWeaponTrailPoints(CPointPairVector& points, const CWeaponTrailList& trail) const
{
    ++g_calls; g_trail = &trail; g_points = &points;
    return trail._M_node != 0;
}
int main()
{
    static int nodes[3];
    CWeaponTrailList lists[3];
    lists[0]._M_node = &nodes[0]; lists[1]._M_node = 0; lists[2]._M_node = &nodes[2];
    CTCWeaponTrail w; w.NoTrails = 3;
    w.Trail._M_start = lists; w.Trail._M_finish = lists + 3; w.Trail._M_end_of_storage = lists + 3;
    CPointPairVector p;
    bool ok = w.GetLastWeaponTrailPoints(p, 2L) && g_trail == &lists[2] && g_points == &p;
    ok = ok && !w.GetLastWeaponTrailPoints(p, 1L) && g_trail == &lists[1];
    ok = ok && w.GetLastWeaponTrailPoints(p, -5L) && g_trail == &lists[0];
    ok = ok && g_calls == 3 && !w.GetLastWeaponTrailPoints(p, 3L) && g_calls == 3;
    if (!ok) { puts("0076e5f0_TEST FAIL"); return 1; }
    puts("0076e5f0_TEST PASS");
    return 0;
}
