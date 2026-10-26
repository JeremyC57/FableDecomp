// CAIStateGroup_KidWanderNearKids::OnReplace() -- when another group replaces this one, let the creature's
// idle scheduler pick a fresh idle.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00914C10, length 68 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (the inlined interface lookup, type 0xD3 = bit 19 of the bitset
// dword at +0x38), then CTCIdleScheduler::ChooseNewIdle @ 0x00801A50 on the component.
// Shape: `if (creature->GetInterface(&p, 0xD3)) p->ChooseNewIdle();` -- the lookup's bool is tested so the
// miss path falls straight to the epilogue; frame = one `push ecx` for the key temporary. The engine header
// flattens the base into pads, so the base layout is modelled locally (PDB names).
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound -- the same shape the landed
// GetInterface_0x.. bodies use, here inlined into the caller through GetTC<T>().
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18 CThingAICreature*
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

class CTCIdleScheduler { public:                       // interface type 0xD3 component
    void ChooseNewIdle();                       // 0x00801A50
};

struct CAIStateGroup_KidWanderNearKids : CAIStateGroupBase {
    virtual void OnReplace();
};

void CAIStateGroup_KidWanderNearKids::OnReplace()
{
    CTCIdleScheduler* pScheduler;
    if (PBrain->Creature->GetInterface(&pScheduler, 0xD3))
        pScheduler->ChooseNewIdle();
}