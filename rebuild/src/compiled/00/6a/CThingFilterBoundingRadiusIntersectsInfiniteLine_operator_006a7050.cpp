// CThingFilter_BoundingRadiusIntersectsInfiniteLine::operator()(const CThing& thing) const -- true when the thing's physics bounding sphere reaches the filter's infinite line
// Leaf: ??RCThingFilter_BoundingRadiusIntersectsInfiniteLine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7050, length 89 bytes (+ int3 padding), thiscall `ret 4`.
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector& centre, float& radius) const @ 0x006AF640, the squared
//   point-to-infinite-line distance helper @ 0x00A442E0 (__fastcall: ecx = line start, edx = line end, then the
//   point and a float& line parameter on the stack, result on st0, `ret 8`; unnamed in the PDBs, the catalog
//   says "Vector3_ClosestPointOnLineSegment" -- the name below is descriptive).
// Identity: Ego_r.pdb `??RCThingFilter_BoundingRadiusIntersectsInfiniteLine@@QBE_NABVCThing@@@Z` (name set only);
//   the caller is CThingSearchTools::GetAllThingsInRoughArea with a CFilter_Bind4 holding this filter. The
//   catalog's "CThingFilter_CheckBoundingIntersection" is a descriptive auto label.
// Layout: filter = {Start, End} C3DVectors (Ego_r, 24 bytes). Retail CThing PhysicsTC at +0x60 (Ego_r +0x70; the
//   retail CThing is 0x10 smaller there, as SerialisationSectionName +0x58 in 0x006A7C60), so a local model.
// Shape: no physics component -> false; else the bounding radius is returned into the dead `thing` argument slot,
//   the centre and the line parameter are frame locals, and the test is `distance_sq < radius * radius`
//   (`fld r ; fmul r ; fxch ; fcompp`), false on the `jp`. The distance must be a NAMED float local: the call
//   written inside the compare lets VC flip it to `r*r > d` (`test ah,41h ; jne`, no fxch: DIFFER 87v89).
class C3DVector {
public:
    float X, Y, Z;
};

class CTCPhysicsBase {
public:
    void GetBoundingInfo(C3DVector& centre, float& radius) const;   // 0x006AF640
};

float __fastcall GetSquaredDistanceToInfiniteLine(const C3DVector& start, const C3DVector& end,
                                                 const C3DVector& point, float& t);   // 0x00A442E0

class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
};

class CThingFilter_BoundingRadiusIntersectsInfiniteLine {
public:
    C3DVector Start;                                        // +0x00
    C3DVector End;                                          // +0x0c
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_BoundingRadiusIntersectsInfiniteLine::operator()(const CThing& thing) const
{
    if (thing.PhysicsTC) {
        C3DVector centre;
        float radius;
        thing.PhysicsTC->GetBoundingInfo(centre, radius);
        float t;
        float distance_sq = GetSquaredDistanceToInfiniteLine(Start, End, centre, t);
        if (distance_sq < radius * radius)
            return true;
    }
    return false;
}