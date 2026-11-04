// CThingCompare_BestForTargeting2::operator()(const CTargetPoint& a, const CTargetPoint& b) const -- strict-weak "a is the better target": each point's vector score times its thing's multiplier, a wins when it beats b by more than DELTA_NOTIONAL_ZERO
// Grade: DIFFER(183v183) -- fstp of the first score scheduled after mov ecx,esi (retail before); retail-header rewrite b (QBE/NPlayerTargeting identity, DELTA_NOTIONAL_ZERO) + named/one-expression/ref-local/order forms unchanged
// Leaf: ??RCThingCompare_BestForTargeting2@@QBE_NABVCTargetPoint@NPlayerTargeting@@0@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008098B0, 183 bytes, `ret 8`, bool in al.
// Identity: FableWin.pdb / Ego_r.pdb ??RCThingCompare_BestForTargeting2@@QBE_NABVCTargetPoint@NPlayerTargeting@@0@Z; the callers
//   are the STLport sort helpers over the target-point vector (__median 0x0080A140, partition 0x0080A1B0, insertion 0x0080A340).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ),
//   CThingCompare_BestForTargeting2::GetThingCompareMultiplier 0x007F9E70 (?GetThingCompareMultiplier@CThingCompare_BestForTargeting2@@ABEMPBVCThing@@@Z),
//   CVectorCompare_BestForTargeting::GetCompareValueForPoint 0x007F9660 (?GetCompareValueForPoint@CVectorCompare_BestForTargeting@@QBEMABVC3DVector@@@Z).
// Global: DELTA_NOTIONAL_ZERO 0x0129BA3C (?DELTA_NOTIONAL_ZERO@@3MB, 1e-4f).
// Layout: NPlayerTargeting::CTargetPoint (Ego_r.pdb, 28 bytes): PThing +0x00, Index +0x08, TargetingPosition +0x0C,
//   FrameRejected +0x18. CThingCompare_BestForTargeting2: Hero +0x00, VectorCompare +0x04, EnemyPreferenceFactor +0x30.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
namespace NPlayerTargeting {
class CTargetPoint {
public:
    CIntelligentPointer<CThing> PThing;                 // +0x00
    long Index;                                         // +0x08
    C3DVector TargetingPosition;                        // +0x0c
    long FrameRejected;                                 // +0x18
};
}
class CVectorCompare_BestForTargeting {
public:
    float GetCompareValueForPoint(const C3DVector& point) const;   // 0x007F9660
private:
    unsigned char _body[0x2c];                          // AnchorPos, PreferredLine, the four compare floats, CompareInXYPlane
};
class CThingCompare_BestForTargeting2 {
public:
    bool operator()(const NPlayerTargeting::CTargetPoint& a, const NPlayerTargeting::CTargetPoint& b) const;
private:
    float GetThingCompareMultiplier(const CThing* thing) const;   // 0x007F9E70
    CThing* Hero;                                       // +0x00
    CVectorCompare_BestForTargeting VectorCompare;      // +0x04
    float EnemyPreferenceFactor;                        // +0x30
};

bool CThingCompare_BestForTargeting2::operator()(const NPlayerTargeting::CTargetPoint& a, const NPlayerTargeting::CTargetPoint& b) const
{
    const CThing* thingA = a.PThing.Get();
    const CThing* thingB = b.PThing.Get();
    float multiplierA = GetThingCompareMultiplier(thingA);
    float multiplierB = GetThingCompareMultiplier(thingB);
    C3DVector posA = a.TargetingPosition;
    C3DVector posB = b.TargetingPosition;
    float valueA = VectorCompare.GetCompareValueForPoint(posA) * multiplierA;
    if (valueA - VectorCompare.GetCompareValueForPoint(posB) * multiplierB > DELTA_NOTIONAL_ZERO)
        return true;
    return false;
}
