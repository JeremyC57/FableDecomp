// CThingCreatureBase::AutoPlaceNear(const C3DVector& position, float radius) -- find a wander position near the point and, if one exists, move the creature there (navigation reset before and after the physics reset)
// Leaf: ?AutoPlaceNear@CThingCreatureBase@@QAE_NABVC3DVector@@M@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00665DF0, 149 bytes (`ret 8`).
// Callees: CInterfaceMap::LowerBound @ 0x0040F020, CTCCreatureNavigation::GetNearbyWanderPosition(const C3DVector*, float,
//   C3DVector*, bool) @ 0x006CB6E0, CTCCreatureNavigation::ResetNavigator @ 0x006CB6B0 (forwards to its +0xc object's
//   slot 18; label "CScriptThing::operator==" propagated), CTCPhysicsBase::ResetPosition slot 31 (+0x7c).
// Layout: retail CThing: interface bitset +0x20 (TC 7 = CTCCreatureNavigation: bit 7 of the byte +0x20), CVectorMap +0x44,
//   physics component +0x60.
// Shape: `T* p = 0; if (HasTC(k)) p = GetTC(k);` (the preloaded `xor esi,esi`), the result vector initialised from the
//   argument through a `__forceinline` memberwise copy ctor (dword copies straight from the argument pointer in eax;
//   the implicit copy goes through `mov ecx,eax`), the `false` pushed first.
#include "engine/C3DVector.h"
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCCreatureNavigation { public:
    bool GetNearbyWanderPosition(const C3DVector* from, float radius, C3DVector* result, bool flag);    // 0x006CB6E0
    void ResetNavigator();                      // 0x006CB6B0
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& position);     // slot 31 (+0x7c)
};

class CThingCreatureBase : public CThing { public:
    bool AutoPlaceNear(const C3DVector& position, float radius);
};

bool CThingCreatureBase::AutoPlaceNear(const C3DVector& position, float radius)
{
    CTCCreatureNavigation* navigation = 0;
    if (HasTC(TCI_NAVIGATION))
        navigation = ((CTCCreatureNavigation*)&GetTC(TCI_NAVIGATION));
    C3DVector placed = position;
    if (navigation->GetNearbyWanderPosition(&position, radius, &placed, false)) {
        navigation->ResetNavigator();
        PhysicsTC->ResetPosition(placed);
        navigation->ResetNavigator();
        return true;
    }
    return false;
}