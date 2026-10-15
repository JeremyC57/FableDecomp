// CCreatureAction_JackFall::Initialise() -- clear the effects on Jack's boss component (interface type 0xAD), then the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C11F0, length 103 bytes (+ int3 padding).
// Model: rebuild/include/engine/CCreatureAction_JackFall.h is the flattened PDB layout and sits high against the bytes (CCombatActionBase is 0x12c bytes in the PDB, 0x114 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface
// for type 0xAD = bit 13 of the presence word at +0x34, `test ch,20h`), CTCBoss::ClearEffects @ 0x007AEF60,
// CCombatActionBase::Initialise @ 0x00857520 (direct base call, last).
// Shape: the GetInterface result is ignored -- the absent arm reads the uninitialised out pointer from the key's frame word
// (`mov eax,[esp+4]`), and VC tail-duplicates the two calls into both arms; esi is only pushed inside the present arm.
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CTCBoss {                            // interface type 0xAD
    void ClearEffects();                    // 0x007AEF60
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12: 0x00857520
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

struct CCreatureAction_JackFall : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_JackFall::Initialise()
{
    CTCBoss* boss;
    PCreature.GetPItem()->GetInterface(&boss, 0xAD);
    boss->ClearEffects();
    CCombatActionBase::Initialise();
}