// CThingCreatureBase::IsThingInFOV(CThing* thing, bool flag) -- within the perception sight radius (default 10) and inside the perception field of view (default 0.5) of this creature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00665190, length 206 bytes, `ret 8`, bool in al.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0x34 = CTCPerceiveThing), CTCPerceiveThing::GetExtendedSightRadius
//   @ 0x007448F0 (the landed callers' spelling; the manifest repeats the label on 0x00744580), the component's field-of-view
//   getter @ 0x007445D0 (`PDef->[+0x28] * const`; manifest "CTCPhysicsBase::GetAirResistance" is propagated),
//   CThing::GetPosition @ 0x004C73D0, the creature's view-angle test @ 0x00664E50 (thing, fov, flag; manifest
//   "CVertexBufferWin32_CalculateViewingAngle" is propagated, real name unproven).
// Layout: retail CThing interface bitset +0x20 (TC 0x34 = word +0x24 bit 20), interface map +0x44.
// Shape: the two defaults are float locals stored as immediates and overwritten from the component; the delta is
//   `GetPosition() - thing->GetPosition()` through an inline C3DVector operator- (thing's position fetched first) and
//   its squared length the plain inline `X*X + Y*Y + Z*Z`; `lenSq <= r*r` tail-calls the angle test (`test ah,1 ; jne` to the false exit), else false.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

struct CTCPerceiveThing {
    float GetExtendedSightRadius();             // 0x007448F0
    float GetFieldOfView_007445d0();            // 0x007445D0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
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
    if ((GetPosition() - thing->GetPosition()).GetLengthSquared() <= sightRadius * sightRadius)
        return IsThingInViewAngle_00664e50(thing, fieldOfView, flag);
    return false;
}