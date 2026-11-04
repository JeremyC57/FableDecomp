// CThingFilter_BoundingBoxOverlapsMap::operator()(const CThing& thing) const -- a physical thing passes when the square around its bounding circle overlaps the map's area
// Grade: DIFFER(220v220) -- frame slots: retail homes the top-left C2DCoordI copy at [esp+20h] (the later thing box) and the converted map Y values swapped in [esp+8]/[esp+10h]; PDB identity applied (operator()(const CThing&) const, GetBoundingInfo(C3DVector&,float&) const, C2DBoxI::GetTLPos + C2DCoordI::ToC2DCoordF, C2DBoxF(centre, half) ctor); named/temporary boxes, member Overlaps either way, named coords
// Leaf: ??RCThingFilter_BoundingBoxOverlapsMap@@QBE_NABVCThing@@@Z
// Flags: none
class C3DVector { public: float X, Y, Z; C3DVector() {} };
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
};
class C2DCoordI { public: long X, Y; C2DCoordI() {} C2DCoordI(long x, long y) : X(x), Y(y) {}
    C2DVector ToC2DCoordF() const { return C2DVector((float)X, (float)Y); } };
class C2DBoxI { public: long TLX, TLY, BRX, BRY;
    C2DCoordI GetTLPos() const { return C2DCoordI(TLX, TLY); }
    C2DCoordI GetBRPos() const { return C2DCoordI(BRX, BRY); } };
class C2DBoxF { public: float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DVector& tl, const C2DVector& br) : TLX(tl.X), TLY(tl.Y), BRX(br.X), BRY(br.Y) { }
    C2DBoxF(const C2DVector& centre, float halfSize) : TLX(centre.X - halfSize), TLY(centre.Y - halfSize), BRX(centre.X + halfSize), BRY(centre.Y + halfSize) { }
    float GetTLX() const { return TLX; } float GetTLY() const { return TLY; } float GetBRX() const { return BRX; } float GetBRY() const { return BRY; }
};
class CMapInfo { public: C2DBoxI Area; unsigned char _pad10[0x48 - 0x10]; };
template <class T> class CArray { public: T* first; T* last; T* end; const T& operator[](unsigned int n) const { return first[n]; } };
class CWorldMap { public: unsigned char _pad00[0x20]; CArray<CMapInfo> Maps; };
class CTCPhysicsBase { public: void GetBoundingInfo(C3DVector& centre, float& radius) const; };
class CThing { public: unsigned char _pad00[0x60]; CTCPhysicsBase* PhysicsTC; const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; } };
class CThingFilter_BoundingBoxOverlapsMap { public:
    const CWorldMap& WorldMap;
    long MapNo;
    bool operator()(const CThing& thing) const;
};
bool CThingFilter_BoundingBoxOverlapsMap::operator()(const CThing& thing) const
{
    const CTCPhysicsBase* physics = thing.GetPhysicsTC();
    if (!physics)
        return false;
    C3DVector centre;
    float radius;
    physics->GetBoundingInfo(centre, radius);
    const C2DBoxI& area = WorldMap.Maps[MapNo].Area;
    C2DCoordI tl = area.GetTLPos();
    C2DBoxF mapBox(tl.ToC2DCoordF(), area.GetBRPos().ToC2DCoordF());
    C2DBoxF thingBox(C2DVector(centre), radius);
    if (mapBox.GetTLX() > thingBox.GetBRX())
        return false;
    if (mapBox.GetBRX() < thingBox.GetTLX())
        return false;
    if (mapBox.GetTLY() > thingBox.GetBRY())
        return false;
    if (mapBox.GetBRY() < thingBox.GetTLY())
        return false;
    return true;
}
