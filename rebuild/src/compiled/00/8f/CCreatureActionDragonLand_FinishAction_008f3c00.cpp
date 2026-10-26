// CCreatureAction_DragonLand::FinishAction() -- flag the dragon's interface-0x3a component (bit 0 of +0x50), release this action's animation suspension on a creature, then the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F3C00, length 111 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap::LowerBound @ 0x0040F020,
//          CThingAnimatable::SetSuspendAnimation(bool, requester) @ 0x00833A10 (landed UndeadRiseUp shape),
//          CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: `this` lives in ebp (it is pushed as the requester), PCreature's address in edi; the bool out-parameter
//   GetInterface for bit 0x3a (`test dword ptr [eax+24h],4000000h`) with the component flag set by `or byte ptr`;
//   the thing-type byte +0x10 compared with 1; the creature for SetSuspendAnimation is a named local fetched before
//   the pushes; the base FinishAction is a direct call.
// Layout: CCombatActionBase (retail 0x114 bytes) with PCreature at +8; nothing else of the class is touched. The
//   0x3a component is not identified (only its flag byte at +0x50 is modelled).
class CTCBase;

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x10];
    unsigned char m_type;                   // +0x10: 1 = creature
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    void SetSuspendAnimation(bool suspend, const void* requester);   // 0x00833A10 (CThingAnimatable)
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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

struct CTCInterface3A {                     // interface type 0x3a component
    unsigned char _pad00[0x50];
    unsigned char m_flags50;                // +0x50 bit 0 set when the dragon lands
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
    virtual void FinishAction();            // 0x00859A20
};

class CCreatureAction_DragonLand : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_DragonLand::FinishAction()
{
    CTCInterface3A* tc;
    if (PCreature.GetPItem()->GetInterface(&tc, 0x3a))
        tc->m_flags50 |= 1;
    if (PCreature.GetPItem()->m_type == 1) {
        CThing* creature = PCreature.GetPItem();
        creature->SetSuspendAnimation(false, this);
    }
    CCombatActionBase::FinishAction();
}