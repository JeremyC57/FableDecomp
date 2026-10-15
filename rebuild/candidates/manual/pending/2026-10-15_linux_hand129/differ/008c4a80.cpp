// CCreatureAction_ScorpionSummon::Initialise() -- base Initialise, put the scorpion's mode manager into mode 0x38, and if the creature's +0x10 state byte is 1 suspend its animation
// Grade: DIFFER(119v117) -- retail merges the two GetInterface arms straight into ecx (`mov ecx,[eax+4]` / `mov ecx,[esp+8]`); every source form here merges into eax and adds `mov ecx,eax` before AddMode (+2 bytes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4A80, length 117 bytes (+ int3 padding).
// Callees: CCombatActionBase::Initialise @ 0x00857520 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// three times), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x31), CTCCreatureModeManager::AddMode @ 0x0073AE10,
// CThingAnimatable::SetSuspendAnimation(bool, long) @ 0x00833A10.
// Shape: the GetInterface result is ignored (absent arm reads the key's frame word straight into ecx); PCreature is addressed once
// (`lea edi,[esi+8]`); the last creature is a named local because retail fetches it before pushing the two arguments.
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
struct CTCCreatureModeManager {             // interface type 0x31
    bool AddMode(unsigned long mode);       // 0x0073AE10
};
struct CThingCreatureBase {
    unsigned char _pad00[0x10];
    unsigned char m_state;                  // +0x10 creature state byte (compared with 1; meaning not pinned down)
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};
struct CCreaturePointer : CBaseIntelligentPointer {
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)GetPItem(); }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12: 0x00857520
    unsigned char _pad04[4];                // +0x04
    CCreaturePointer PCreature;             // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

struct CCreatureAction_ScorpionSummon : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_ScorpionSummon::Initialise()
{
    CTCCreatureModeManager* modes;
    CCombatActionBase::Initialise();
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->AddMode(0x38);
    if (PCreature.GetCreature()->m_state == 1) {
        CThingCreatureBase* creature = PCreature.GetCreature();
        creature->SetSuspendAnimation(true, 0);
    }
}
