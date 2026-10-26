// CCreatureAction_PlayerInteractionViewQuests::PerformAction() -- with both the player and the guide thing alive, open the guide's quest-select interface (type 0x6B component) and finish the action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845A70, length 155 bytes (+ int3 padding).
// Model: rebuild/include/engine/CCreatureAction_PlayerInteractionViewQuests.h is the flattened PDB layout and sits high against the bytes (CCreatureActionBase is 0xb4 bytes in the PDB, 0xa8 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8 twice, the guide pointer at +0xa8 three times),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x6B = bit 11 of the presence word at +0x2c, `test ch,8`),
// CTCHeroGuide::OpenQuestSelectInterface @ 0x007800B0, this action's virtual slot 4 (`call [edx+10h]`, FinishAction).
// Shape: two inlined IS_THING_ALIVE tests joined by `&&`; the GetInterface result is ignored (the absent arm reads the out pointer
// from the key's frame word) and the out pointer is null-tested instead.
class CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x45];             // +0x4c
    unsigned char m_flags91;                // +0x91 bit 0: dead
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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, landed 0x0094DBC0 form).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCHeroGuide { public:                       // interface type 0x6B
    void OpenQuestSelectInterface();        // 0x007800B0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_PlayerInteractionViewQuests : public CCreatureActionBase { public:
    CBaseIntelligentPointer PGuide;         // +0xa8 the thing whose quest guide is opened
    virtual void PerformAction();
};

void CCreatureAction_PlayerInteractionViewQuests::PerformAction()
{
    if (IS_THING_ALIVE(PCreature) && IS_THING_ALIVE(PGuide)) {
        CTCHeroGuide* guide;
        PGuide.GetPItem()->GetInterface(&guide, 0x6B);
        if (guide) {
            guide->OpenQuestSelectInterface();
            FinishAction();
        }
    }
}