// CTCHeroCentreDoorMarker::OnInitialActivate() -- when the parent thing has the hero-centre link component (interface 0x33), point PHeroCentre at the hero centre that component refers to
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F750, length 129 bytes (+15 int3 padding). Vtable 0x0126B044 slot 6.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined interface lookup), CBaseIntelligentPointer::
// GetPItem @ 0x00A01B50 (the component's pointer at +0xd, then the temporary), CBaseIntelligentPointer ctor @ 0x00A01B10 /
// SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10.
// Shape: the component (bit 19 of CThing +0x24 = interface 0x33, `test dword ptr`) is fetched through the inline GetInterface;
// `PHeroCentre = CIntelligentPointer<CThing>(item);` builds, assigns and destroys a frame temporary (no user dtor on
// CIntelligentPointer, so no vtable store before the base dtor).
// Layout (packed): PHeroCentre +0x1d; the interface-0x33 component keeps a packed CIntelligentPointer<CThing> at +0x0d.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
#pragma pack(push, 1)
class CTCHeroCentreLink {                       // interface 0x33
public:
    unsigned char _pad00[0x0d];
    CIntelligentPointer<CThing> PHeroCentre;    // +0x0d
};
class CTCHeroCentreDoorMarker : public CTCBase {    // vtable 0x0126B044
public:
    float Radius;                               // +0x0c
    long  DoorType;                             // +0x10
    bool  HeroInsideRadius;                     // +0x14
    CIntelligentPointer<CThing> PParticle;      // +0x15
    CIntelligentPointer<CThing> PHeroCentre;    // +0x1d
    virtual void OnInitialActivate();
};
#pragma pack(pop)

void CTCHeroCentreDoorMarker::OnInitialActivate()
{
    CTCHeroCentreLink* link;
    if (ParentThing->GetInterface(&link, 0x33))
        PHeroCentre = CIntelligentPointer<CThing>(link->PHeroCentre.GetPItem());
}