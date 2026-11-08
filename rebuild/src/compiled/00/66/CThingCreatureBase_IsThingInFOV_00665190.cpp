// CThingCreatureBase::IsThingInFOV(CThing* thing, bool flag) -- within the perception sight radius (default 10) and inside the perception field of view (default 0.5) of this creature
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation); local free operator- removed (the header member operator- is the same inline member-wise computation).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00665190, length 206 bytes, `ret 8`, bool in al.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0x34 = CTCPerceiveThing), CTCPerceiveThing::GetExtendedSightRadius
//   @ 0x007448F0 (the landed callers' spelling; the manifest repeats the label on 0x00744580), the component's field-of-view
//   getter @ 0x007445D0 (`PDef->[+0x28] * const`; manifest "CTCPhysicsBase::GetAirResistance" is propagated),
//   CThing::GetPos @ 0x004C73D0, the creature's view-angle test @ 0x00664E50 (thing, fov, flag; manifest
//   "CVertexBufferWin32_CalculateViewingAngle" is propagated, real name unproven).
// Layout: retail CThing interface bitset +0x20 (TC 0x34 = word +0x24 bit 20), interface map +0x44.
// Shape: the two defaults are float locals stored as immediates and overwritten from the component; the delta is
//   `GetPos() - thing->GetPos()` through an inline C3DVector operator- (thing's position fetched first) and
//   its squared length the plain inline `X*X + Y*Y + Z*Z`; `lenSq <= r*r` tail-calls the angle test (`test ah,1 ; jne` to the false exit), else false.
#include "fable_compiler.h"
#include "engine/C3DVector.h"

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCPerceiveThing { public:
    float GetExtendedSightRadius();             // 0x007448F0
    float GetFieldOfView_007445d0();            // 0x007445D0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return (T*)it->second;
    }
};

class CThingCreatureBase : public CThing {
public:
    bool IsThingInViewAngle_00664e50(CThing* thing, float fieldOfView, bool flag);   // 0x00664E50
    bool IsThingInFOV(CThing* thing, bool flag);
};

bool CThingCreatureBase::IsThingInFOV(CThing* thing, bool flag)
{
    float sightRadius = 10.0f;
    float fieldOfView = 0.5f;
    if (UsingComponents[0x34 >> 5] & (1UL << (0x34 & 31))) {
        CTCPerceiveThing* perceive = GetTC<CTCPerceiveThing>(0x34);
        sightRadius = perceive->GetExtendedSightRadius();
        fieldOfView = perceive->GetFieldOfView_007445d0();
    }
    if ((GetPos() - thing->GetPos()).GetSquaredMagnitude() <= sightRadius * sightRadius)
        return IsThingInViewAngle_00664e50(thing, fieldOfView, flag);
    return false;
}