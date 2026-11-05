// CThingFilter_IsValidCombatSpline::operator()(CThing* thing) -- true when the thing's interface-0x3F component is a scripted camera spline (virtual type 7) valid for the filter's AnimIndex and Thing
// Leaf: ??RCThingFilter_IsValidCombatSpline
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085AFD0, length 93 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, the component's virtual slot 28
// (`call [edx+70h]`, the camera-point type), CTCCameraPointScriptedSpline::IsCombatSplineValidForAnimAndThing @ 0x0075D8B0.
// Layout: PDB CThingFilter_IsValidCombatSpline { long AnimIndex; CThing* Thing; }; interface 0x3F -> bitset word 1 bit 31
// (`test ecx,ecx ; jns`).
// Shape: the inner call's bool is returned directly; the false tail is shared.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCameraPointScriptedSpline { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetCameraPointType() const;              // slot 28 (+0x70)
    bool IsCombatSplineValidForAnimAndThing(long animIndex, CThing* thing) const;   // 0x0075D8B0
};

struct CThingFilter_IsValidCombatSpline {
    long    AnimIndex;                              // +0x00
    CThing* Thing;                                  // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsValidCombatSpline::operator()(CThing* thing) const
{
    const CTCCameraPointScriptedSpline* spline;
    if (thing->PeekTC(TCI_CAMERA_POINT, &spline) && spline->GetCameraPointType() == 7)
        return spline->IsCombatSplineValidForAnimAndThing(AnimIndex, Thing);
    return false;
}