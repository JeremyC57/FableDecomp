// CThingFilter_NoiseCanBeHeardFromPosition::operator()(CThing* thing) const -- a noise-making thing passes when its distance from the listening position is within the smaller of the listener's hearing range and the thing's noise radius
// Leaf: ??RCThingFilter_NoiseCanBeHeardFromPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8240, length 178 bytes, `ret 4`, bool in al.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0x37, the noise-emitting component; class not identified),
//   CThing::GetPosition @ 0x004C73D0.
// Layout: CThingFilter_NoiseCanBeHeardFromPosition (Ego_r.pdb, 16 bytes): Position +0, ListenersHearingRange +0xc; retail
//   CThing interface bitset +0x20 (TC 0x37 = word +0x24 bit 23), interface map +0x44; the component's noise radius at
//   +0x44.
// Shape: the HasTC test wraps the body (the false exit laid out last); GetTC inline; the squared distance is the plain inline `X*X + Y*Y + Z*Z` on an inline operator- delta
//   kept on the x87 stack; the range is the Dinkumware reference `min(radius, ListenersHearingRange)` (`Right < Left ?
//   Right : Left`, the radius copied into the dead argument slot so its address can be selected).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
template<class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CTCNoiseMaker_37 {                       // the component behind ETCInterfaceType 0x37
    unsigned char _pad00[0x44];
    float NoiseRadius;                          // +0x44
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

class CThingFilter_NoiseCanBeHeardFromPosition { public:
    C3DVector Position;                         // +0x00
    float ListenersHearingRange;                // +0x0c
    bool operator()(CThing* thing) const;
};

bool CThingFilter_NoiseCanBeHeardFromPosition::operator()(CThing* thing) const
{
    if (thing->HasTC(0x37)) {
        CTCNoiseMaker_37* noise = thing->GetTC<CTCNoiseMaker_37>(0x37);
        float distanceSquared = (thing->GetPosition() - Position).GetLengthSquared();
        float radius = noise->NoiseRadius;
        float range = Min(radius, ListenersHearingRange);
        return distanceSquared <= range * range;
    }
    return false;
}