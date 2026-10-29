// CThingFilter_IsValidCombatSpline::operator()(CThing* thing) -- true when the thing's interface-0x3F component is a scripted camera spline (virtual type 7) valid for the filter's AnimIndex and Thing
// Leaf: ??RCThingFilter_IsValidCombatSpline
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085AFD0, length 93 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, the component's virtual slot 28
// (`call [edx+70h]`, the camera-point type), CTCCameraPointScriptedSpline::IsCombatSplineValidForAnimAndThing @ 0x0075D8B0.
// Layout: PDB CThingFilter_IsValidCombatSpline { long AnimIndex; CThing* Thing; }; interface 0x3F -> bitset word 1 bit 31
// (`test ecx,ecx ; jns`).
// Shape: the inner call's bool is returned directly; the false tail is shared.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;           // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCCameraPointScriptedSpline { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetCameraPointType();              // slot 28 (+0x70)
    bool IsCombatSplineValidForAnimAndThing(long animIndex, CThing* thing);   // 0x0075D8B0
};

struct CThingFilter_IsValidCombatSpline {
    long    AnimIndex;                              // +0x00
    CThing* Thing;                                  // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsValidCombatSpline::operator()(CThing* thing) const
{
    CTCCameraPointScriptedSpline* spline;
    if (thing->GetInterface(0x3F, &spline) && spline->GetCameraPointType() == 7)
        return spline->IsCombatSplineValidForAnimAndThing(AnimIndex, Thing);
    return false;
}