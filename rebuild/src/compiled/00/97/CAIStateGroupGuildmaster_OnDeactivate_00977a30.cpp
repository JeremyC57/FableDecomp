// CAIStateGroup_Guildmaster::OnDeactivate() -- drop the Guildmaster's combat creature mode (0x3B) when the group ends.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00977A30, length 88 bytes.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (the inlined interface lookup, type 0x31 = bit 17 of the bitset
// dword at +0x24), CTCCreatureModeManager::HasMode(ulong) @ 0x0073A8A0 (manifest "CPlayer::HasMode" is
// propagated) and RemoveMode(ulong) @ 0x0073ABA0.
//
// Shape: the brain's creature (PBrain->PCreature at +0x18) is asked for its mode-manager component through the
// inlined GetInterface: the component pointer is an UNINITIALISED local (hit path: lookup result; miss path:
// retail reads the never-written slot `[esp+4]`, shared with the key temporary), then
// `if (HasMode(0x3B)) RemoveMode(0x3B);`. Frame: one `push ecx`. The engine header for the class keeps PBrain
// as a pad, so the base layout is modelled locally.
struct CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the same shape the landed
// GetInterface_0x.. bodies (0x007BEA08 ...) use, here inlined into the caller through GetTC<T>().
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

struct CTCCreatureModeManager {                 // interface type 0x31 component
    bool HasMode(unsigned long mode);           // 0x0073A8A0
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_Guildmaster : CAIStateGroupBase {
    unsigned char _guildmaster[0x28];           // +0x14 AttackNum, PEnemy, PLastTeleportMarker, TeleportMarkers, frames
    void OnDeactivate();
};

enum { CREATURE_MODE_GUILDMASTER_COMBAT = 0x3B };

void CAIStateGroup_Guildmaster::OnDeactivate()
{
    CTCCreatureModeManager* pModes;
    PBrain->PCreature->GetInterface(&pModes, 0x31);
    if (pModes->HasMode(CREATURE_MODE_GUILDMASTER_COMBAT))
        pModes->RemoveMode(CREATURE_MODE_GUILDMASTER_COMBAT);
}