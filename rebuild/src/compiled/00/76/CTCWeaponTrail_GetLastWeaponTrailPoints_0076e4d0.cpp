// CTCWeaponTrail::GetLastWeaponTrailPoints(std::vector<std::pair<C3DVector,C3DVector> >& points, const std::list<CWeaponTrailEntry>& trail) const -- append the top/bottom pairs of the newest (interpolation count + 1) entries of a weapon trail, oldest first; false when the trail has fewer than two entries
// Leaf: ?GetLastWeaponTrailPoints@CTCWeaponTrail@@ABE_NAAV?$vector@U?$pair@VC3DVector@@V1@@std@@V?$allocator@U?$pair@VC3DVector@@V1@@std@@@2@@std@@ABV?$list@VCWeaponTrailEntry@CEnginePrimitiveWeaponTrail@@V?$allocator@VCWeaponTrailEntry@CEnginePrimitiveWeaponTrail@@@std@@@3@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076E4D0, length 256 bytes (thiscall, `ret 8`; `this` is never read).
// Identity: Ego_r.pdb ?GetLastWeaponTrailPoints@CTCWeaponTrail@@ABE_NAAV?$vector@U?$pair@VC3DVector@@V1@@std@@...@std@@
//   ABV?$list@VCWeaponTrailEntry@CEnginePrimitiveWeaponTrail@@...@3@@Z (the private overload). Its two callers are the
//   public wrappers: 0x0076E5D0 GetLastProjectileWeaponTrailPoints (passes this+0x60, ProjectileWeaponTrail) and
//   0x0076E5F0 GetLastWeaponTrailPoints(points, long trail) (bounds-checks against NoTrails +0x0C, Trail at +0x48).
//   The catalog label "WeaponTrail_CollectPoints" is descriptive.
// Callees: vector<pair<C3DVector,C3DVector> >::_M_insert_overflow 0x007700D0 (push_back's growth path, fill 1 at end).
// Globals: 0x013828A0, a long the weapon-trail TU reads as its interpolation step count (descriptive name).
// Layout: CWeaponTrailEntry (Ego_r.pdb, 28 bytes) {C3DVector Top +0; C3DVector Bottom +0x0C; float CreationTime +0x18};
//   STLport list node {next +0, prev +4, data +8}; the output pair is 24 bytes.
// Shape: `long count = trail.size(); if (count <= 1) return false; it = trail.end(); advance(it, -(count < g + 1 ? count : g + 1));
//   for (; it != trail.end(); ++it) points.push_back(pair(it->Top, it->Bottom)); return true;` -- size() is the
//   STLport distance walk, advance is the bidirectional two-loop form.
// Same-TU bodies (chunk 892): retail's TU holds the COMDAT bodies of the out-of-line STLport members push_back reaches --
//   _M_insert_overflow 0x007700D0 and its __uninitialized_copy 0x0076F780 / __uninitialized_fill_n 0x0076F7D0 (STLport 4.6
//   _uninitialized.h false_type loops). With all three visible c2 knows the pair temporary's address is never captured, so the
//   in-place copy of the 24-byte temporary pipelines its loads one store ahead (0x0076E569..0x0076E58F) as in retail; with the
//   two helpers declared only, fill_n may keep `&x` and every store orders the next load (6 paired lines).
#define FABLE_RETAIL_STLP_UNINIT_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_ADVANCE
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"

extern long g_WeaponTrailInterpolationSteps;            // 0x013828A0

class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry {                           // Ego_r.pdb, 28 bytes
    public:
        C3DVector Top;                                  // +0x00
        C3DVector Bottom;                               // +0x0C
        float     CreationTime;                         // +0x18
    };
};

class CTCWeaponTrail {
public:
    typedef CEnginePrimitiveWeaponTrail::CWeaponTrailEntry CWeaponTrailEntry;
private:
    bool GetLastWeaponTrailPoints(std::vector<std::pair<C3DVector, C3DVector> >& points,
                                  const std::list<CWeaponTrailEntry>& trail) const;
};

bool CTCWeaponTrail::GetLastWeaponTrailPoints(std::vector<std::pair<C3DVector, C3DVector> >& points,
                                              const std::list<CWeaponTrailEntry>& trail) const
{
    long count = trail.size();
    if (count > 1) {
        long steps = g_WeaponTrailInterpolationSteps + 1;
        long n = count < steps ? count : steps;
        std::list<CWeaponTrailEntry>::const_iterator it = trail.end();
        std::advance(it, -n);
        for (; it != trail.end(); ++it)
            points.push_back(std::pair<C3DVector, C3DVector>(it->Top, it->Bottom));
        return true;
    }
    return false;
}