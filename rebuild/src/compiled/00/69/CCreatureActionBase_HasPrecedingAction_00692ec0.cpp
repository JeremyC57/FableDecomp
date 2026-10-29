// CCreatureActionBase::HasPrecedingAction() -- true when the action reports a preceding animation (virtual slot 36) and the creature's interface-70 component says it is not busy with one; false otherwise
// Grade: RELOCATION_MATCH -- the && query written inside `if (IsPlayingPrecedingAnimation()) { ... }` with the `return false` after it: the early false becomes retail's own trailing `xor al,al` block (the guard-clause `if (!x) return false;` form returns the tested al through the shared epilogue)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692EC0, length 97 bytes (+ int3 padding).
// Callees: this action's virtual slot 36 (+0x90, bool), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the inlined GetInterface lookup for type 70 = bit 6 of the bitset
// dword at +0x28, `test byte ptr [eax+28h],40h`), the component's bool query @ 0x006A1270 (manifest "CThing_Array_GetFirstPhysicalSwitch"
// is a propagated guess; it walks the component's 12-byte-element vector at +0xc/+0x10).
// Shape: three exits -- `xor al,al` for the trailing false (a bool store), while `!query()` is computed as an int (`mov eax,1` /
// `xor eax,eax`) and the trailing `return false` shares its zero block. One `push ecx` frame slot for the lookup key.
// Layout: engine/CCreatureActionBase.h (PDB) agrees on PCreature at +8; a local model carries the virtual slot.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // CVectorMap<ETCInterfaceType, CTCBase*>: the thing's sorted {type, component} map at +0x44
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
    template<class T> __forceinline long GetInterface(T** ppOut, long interfaceType)   // a BOOL (long) result in this TU: the
    {                                                                                 //   `&&` below is then materialised in eax
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return 1;
        }
        return 0;
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
class CTCCarrying { public:                     // the interface-70 component
public:
    bool IsPrimarySlotFree() const;   // 0x006A1270
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual bool IsPlayingPrecedingAnimation();   // slot 36 (+0x90)
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
    bool HasPrecedingAction();
};

bool CCreatureActionBase::HasPrecedingAction()
{
    if (IsPlayingPrecedingAnimation()) {
        CTCCarrying* pQueue;
        return PCreature.GetPItem()->GetInterface(&pQueue, 70) && !pQueue->IsPrimarySlotFree();
    }
    return false;
}