// CThingFilter_IsInSameVolumeContextAsThing::CThingFilter_IsInSameVolumeContextAsThing(CThing* thing, bool override) -- constructor: Override from the argument, PThingInside = the thing's interface-0x75 component's intelligent-pointer item (+0x0C), else null
// Leaf: ??0CThingFilter_IsInSameVolumeContextAsThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8C00, length 87 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: PDB CThingFilter_IsInSameVolumeContextAsThing { CThing* PThingInside; bool Override; } (8 bytes);
// interface 0x75 -> bitset word 3 (+0x2C) bit 21 (`test dword ptr [eax+2Ch],200000h`); the component holds an
// intelligent pointer at +0x0C (`lea ecx,[eax+0Ch]`).
// Shape: Override (+4) is stored before PThingInside (+0) is cleared -- the bool is an init-list member and the pointer
// is assigned in the body (zero first, then the component's item when present).
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

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CTCInterface75 {                             // interface 0x75 component (class not established)
    unsigned char _pad[0x0c];
    CIntelligentPointer<CThing> m_inside;               // +0x0c
};

class CThingFilter_IsInSameVolumeContextAsThing { public:
    CThing* PThingInside;                           // +0x00
    bool    Override;                               // +0x04
    CThingFilter_IsInSameVolumeContextAsThing(CThing* thing, bool override);
};

CThingFilter_IsInSameVolumeContextAsThing::CThingFilter_IsInSameVolumeContextAsThing(CThing* thing, bool override)
    : Override(override)
{
    PThingInside = 0;
    CTCInterface75* component;
    if (thing->GetInterface(0x75, &component))
        PThingInside = component->m_inside.GetPItem();
}