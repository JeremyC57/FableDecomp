// CTCCameraPointBuilding::vfunc_17(CCharString& error) const -- validity check: the camera point must hang off a live building through its owner link, may not be both a start and an exit point, and its inside/outside position must agree with the start/exit flags; writes the reason into `error` and returns false on the first failure
// Leaf: ?vfunc_17@CTCCameraPointBuilding@@QBE_NAAVCCharString@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075D9A0, 254 bytes (+ int3 padding), `ret 4`. Vtable 0x0123ECBC
//   (CTCCameraPointBuilding) slot 17, the slot the landed CTCCameraPointScripted::vfunc_17 0x0075CDF0 fills.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (inlined GetInterface for type 0x33 =
//   word 1 bit 19), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPos @ 0x004C73D0,
//   CThingBuilding::IsPointInside(const C3DVector&) @ 0x0082D000, CCharString::operator=(const char*) @ 0x0099EFE0.
// Strings: 0x01269FF0 "CAMERA_POINT_BUILDING is not attached to a building!", 0x0126A028 "CAMERA_POINT_BUILDING is both
//   start and end point!", 0x01269F30 "CAMERA_POINT_BUILDING is attached to something other than a building!",
//   0x01269FB8 "point is inside building but flagged as an exit point!", 0x01269F78 "point is not inside building but
//   is flagged as a start point!".
// Layout = Ego_r.pdb CTCCameraPointBuilding (0x44): InMyBuilding +0x40, ExitPoint +0x41, StartPoint +0x42; the
//   interface-0x33 component's owner link at the odd +0x0d (landed GetOwnerBuilding 0x0075D940); CThing ThingTypeFlags
//   +0x6c (0x40 = building), Dead bit 0 of +0x91.
// Shape: early-out ifs that each set `error` and `goto fail;` to ONE shared `fail: return false;` after `return true;`
//   (the shared exit keeps esi/edi pushed at entry; VC7.1 still duplicates the small epilogue into every failure); the
//   component is fetched with the inline out-param GetInterface before the start/exit test; IsPointInside's result is a
//   named bool tested in two ifs.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/C3DVector.h"
class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing;
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CThingBuilding : public CThing { public:
    bool IsPointInside(const C3DVector& point) const;   // 0x0082D000
};
#pragma pack(push, 1)
class CTCOwnedEntity { public:                         // the interface-0x33 component (owner link at +0x0d)
    unsigned char _pad00[0x0d];
    CIntelligentPointer<CThing> POwner;             // +0x0d
};
#pragma pack(pop)
struct CTCCameraPointBuilding {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x40 - 0x08];          // CTCCameraPointDefinitionBase members
    bool InMyBuilding;                          // +0x40
    bool ExitPoint;                             // +0x41
    bool StartPoint;                            // +0x42
    bool vfunc_17(CCharString& error) const;
};

bool CTCCameraPointBuilding::vfunc_17(CCharString& error) const
{
    const CTCOwnedEntity* owned;
    if (!ParentThing->PeekTC(TCI_OWNED_ENTITY, &owned)) {
        error = "CAMERA_POINT_BUILDING is not attached to a building!";
        goto fail;
    }
    if (StartPoint && ExitPoint) {
        error = "CAMERA_POINT_BUILDING is both start and end point!";
        goto fail;
    }
    CThing* owner = owned->POwner.Get();
    if (!owner || owner->IsDead() || !(owner->GetTypeFlags() & 0x40)) {
        error = "CAMERA_POINT_BUILDING is attached to something other than a building!";
        goto fail;
    }
    bool inside = ((CThingBuilding*)owner)->IsPointInside(ParentThing->GetPos());
    if (ExitPoint && inside) {
        error = "point is inside building but flagged as an exit point!";
        goto fail;
    }
    if (StartPoint && !inside) {
        error = "point is not inside building but is flagged as a start point!";
        goto fail;
    }
    return true;
fail:
    return false;
}