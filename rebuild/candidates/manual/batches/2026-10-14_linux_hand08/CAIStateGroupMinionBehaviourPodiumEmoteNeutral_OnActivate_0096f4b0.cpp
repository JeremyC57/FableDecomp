// CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::OnActivate() -- clear the emote counters and put the creature
// into creature mode 0x17 (the podium emote stance).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096F4B0, length 91 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (the inlined interface lookup, type 0x31 = bit 17 of the bitset
// dword at +0x24), then CTCCreatureModeManager::AddMode(ulong) @ 0x0073AE10 (bool result dropped).
// Shape: the two zero stores (+0x3c, +0x40) come first through the `xor eax,eax` on the raw `this`, then the
// uninitialised component local / untested lookup with the AddMode call duplicated on both paths (the miss
// path reads the dead slot `[esp]`). No engine header for this class; the layout is local, names descriptive.
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

struct CTCCreatureModeManager {                 // interface type 0x31 component
    bool AddMode(unsigned long mode);           // 0x0073AE10
};

struct CAIStateGroup_MinionBehaviourPodiumEmoteNeutral : CAIStateGroupBase {
    unsigned char _pad14[0x28];                 // +0x14 behaviour / crowd base state (untouched here)
    long m_emoteFrame;                          // +0x3c
    long m_emoteCount;                          // +0x40
    virtual void OnActivate();
};

void CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::OnActivate()
{
    m_emoteFrame = 0;
    m_emoteCount = 0;
    CTCCreatureModeManager* pModes;
    PBrain->Creature->GetInterface(&pModes, 0x31);
    pModes->AddMode(0x17);
}
