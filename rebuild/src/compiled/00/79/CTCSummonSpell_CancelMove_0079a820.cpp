// CTCSummonSpell::CancelMove() -- if the summoned minion is alive and carries the interface-0x9B timer component, stop that component once (copy its +0x18 value into +0x14 and set its stopped flag)
// Leaf: CancelMove
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079A820, length 111 bytes (padded row 112).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on Minion), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// @ 0x0040F020 (inlined GetInterface for interface 0x9B = bit 27 of CThing+0x30).
// Layout: Ego_r.pdb CTCSummonSpell: PDef +0x0C, JustReleased +0x10, Minion (CIntelligentPointer<CThingAICreature>) at the odd offset +0x11
// (`#pragma pack(1)`), FadeInTimer +0x1C. The 0x9B component: bool +0x0C, longs +0x14/+0x18 (class unnamed here).
// Shape: Minion is a reference local (`lea esi,[ecx+11h]`), each test calls GetPItem again (inline IsThingAlive); the component comes
// through the out-parameter GetInterface (`mov eax,[eax+4]`, fields addressed off eax).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThingAICreature>: 8 bytes
    void* m_vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTCMinionTimer {                         // interface 0x9B component
    unsigned char _pad00[0x0c];
    bool  Stopped;                              // +0x0c
    unsigned char _pad0d[0x14 - 0x0d];
    long  Current;                              // +0x14
    long  Target;                               // +0x18
};
#pragma pack(push, 1)
class CTCSummonSpell { public:
    void* __vfptr;                              // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    unsigned char _base08[4];
    void* PDef;                                 // +0x0c CDefPointer<const CSpecialAbilitiesSummonSpellDef>
    bool  JustReleased;                         // +0x10
    CBaseIntelligentPointer Minion;             // +0x11
    void CancelMove();
};
#pragma pack(pop)

static inline bool IsThingAlive(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() && !(p.GetPItem()->m_flags91 & 1);
}

void CTCSummonSpell::CancelMove()
{
    CBaseIntelligentPointer& minion = Minion;
    CTCMinionTimer* timer;
    if (IsThingAlive(minion) && minion.GetPItem()->GetInterface(&timer, 0x9b)) {
        if (!timer->Stopped) {
            timer->Current = timer->Target;
            timer->Stopped = true;
        }
    }
}