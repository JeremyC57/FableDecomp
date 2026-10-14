// CAIStateGroup_MazeTeleport::IsRunnable() -- runnable when the boss component is ready for a phase change.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009543B0, length 91 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (the inlined interface lookup, type 0xAD = bit 13 of the bitset
// dword at +0x34), then CTCBoss::IsReadyForPhaseChange @ 0x007AEE90 whose byte result is normalised to a bool
// (`test al,al ; setne al` = `return byte ? true : false;` -- a plain `!= 0` widens through eax with
// `neg al ; sbb eax,eax ; neg eax`).
// Shape: uninitialised component local, lookup bool not tested (the miss path reads the dead slot `[esp]`),
// the call + normalisation duplicated on both paths. Base layout modelled locally (PDB names).
struct CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound -- the same shape the landed
// GetInterface_0x.. bodies use, here inlined into the caller through GetTC<T>().
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18 CThingAICreature*
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CTCBoss {                                // interface type 0xAD component
    unsigned char IsReadyForPhaseChange();      // 0x007AEE90
};

struct CAIStateGroup_MazeTeleport : CAIStateGroupBase {
    virtual bool IsRunnable();
};

bool CAIStateGroup_MazeTeleport::IsRunnable()
{
    CTCBoss* pBoss;
    PBrain->Creature->GetInterface(&pBoss, 0xAD);
    return pBoss->IsReadyForPhaseChange() ? true : false;
}
