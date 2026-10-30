// Grade: DIFFER(218v218) -- register choice only (ebp vs ebx for &PCreature); tried (this sweep): shared CVectorMap model with the same-TU 0x0040F020 LowerBound body, PDB callee CTCSolidToThing::RemoveThingNotSolidTo(const CThing*), bool HasTC test (worse 224)
// CCombatAction_KickThingOnGround::FinishAction() -- when the kicked thing is still alive, unlink kicker and target from each other's interface-0x4D component, then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087EBD0, 218 bytes (224 with padding), virtual FinishAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface, type
//   0x4D = bit 13 of +0x28), the component call 0x00770F20 (thiscall, CThing*; manifest "CThing_RemoveFromList"),
//   CCombatActionBase::FinishAction 0x00859A20.
// Layout: CCombatActionBase PCreature +0x08 (landed 0x008C2A80); the kick target CIntelligentPointer at +0xa8 (retail;
//   engine/CCombatAction_KickThingOnGround.h is pads only at these offsets). Thing model as the landed 0x0095D310.
// Shape: IS_THING_ALIVE re-fetches GetPItem for each test; each GetInterface's key lives in the frame; `this` is spilled
//   once to the frame because esi is reused for the map.
// Leaf: FinishAction
enum ETCInterfaceType { TC_INTERFACE_NONE = 0 };
class CTCBase;
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
};
}
template <class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() { } };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    __declspec(noinline) value_type* LowerBound(const K& key)       // 0x0040F020: the retail body in the TU (same-TU mod-ref)
    {
        long count = this->_M_finish - this->_M_start;
        value_type* first = this->_M_start;
        while (count > 0) {
            long half = count / 2;
            value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
    C Compare;
    bool Dirty;
};
typedef CVectorMap<ETCInterfaceType, CTCBase*> CInterfaceMap;

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad1[0x91 - 0x44 - sizeof(CInterfaceMap)];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->_M_finish || interfaceType < it->first)
                it = map->_M_finish;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCSolidToThing { public:                 // interface type 0x4D
    void RemoveThingNotSolidTo(const CThing* thing);   // 0x00770F20
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0xa8 - 0x10];          // +0x10
    void FinishAction();                        // 0x00859A20
};

struct CCombatAction_KickThingOnGround : CCombatActionBase {
    CBaseIntelligentPointer PTarget;            // +0xa8
    void FinishAction();
};

void CCombatAction_KickThingOnGround::FinishAction()
{
    if (IS_THING_ALIVE(PTarget)) {
        CTCSolidToThing* kickerComponent;
        if (PCreature.GetPItem()->GetInterface(&kickerComponent, 0x4d))
            kickerComponent->RemoveThingNotSolidTo(PTarget.GetPItem());
        CTCSolidToThing* targetComponent;
        if (PTarget.GetPItem()->GetInterface(&targetComponent, 0x4d))
            targetComponent->RemoveThingNotSolidTo(PCreature.GetPItem());
    }
    CCombatActionBase::FinishAction();
}
