// CCreatureAction_ScreamerDie::FinishAction() -- clear the attachments of the live effect thing's particle emitter (interface type 0x4b), run the base FinishAction, then kill the creature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8F30, length 132 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the effect pointer at +0x114 three times, PCreature at +8 once),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x4b = bit 11 of the bitset dword at +0x28, `test ch,8`),
// CTCDParticleEmitter::ClearAttachments @ 0x006E0DE0, CCombatActionBase::FinishAction @ 0x00859A20 (direct base call),
// CThing::Kill(bool) @ 0x004C9B80.
// Shape: the landed IS_THING_ALIVE idiom on the +0x114 pointer, then a NULL-INITIALISED emitter pointer (`xor edi,edi`, pushed inside
// the if: edi is only live there) filled by the out-parameter GetInterface and used unconditionally. `this` rides in ebx.
// No engine header for this class; modelled locally (CCombatActionBase is 0x114 bytes in retail, the effect pointer is the first own member).
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
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    void Kill(bool immediately);            // 0x004C9B80
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
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CTCDParticleEmitter {                // interface type 0x4b component
    void ClearAttachments();                // 0x006E0DE0
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

struct CCreatureAction_ScreamerDie : CCombatActionBase {
    CBaseIntelligentPointer PEffect;        // +0x114 CIntelligentPointer<CThing>: the scream effect thing
    virtual void FinishAction();
};

void CCreatureAction_ScreamerDie::FinishAction()
{
    if (IS_THING_ALIVE(PEffect)) {
        CTCDParticleEmitter* pEmitter = 0;
        PEffect.GetPItem()->GetInterface(&pEmitter, 0x4b);
        pEmitter->ClearAttachments();
    }
    CCombatActionBase::FinishAction();
    PCreature.GetPItem()->Kill(true);
}