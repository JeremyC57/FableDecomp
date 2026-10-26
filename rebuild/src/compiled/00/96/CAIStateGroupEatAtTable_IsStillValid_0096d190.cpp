// CAIStateGroup_EatAtTable::IsStillValid() -- drops creature mode 0x28; if the mode is still present the group is no longer valid, otherwise defers to the next validity check (virtual slot 15)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096D190, length 105 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x31 =
// CTCCreatureModeManager: bitset word 1 bit 17 -> `test dword ptr [eax+24h],20000h`),
// CTCCreatureModeManager::RemoveMode(ulong) @ 0x0073ABA0, CTCCreatureModeManager::HasMode(ulong) @ 0x0073A8A0 (the
// manifest's CPlayer::HasMode label is propagated), `this`'s virtual slot 15 (+0x3c).
// Shape: the MazeTeleport (0x009543B0) form -- the lookup's bool is not tested, so the miss path reads the
// uninitialised out slot (`mov esi,[esp+8]`); the mode manager stays in esi for both calls; `return false` when
// the mode survived, else the virtual's bool is returned (`call [edx+3Ch]` then the epilogue).
// Model: local (no engine header for this class).
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThingAICreature { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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

class CTCCreatureModeManager { public:                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
    bool HasMode(unsigned long mode);           // 0x0073A8A0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual bool IsStillValid();                // slot 14 (+0x38)
    virtual bool IsStateGroupStillValid();      // slot 15 (+0x3c)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_EatAtTable : CAIStateGroupBase {
    virtual bool IsStillValid();
};

bool CAIStateGroup_EatAtTable::IsStillValid()
{
    CTCCreatureModeManager* modes;
    PBrain->Creature->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x28);
    if (modes->HasMode(0x28))
        return false;
    return IsStateGroupStillValid();
}