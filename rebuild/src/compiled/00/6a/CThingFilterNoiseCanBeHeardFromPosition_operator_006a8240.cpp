// CThingFilter_NoiseCanBeHeardFromPosition::operator()(CThing* thing) const -- a noise-making thing passes when its distance from the listening position is within the smaller of the listener's hearing range and the thing's noise radius
// Leaf: ??RCThingFilter_NoiseCanBeHeardFromPosition
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation); local free operator- removed (the header member operator- is the same inline member-wise computation).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8240, length 178 bytes, `ret 4`, bool in al.
// Callees: CInterfaceMap::LowerBound const @ 0x004365B0 (TC 0x37, the noise-emitting component; class not identified),
//   CThing::GetPos @ 0x004C73D0.
// Layout: CThingFilter_NoiseCanBeHeardFromPosition (Ego_r.pdb, 16 bytes): Position +0, ListenersHearingRange +0xc; retail
//   CThing interface bitset +0x20 (TC 0x37 = word +0x24 bit 23), interface map +0x44; the component's noise radius at
//   +0x44.
// Shape: the HasTC test wraps the body (the false exit laid out last); GetTC inline; the squared distance is the plain inline `X*X + Y*Y + Z*Z` on an inline operator- delta
//   kept on the x87 stack; the range is the Dinkumware reference `min(radius, ListenersHearingRange)` (`Right < Left ?
//   Right : Left`, the radius copied into the dead argument slot so its address can be selected).
#include "fable_compiler.h"
#include "engine/C3DVector.h"
template<class T> inline const T& Min(const T& left, const T& right) { return right < left ? right : left; }

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CTCNoiseMaker_37 {                       // the component behind ETCInterfaceType 0x37
    unsigned char _pad00[0x44];
    float NoiseRadius;                          // +0x44
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    const C3DVector& GetPos() const;       // 0x004C73D0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return (T*)it->second;
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
        float distanceSquared = (thing->GetPos() - Position).GetSquaredMagnitude();
        float radius = noise->NoiseRadius;
        float range = Min(radius, ListenersHearingRange);
        return distanceSquared <= range * range;
    }
    return false;
}