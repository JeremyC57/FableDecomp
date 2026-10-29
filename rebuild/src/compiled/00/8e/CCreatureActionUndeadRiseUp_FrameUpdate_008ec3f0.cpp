// CCreatureAction_UndeadRiseUp::FrameUpdate() -- while still rising, once the action is past half way: flag the creature's type-9 component (+0x10 = 1), drop the creature's underground mode and clear the rising flag; then the base FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC3F0, length 118 bytes.
// Callees: CCreatureActionBase::GetProgressRatio @ 0x006925E0 (float (elapsed) / (duration); the manifest's
// "CInputTypeMouseMovementEvent::operator==" / landed "GetMovementRatio" labels are propagated), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 9 =
// bit 9 of the bitset dword at +0x20, `test ch,2`), CCreatureAction_RiseUp::RemoveUndergroundMode @ 0x008B7640 (landed),
// CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call; the push-ecx frame keeps it a call, not a jump).
// Shape: `fcomp [0.5f] ; test ah,41h ; jne` = `ratio > 0.5f`; the component pointer comes from the landed out-parameter
// GetInterface template (no null test before the byte store). The rising flag at +0x118 is the first member after the
// retail CCreatureAction_RiseUp base (0x118 bytes: CCombatActionBase 0x114 + one word); modelled locally (the generated
// headers flatten the bases).
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
struct CTCType9 {                           // the creature's interface-type-9 component
    unsigned char _pad[0x10];
    unsigned char m_flag;                   // +0x10
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

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    float GetProgressRatio();               // 0x006925E0 (CCreatureActionBase)
public:
    virtual void FrameUpdate();   // 0x00859D80
};

class CCreatureAction_RiseUp : public CCombatActionBase { public:   // retail sizeof 0x118
    unsigned char _pad114[4];               // +0x114
    void RemoveUndergroundMode();           // 0x008B7640
};

class CCreatureAction_UndeadRiseUp : public CCreatureAction_RiseUp { public:
    unsigned char m_rising;                 // +0x118
    void FrameUpdate();
};

void CCreatureAction_UndeadRiseUp::FrameUpdate()
{
    if (m_rising) {
        if (GetProgressRatio() > 0.5f) {
            CTCType9* component;
            if (PCreature.GetPItem()->GetInterface(&component, 9))
                component->m_flag = 1;
            RemoveUndergroundMode();
            m_rising = 0;
        }
    }
    CCombatActionBase::FrameUpdate();
}