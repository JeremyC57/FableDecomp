// CAIStateGroup_MinionHasInformation::CreatureWantsPlayersAttention() -- the creature is in the "has information"
// state (physics flag bit 2 set, bit 5 clear), the current player's hero is alive, and the creature's perceive
// component has the player in extended line of sight.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F290, length 136 bytes (+ int3 padding).
// Callees on PBrain->PCreature: the +0x1C8 flag getters @ 0x008300D0 (bit 2) and @ 0x00830100 (bit 5) (manifest
// "CanCollideWithLandscape" is propagated); the hero chain: the brain's player manager @ 0x008FCF80
// (PBrain->+0x20), CPlayerManager::GetCurrentPlayer @ 0x00449960, CPlayer::GetPHero @ 0x00487DC0 (the
// intelligent pointer at +0x2c, tail-jumps to GetPItem); CVectorMap::LowerBound @ 0x004365B0 (inlined
// GetInterface, type 0x34 = CTCPerceiveThing, bit 20 of the bitset dword at +0x24), CTCPerceiveThing::
// IsPlayerExtendedLineOfSight @ 0x00744830 (its bool returned as is).
// Shape: nested ifs with ONE `return false` at the end (an early `return false` per test duplicates the
// `xor al,al` exit); the hero pointer is a local tested for null
// and for the dead bit (+0x91 bit 0); the perceive component comes from the inlined
// lookup through the pointer-returning FindTC form (the component is loaded straight into ecx for the final
// call: `mov ecx,[eax+4]`, where the out-parameter form leaves it in eax); the last call's result is returned directly. Layout local (engine header flattens the base).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CThingCreatureFlags {                    // the creature's physics flag byte at +0x1c8
    bool HasInformationFlag();                  // 0x008300D0 (bit 2)
    bool InformationGivenFlag();                // 0x00830100 (bit 5)
};

class CPlayer { public:
    CThing* GetPHero();                         // 0x00487DC0 (+0x2c intelligent pointer -> GetPItem)
};
class CPlayerManager { public:
    CPlayer* GetCurrentPlayer_00449960();                // 0x00449960
};
struct CAIStateGroupBaseP : CAIStateGroupBase {
    CPlayerManager* GetPPlayerManager();        // 0x008FCF80 (PBrain->+0x20)
};

class CTCPerceiveThing { public:                       // interface type 0x34
    bool IsPlayerExtendedLineOfSight();         // 0x00744830
};

struct CInterfaceMapB {                         // the same vector map, this module's LowerBound instantiation
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThingB {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMapB m_interfaces;                // +0x44
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    // the pointer-returning lookup: the component lands straight in ecx for the call that follows
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMapB* map = &m_interfaces;
        CInterfaceMapB::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

struct CAIStateGroup_MinionHasInformation : CAIStateGroupBaseP {
    long LastFrameWaved;                        // +0x14
    long LastFrameWandered;                     // +0x18
    long FrameInitialised;                      // +0x1c
    long WavesWithoutCalling;                   // +0x20
    bool CreatureWantsPlayersAttention();
};

bool CAIStateGroup_MinionHasInformation::CreatureWantsPlayersAttention()
{
    if (((CThingCreatureFlags*)PBrain->PCreature)->HasInformationFlag()) {
        if (!((CThingCreatureFlags*)PBrain->PCreature)->InformationGivenFlag()) {
            CThing* hero = GetPPlayerManager()->GetCurrentPlayer_00449960()->GetPHero();
            if (hero != 0 && !(hero->m_flags91 & 1)) {
                CThingB* creature = (CThingB*)PBrain->PCreature;
                if (creature->HasTC(0x34))
                    return creature->FindTC<CTCPerceiveThing>(0x34)->IsPlayerExtendedLineOfSight();
            }
        }
    }
    return false;
}