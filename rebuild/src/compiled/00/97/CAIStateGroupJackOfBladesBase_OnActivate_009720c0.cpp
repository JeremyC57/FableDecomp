// CAIStateGroup_JackOfBladesBase::OnActivate() -- clear the boss component's "defeated" flag when the group starts.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009720C0, length 66 bytes.
// Callee: CVectorMap::LowerBound @ 0x0040F020 (the inlined interface lookup, type 0xCE = bit 14 of the bitset
// dword at +0x38, tested with `test ch,40h`).
//
// Shape: `if (creature->GetInterface(&pBoss, 0xCE)) pBoss->m_flag18 = false;` -- unlike the Guildmaster /
// GoToPub bodies the lookup's bool IS tested, so the miss path falls straight to the epilogue and the
// uninitialised slot is never read. Frame: one `push ecx` for the key temporary. The engine header keeps
// PBrain as a pad, so the base layout is modelled locally.
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the same shape the landed
// GetInterface_0x.. bodies (0x007BEA08 ...) use, here inlined into the caller through GetTC<T>().
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
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

struct CTCJackOfBlades {                        // interface type 0xCE component
    unsigned char _pad[0x18];
    bool m_defeated;                            // +0x18
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_JackOfBladesBase : CAIStateGroupBase {
    unsigned char _jack[0x38];                  // +0x14 PSwordEmitter, PArenaCentre, PTargetThing, counters, vectors
    void OnActivate();
};

void CAIStateGroup_JackOfBladesBase::OnActivate()
{
    CTCJackOfBlades* pJack;
    if (PBrain->PCreature->GetInterface(&pJack, 0xCE))
        pJack->m_defeated = false;
}