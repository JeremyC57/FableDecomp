#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCVillage::InitialiseToggleTorchTypes(CThingObject&) -- if the object has a light component (type 0x27), toggles that light's active state
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- PeekTC as the ternary entry pick (landed 0x00584157 idiom): the light loads straight into ecx and the bool builds in al
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00659295, length 77 bytes (PDB `?InitialiseToggleTorchTypes@CTCVillage@@AAEXAAVCThingObject@@@Z`).
// Frame-pointer size TU (`push ebp ; mov ebp,esp` with no locals, the lookup key homed in the dead argument slot
// `[ebp+8]`): `optimize("y",off)` + `("s",on)`.
// Callees: std::bitset<>::test(size_t) const @ 0x00410DE0 (the object's interface bitset at +0x20), CVectorMap<
// ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the map at +0x44), CTCLight::SetActive(bool) @ 0x006412B0.
// Shape: the presence test is the out-of-line bitset call; the component fetch is the landed GetInterface lookup
// without its own test (key in a named local, the map a named pointer, end-or-larger-key -> end entry), `__forceinline`
// under /Os. The toggle is an inline CTCLight member so the component goes straight into ecx (a named light local
// keeps it in eax); `!Active` is `cmp byte ptr [ecx+1Ch],0 ; sete al ; push eax`. `this` is unused.
class CTCBase;

class CTCLight { public:
    unsigned char _pad[0x1C];
    bool          Active;                   // +0x1C
    void SetActive(bool active);            // 0x006412B0
    void ToggleActive() { SetActive(!Active); }
};

struct CBitset {
    bool test(unsigned long position) const;   // 0x00410DE0
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingObject { public:
    unsigned char _pad0[0x20];
    CBitset       TypeBits;                 // +0x20 std::bitset of interface types
    unsigned char _pad24[0x20];
    CInterfaceMap Interfaces;               // +0x44
    template<class T> __forceinline T* PeekTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
};

class CTCVillage { public:
    void InitialiseToggleTorchTypes(CThingObject& object);
};

void CTCVillage::InitialiseToggleTorchTypes(CThingObject& object)
{
    if (object.TypeBits.test(0x27)) {
        object.PeekTC<CTCLight>(0x27)->ToggleActive();
    }
}