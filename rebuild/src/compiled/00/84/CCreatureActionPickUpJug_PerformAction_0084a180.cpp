// CCreatureAction_PickUpJug::PerformAction() -- the table pick-up's PerformAction, then if the jug is still alive clear its jug component's "put down" flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084A180, length 119 bytes (+ int3 padding).
// Callees: CCreatureAction_PickUpThingAtTable::PerformAction @ 0x00849F70 (direct base call, first), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (the thing pointer at +0xa8, three times), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0xBB =
// bit 27 of the presence word at +0x34).
// Shape: IS_THING_ALIVE inlined (null test, then the +0x91 dead bit); the GetInterface result is ignored, so the absent arm stores
// through the uninitialised out pointer read from the key's frame word (`mov eax,[esp+4]`), and the byte store is tail-duplicated.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x45];             // +0x4c
    unsigned char m_flags91;                // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, landed 0x0094DBC0 form).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCJug {                             // interface type 0xBB
    unsigned char _pad00[0x10];
    bool m_putDown;                         // +0x10 (PickUpJug clears it, PutDownJug sets it)
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[0xa8 - 4];
};
class CCreatureAction_PickUpThingAtTable : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PThing;         // +0xa8 the thing picked up
    virtual void PerformAction();           // 0x00849F70
};
class CCreatureAction_PickUpJug : public CCreatureAction_PickUpThingAtTable { public:
    virtual void PerformAction();
};

void CCreatureAction_PickUpJug::PerformAction()
{
    CCreatureAction_PickUpThingAtTable::PerformAction();
    if (IS_THING_ALIVE(PThing)) {
        CTCJug* jug;
        PThing.GetPItem()->GetInterface(&jug, 0xBB);
        jug->m_putDown = false;
    }
}