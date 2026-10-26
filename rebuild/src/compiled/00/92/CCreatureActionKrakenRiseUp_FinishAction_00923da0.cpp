// CCreatureAction_KrakenRiseUp::FinishAction() -- set the flag byte +0x10 of the owner's interface-9 component, clear the owner's ImmuneToAttack bit, then the base FinishAction
// Leaf: ?FinishAction@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00923DA0, length 100 bytes (+ int3 padding), vtable slot 4 of 0x01290594
// (RTTI `?A0x29922355::CCreatureAction_KrakenRiseUp`; FableWin.pdb ?FinishAction@CCreatureAction_KrakenRiseUp@?A0xe2035276@@UAEXXZ).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, address CSE'd in ebx), CVectorMap::LowerBound
// @ 0x0040F020 (inlined GetInterface 9 = bit 9 of the bitset dword at +0x20, `test ch,2`), CCombatActionBase::FinishAction
// @ 0x00859A20 (direct base call, last).
// Shape: the UNCHECKED interface-9 accessor (absent arm reads the key word); `and byte ptr [eax+1c8h],0BFh` is a bool bit-field
// cleared in place through a NAMED local (`GetAICreature()->ImmuneToAttack = false` adds `add eax,1c8h` and a byte round trip) (CThingAICreature::ImmuneToAttack, bit 6 of +0x1c8; Ego_r.pdb +0x1f8).
class CTCBase;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xb4 - 0x92];
    float         m_health;                     // +0xb4
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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
class CTCCreatureModeManager { public:                 // interface type 0x31 component
    bool AddMode(unsigned long mode);           // 0x0073AE10
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CTCInterface9 {                          // the owner's interface-9 component (descriptive, as landed 0x008CF610)
    unsigned char _pad[0x10];
    bool          m_flag10;                     // +0x10
};
class CThingAICreature : public CThing { public:
    unsigned char _padb8[0x1c8 - 0xb8];
    bool PerformingVillageTaskFlag : 1;         // +0x1c8 bit 0 (Ego_r.pdb +0x1f8)
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;                    // bit 6
    bool EnableCreatureAutoPlacing : 1;         // bit 7
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void FinishAction();                // 0x00859A20 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    __forceinline CTCInterface9* GetInterface9() const
    {
        CTCInterface9* component;
        PCreature.GetPItem()->GetInterface(&component, 9);
        return component;
    }
    __forceinline CThingAICreature* GetAICreature() const { return static_cast<CThingAICreature*>(PCreature.GetPItem()); }
};
struct CCreatureAction_KrakenRiseUp : CCombatActionBase {   // vtable 0x01290594, sizeof 0x114
    virtual void FinishAction();
};

void CCreatureAction_KrakenRiseUp::FinishAction()
{
    GetInterface9()->m_flag10 = true;
    CThingAICreature* creature = GetAICreature();   // named local: the bit-field update addresses [eax+1c8h] directly
    creature->ImmuneToAttack = false;
    CCombatActionBase::FinishAction();
}