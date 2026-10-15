// CAIStateGroup_MinionReactToNoise::Cleanup() -- (propagated label: this body is a creature-mode cleanup, see the
// note) take the owning thing out of creature mode 12 and clear two flags.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007338A0, length 102 bytes (+ int3 padding).
// LABEL NOTE: the manifest module label is propagated from the byte-similar CAIStateGroup_MinionReactToNoise::
// Cleanup @ 0x0091CFA0, but here the pointer at +4 is tested for the interface bitset DIRECTLY (`mov eax,[edi+4]
// ; test [eax+24h],20000h`), i.e. +4 is the CThing itself, not an AI brain, and the flags are at +0x62/+0x63:
// a creature-mode object of the 0x0073xxxx module (12 callers, e.g. 0x00733B35 / 0x00733FB4). Modelled locally.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x31 = CTCCreatureModeManager, bit
// 17 of the bitset dword at +0x24), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0 with 12.
// Shape: the mode manager is an uninitialised local filled by the inlined lookup (the miss path reads the slot
// and duplicates the tail); the two bools are cleared from one `xor al,al`, +0x63 then +0x62.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
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

struct CTCCreatureModeManager {                 // interface type 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};

struct CCreatureModeWithNoiseFlags {            // the real class is a creature mode: thing at +4, flags at +0x62/+0x63
    void* __vftable;
    CThing* m_pThing;                           // +0x04
    unsigned char _pad[0x5a];                   // +0x08..+0x61
    bool m_flag62;                              // +0x62
    bool m_flag63;                              // +0x63
    void Cleanup();
};

void CCreatureModeWithNoiseFlags::Cleanup()
{
    CTCCreatureModeManager* pModes;
    m_pThing->GetInterface(&pModes, 0x31);
    pModes->RemoveMode(12);
    m_flag63 = false;
    m_flag62 = false;
}