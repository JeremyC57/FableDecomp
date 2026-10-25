// Behaviour test for 0x0076E5D0: CTCWeaponTrail::GetLastProjectileWeaponTrailPoints forwards the projectile trail list to the private overload and returns its result
#include <cstdio>

class CPointPairVector { public: int Count; };
class CWeaponTrailList { public: void* _M_node; };

class CTCWeaponTrail {
public:
    long NoTrails;
    long NoProjectileTrails;
    CWeaponTrailList Trail;
    CWeaponTrailList ProjectileWeaponTrail;
    mutable const CWeaponTrailList* SeenTrail;      // test probes
    mutable CPointPairVector* SeenPoints;
    bool Result;
    CTCWeaponTrail() : NoTrails(0), NoProjectileTrails(0), SeenTrail(0), SeenPoints(0), Result(false) { }
    bool GetLastProjectileWeaponTrailPoints(CPointPairVector& points) const      // the reconstructed body
    {
        return GetLastWeaponTrailPoints(points, ProjectileWeaponTrail);
    }
private:
    bool GetLastWeaponTrailPoints(CPointPairVector& points, const CWeaponTrailList& trail) const
    {
        SeenTrail = &trail; SeenPoints = &points; points.Count = 2; return Result;
    }
};

int main()
{
    CTCWeaponTrail t;
    CPointPairVector points;
    points.Count = 0;
    t.Result = true;
    bool r = t.GetLastProjectileWeaponTrailPoints(points);
    if (!r || t.SeenTrail != &t.ProjectileWeaponTrail || t.SeenPoints != &points || points.Count != 2) {
        std::printf("0076e5d0_TEST FAIL\n");
        return 1;
    }
    t.Result = false;
    if (t.GetLastProjectileWeaponTrailPoints(points)) {
        std::printf("0076e5d0_TEST FAIL\n");
        return 1;
    }
    std::printf("0076e5d0_TEST PASS\n");
    return 0;
}