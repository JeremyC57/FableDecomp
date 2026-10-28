// CAIStateGroup_OpinionReactionRoot::GreetedThingReactingToBefore() -- when reacting to the player, the player
// component's "already greeted" flag.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095F7D0, length 89 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (a second copy of the interface-map lookup; type 0x4F = bit 15 of
// the bitset dword at +0x28, tested as `test ch,ch ; jns`).
// Shape: `if (ReactingToPlayer) { ... }` (+0x89; the false return is the final tail), then the player component is an UNINITIALISED local
// filled by the inlined GetInterface: the hit path reads the lookup result, the miss path reads the never-written
// frame slot (`mov eax,[esp]`, shared with the key temporary), and the flag byte at +0x341 is returned as the bool.
class CTCBase;
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the shape of the landed
// GetInterface_0x.. bodies, inlined into the AI bodies through GetTC<T>().
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;      // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;    // 0x004C73D0 (`[this+60h]+0Ch`)
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
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
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
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

class CWorld;

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0 (PBrain->+0x1c->+0x24)
public:
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0 (world random seed)
    void Terminate();                           // 0x008FCF40 (Terminated = true)
    bool IsStopping();                          // 0x008FD010
    long AIAction_DoNothing();                  // 0x0097E200 (EAIActionReturn)
};

struct CTCPlayerGreeting {                      // interface type 0x4F component
    unsigned char _pad[0x341];
    bool m_greeted;                             // +0x341
};

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:
    void* PTCTalk;                              // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    unsigned char ThingReactingTo[0x9];         // +0x6b CIntelligentPointer<CThing>
    unsigned char _pad_0x74[0x4];               // +0x74
    unsigned char CurrentUpdateFunc[0x10];      // +0x78
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool GreetedThingReactingToBefore();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::GreetedThingReactingToBefore()
{
    if (ReactingToPlayer) {
        CTCPlayerGreeting* pGreeting;
        PBrain->PCreature->GetInterface(&pGreeting, 0x4F);
        return pGreeting->m_greeted;
    }
    return false;
}