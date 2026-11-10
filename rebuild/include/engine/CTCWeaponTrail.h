#ifndef FABLE_ENGINE_CTCWEAPONTRAIL_H
#define FABLE_ENGINE_CTCWEAPONTRAIL_H
// Retail opt-in (Codex pass18): full PDB fields, CTCBase hierarchy, native STLport12-byte vectors/4-byte lists.
#include "engine/CTCBase.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitiveWeaponTrail.h"
#include "fable_string.h"
class CTCWeaponTrail : public CTCBase {
public:
    void SetLastProjectileWeaponTrailPoint(const C3DVector& point);
private:
    long NoTrails;
    long NoProjectileTrails;
    long WeaponTrailGraphicIndex;
    CEnginePrimitiveHandle ProjectileWeaponTrailHandle;
    std::vector<CEnginePrimitiveHandle> WeaponTrailHandles;
    CEnginePrimitiveWeaponTrail::EWeaponTrailType TrailType;
    std::vector<bool> ActiveTrail;
    std::vector<long> InactiveTicks;
    std::vector<std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> > Trail;
    std::vector<std::pair<CCharString,CCharString> > HelperNames;
    std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> ProjectileWeaponTrail;
    long MinProjectileTrailTimer;
    bool WeaponTrailActive;
};
FABLE_STATIC_ASSERT(sizeof(CTCWeaponTrail)==108);
#endif
