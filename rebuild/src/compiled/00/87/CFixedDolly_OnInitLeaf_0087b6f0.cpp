// CFixedDolly::OnInitLeaf() -- take the dolly's target thing (intelligent pointer at +0xa4), look up its interface type 0x3f component, and store that component's (or, for a kind other than 1, the fallback component's) centre into DollyVector (+0xc0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087B6F0, length 127 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the inlined
// GetInterface for type 0x3f = bit 31 of the bitset word at +0x24, tested with `jns`), the component's virtual slot 28 (+0x70, its kind),
// the CTCBase member 0x0075DB90 returning a C3DVector by hidden pointer (reads the owner at +4; manifest name
// "CTCShape_InitializeFromInterface" is a descriptive global label).
// Layout: Ego_r.pdb CFixedDolly (0xf0) puts DollyVector at +0xe4; the retail object is 0x24 smaller (DollyVector +0xc0), so `this` is
// modelled locally (the target intelligent pointer at +0xa4).
// Shape: the out-parameter GetInterface template with an UNINITIALISED out pointer (its frame word shared with the lookup key: the
// absent path reads garbage, as the landed 0x0069F480 family); the found component rides in esi. The `kind == 1 ? a : b` object of the
// centre call selects between the component (register) and a second, never-assigned component pointer read from the same shared
// frame word -- retail's `mov ecx,esi ; je ; mov ecx,[esp+8]`.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
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

class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetKind();                 // slot 28 (+0x70)
    CThing* m_pThing;                       // +0x04
    C3DVector GetCentre();                  // 0x0075DB90
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

struct CFixedDolly {
    virtual void v00();
    unsigned char           _pad04[0xa0];
    CIntelligentPointer<CThing> m_target;       // +0xa4
    unsigned char           _padac[0x14];
    C3DVector               DollyVector;    // +0xc0
    void OnInitLeaf();
};

void CFixedDolly::OnInitLeaf()
{
    CTCBase* pComponent;
    m_target.GetPItem()->GetInterface(&pComponent, 0x3f);
    CTCBase* pFallback;
    DollyVector = (pComponent->GetKind() == 1 ? pComponent : pFallback)->GetCentre();
}