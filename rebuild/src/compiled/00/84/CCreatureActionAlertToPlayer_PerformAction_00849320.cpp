// CCreatureAction_AlertToPlayer::PerformAction() -- if the creature perceives a player-type thing, make it directly aware of it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00849320, length 86 bytes. Vtable slot 3 (PerformAction) of
// CCreatureAction_AlertToPlayer (0x0127357C).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the
// inlined GetInterface lookup for interface type 0x34 = bit 20 of the bitset dword at +0x24, landed shape of
// 0x00930DC0), CTCPerceiveThing::GetPPerceivedThing @ 0x007446A0 (`GetPItem` on the pointer at +0xc of the component),
// CTCPerceiveThing::SetAsDirectlyAwareOfPerceivedThing @ 0x007449B0.
// Shape: one `push ecx` frame for the lookup key; the component rides in esi; the perceived thing's type byte at +0x10 is
// compared with 2 (the player creature type) before the awareness call. No engine header exists for this class.
class CTCBase;

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x10];
    unsigned char m_type;                   // +0x10: 2 = player creature
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
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

struct CTCPerceiveThing_Calls {             // interface type 0x34 component; engine/CTCPerceiveThing.h exists but is not used: only the two calls are modelled
    CThing* GetPPerceivedThing();           // 0x007446A0
    void SetAsDirectlyAwareOfPerceivedThing();   // 0x007449B0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

class CCreatureAction_AlertToPlayer { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
};

void CCreatureAction_AlertToPlayer::PerformAction()
{
    CTCPerceiveThing_Calls* pPerceive;
    if (PCreature.GetPItem()->GetInterface(&pPerceive, 0x34)) {
        CThing* pPerceived = pPerceive->GetPPerceivedThing();
        if (pPerceived && pPerceived->m_type == 2)
            pPerceive->SetAsDirectlyAwareOfPerceivedThing();
    }
}