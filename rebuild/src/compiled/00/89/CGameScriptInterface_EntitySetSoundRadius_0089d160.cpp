// CGameScriptInterface::EntitySetSoundRadius(const CScriptThing& thing, float radius) const -- when the script thing is valid and its thing has a CTCPerceiveThing component (interface type 0x34), call CTCPerceiveThing::SetSoundRadius(radius)
// Leaf: ?EntitySetSoundRadius@CGameScriptInterface@@UBEXABVCScriptThing@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089D160, 91 bytes (+ int3 padding), thiscall, `ret 8`; CGameScriptInterface vtable 0x012615B8 slot 211.
// Identity: CGameScriptInterface::EntitySetSoundRadius(const CScriptThing&, float) const -- proven; evidence: FableWin.pdb name set holds
//   ?EntitySetSoundRadius@CGameScriptInterface@@UBEXABVCScriptThing@@M@Z (the catalog label agrees); it calls CTCPerceiveThing::SetSoundRadius
//   0x00744470 (proven by the stored member: CTCPerceiveThing::SoundRadius at +0x3C (Ego_r.pdb layout, no STL member before it)) on the interface-type-0x34 component. Family G045 (8 script setters).
// Callees: the script thing's virtual slot 75 (+0x12C, IsValid) and slot 11 (+0x2C, GetThing), CInterfaceMap::LowerBound
//   0x0040F020 (the thing's {type, component} map at +0x44), CTCPerceiveThing::SetSoundRadius(float) 0x00744470.
// Shape: `this` unused; the GetInterface template is `__forceinline` and the lookup key lives in the dead `thing` argument
//   slot; the component is not null-tested; the value is forwarded as a dword.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                       // a thing's sorted {type, component} vector map
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                          // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCPerceiveThing {
public:
    void SetSoundRadius(float radius);                              // 0x00744470
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetSoundRadius(const CScriptThing& thing, float radius) const
{
    if (thing.IsAlive()) {
        CThing* pThing = thing.GetPThing();
        CTCPerceiveThing* component;
        if (pThing->GetInterface(&component, 0x34))
            component->SetSoundRadius(radius);
    }
}