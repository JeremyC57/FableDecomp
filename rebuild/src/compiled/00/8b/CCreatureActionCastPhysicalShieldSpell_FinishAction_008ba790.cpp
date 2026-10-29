// CCreatureAction_CastPhysicalShieldSpell::FinishAction() -- base FinishAction, then (when the creature has the type-0xA7 component) clears that component's casting flag and level
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BA790, length 80 bytes. FinishAction is vtable slot 4 (protected `MAE`); the base is called directly here.
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup: type 0xA7 = bit 7 of the bitset
// dword at +0x34, which VC tests as the sign of the low byte, `mov cl ; test cl,cl ; jns`; landed shape of 0x00849320).
// Shape: one `push ecx` frame for the lookup key; the component rides in esi; its byte at +0xc and dword at +0x10 are
// cleared through the shared zero register. No engine header matches the retail layout (CCreatureActionBase is 0xa8
// bytes in retail, 0xb4 in the PDB), so `this` is modelled locally.
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

class CThingCreatureBase { public:                 // engine/CThingCreatureBase.h exists but is not used: only the interface lookup is modelled
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CTCSpellCastState {                  // the type-0xA7 component (name descriptive): a casting flag and a level
    unsigned char _pad0[0x0c];
    unsigned char m_isCasting;              // +0x0c
    long          m_castLevel;              // +0x10
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
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void FinishAction();            // 0x00859A20 (retail slot 4; the slot is immaterial to this direct call)
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_CastPhysicalShieldSpell : public CCombatActionBase { public:   // vtable 0x0127BDCC
    virtual void FinishAction();
};

void CCreatureAction_CastPhysicalShieldSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCSpellCastState* pState;
    if (PCreature.GetPItem()->GetInterface(&pState, 0xa7)) {
        pState->m_isCasting = 0;
        pState->m_castLevel = 0;
    }
}