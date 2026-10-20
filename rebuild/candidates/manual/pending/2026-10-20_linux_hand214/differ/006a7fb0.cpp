// CThingFilter_BoundingBoxOverlapsMap::operator()(CThing* thing) const -- a physical thing passes when the square around its bounding circle overlaps the map's area
// Leaf: ??RCThingFilter_BoundingBoxOverlapsMap
// Grade: DIFFER(220v220) -- frame-slot assignment only: retail homes the top-left coordinate copy at [esp+20h] (later reused for the thing box) and keeps the converted map Y values in [esp+8]/[esp+10h] the other way round; ours homes the copy at [esp+0Ch] (by-value C2DCoordI, converting-ctor and named-copy forms tried)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7FB0, length 218 bytes, `ret 4`, bool in al.
// Callees: CTCPhysicsBase::GetBoundingInfo(C3DVector* centre, float* radius) @ 0x006AF640 (engine_api.tsv).
// Layout: CThingFilter_BoundingBoxOverlapsMap (Ego_r.pdb, 8 bytes): WorldMap +0, MapNo +4; retail CWorldMap Maps
//   (CArray<CMapInfo>, first pointer) +0x20; CMapInfo (0x48 bytes) Area (C2DBoxI: TopLeft, BottomRight) +0; retail CThing
//   PhysicsTC +0x60.
// Shape: the radius out parameter lives in the dead thing slot; the map box is C2DBoxF(C2DVector(area.TopLeft),
//   C2DVector(area.BottomRight)) (arguments right to left: the bottom-right coordinate converted first, each C2DCoordI
//   copied into the frame before its `fild`s); the thing box is the centre/half-size constructor; the overlap test is
//   four early `return false` comparisons.
struct C2DCoordI { long X, Y; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(C2DCoordI c) : X((float)c.X), Y((float)c.Y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
struct C3DVector { float X, Y, Z; };
struct C2DBoxI { C2DCoordI TopLeft, BottomRight; };
struct C2DBoxF {
    C2DVector TopLeft, BottomRight;
    C2DBoxF(const C2DVector& topLeft, const C2DVector& bottomRight) : TopLeft(topLeft), BottomRight(bottomRight) { }
    C2DBoxF(const C3DVector& centre, float halfSize)
        : TopLeft(centre.X - halfSize, centre.Y - halfSize), BottomRight(centre.X + halfSize, centre.Y + halfSize) { }
    bool Overlaps(const C2DBoxF& other) const
    {
        if (TopLeft.X > other.BottomRight.X)
            return false;
        if (BottomRight.X < other.TopLeft.X)
            return false;
        if (TopLeft.Y > other.BottomRight.Y)
            return false;
        if (BottomRight.Y < other.TopLeft.Y)
            return false;
        return true;
    }
};
struct CMapInfo {                               // 0x48 bytes
    C2DBoxI Area;                               // +0x00
    unsigned char _pad10[0x48 - 0x10];
};
struct CMapInfoArray {                          // CArray<CMapInfo>
    CMapInfo* first;
    CMapInfo* last;
    CMapInfo* end;
    const CMapInfo& operator[](long n) const { return *(first + n); }
};
struct CWorldMap {
    unsigned char _pad00[0x20];
    CMapInfoArray Maps;                         // +0x20
};
struct CTCPhysicsBase {
    void GetBoundingInfo(C3DVector* centre, float* radius);   // 0x006AF640
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};

struct CThingFilter_BoundingBoxOverlapsMap {
    CWorldMap* WorldMap;                        // +0x00
    long MapNo;                                 // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_BoundingBoxOverlapsMap::operator()(CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (!physics)
        return false;
    C3DVector centre;
    float radius;
    physics->GetBoundingInfo(&centre, &radius);
    const C2DBoxI& area = WorldMap->Maps[MapNo].Area;
    C2DCoordI topLeft = area.TopLeft;
    C2DCoordI bottomRight = area.BottomRight;
    C2DBoxF mapBox((C2DVector(topLeft)), (C2DVector(bottomRight)));
    C2DBoxF thingBox(centre, radius);
    return mapBox.Overlaps(thingBox);
}
