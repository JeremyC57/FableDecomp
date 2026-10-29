// CCreatureAction_BlockCounter::Initialise() -- notify the owner's combat component, clear the melee-flourish flag of a live target creature (state byte 2) that has one, then CCombatActionBase::Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00855920, length 189 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (owner +8 once, target +0xa8 four times: re-fetched per test),
// CVectorMap::LowerBound 0x0040F020 (inlined component lookup, type 0x30 = CTCCombat = bit 16 of the dword at +0x24),
// CTCCombat method 0x007338A0 (no arguments; manifest label "CAIStateGroup_MinionReactToNoise::Cleanup" is propagated, role
// unconfirmed), CTCCombat::SetMeleeFlourishValid(bool) 0x00733E10, CCombatActionBase::Initialise 0x00857520 (direct call).
// Shape: the owner's lookup is the entry-returning HasTC + FindEntry form (`mov ecx,[eax+4]`), the target's the out-parameter
// GetInterface form (`mov eax,[eax+4]` then `mov ecx,eax`) as the last term of a short-circuit chain (GOTCHAS).
// Target: the CCombatActionBase target pointer at +0xa8; thing flag byte +0x91 bit 0 = dead, byte +0x10 = thing state.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCombat { public:
    void SetMeleeFlourishValid(bool valid);     // 0x00733E10
public:
    void UnsetBlocking();   // 0x007338A0
};

class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char m_state;                      // +0x10
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->first <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
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

class CCombatActionBase { public:
    virtual void v00();
    long _pad04;
    CIntelligentPointer<CThing> Owner;              // +0x08 (the owning creature)
    unsigned char _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> Target;             // +0xa8 (the action's target)
    unsigned char _padb0[0x114 - 0xb0];
    virtual void Initialise();                  // 0x00857520
};

class CCreatureAction_BlockCounter : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_BlockCounter::Initialise()
{
    CThing* owner = Owner.GetPItem();
    if (owner->HasTC(0x30))
        ((CTCCombat*)owner->FindEntry(0x30)->second)->UnsetBlocking();
    CTCCombat* combat;
    if (Target.GetPItem() && !(Target.GetPItem()->m_flags91 & 1) && Target.GetPItem()->m_state == 2
        && Target.GetPItem()->GetInterface(&combat, 0x30))
        combat->SetMeleeFlourishValid(false);
    CCombatActionBase::Initialise();
}