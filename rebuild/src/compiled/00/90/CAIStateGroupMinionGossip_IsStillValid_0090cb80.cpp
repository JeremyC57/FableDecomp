// CAIStateGroup_MinionGossip::IsStillValid() -- once relying on the gossipee, still valid only while the gossip partner
// (our CTCGossip's target) is alive and the partner's CTCGossip targets us back.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090CB80, length 185 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0xC1 = CTCGossip, bit 1 of the bitset
// dword at +0x38), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on CTCGossip's +0x0c target pointer (twice).
// Shape: GetInterface writes straight into the members PTCGossip (+0x1c) / PTCGossipOther (+0x20), which are then
// reloaded (`mov [edi+1ch],edx ; mov ecx,[edi+1ch]`); the presence-bit miss path just falls through to the reload.
// Engine header names (PDB); the header flattens the base, so the class is modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

class CTCGossip { public:
    unsigned char _pad[0xc];
    CBaseIntelligentPointerCtor PGossipTarget;  // +0x0c
};

struct CAIStateGroup_MinionGossip : CAIStateGroupBase {
    long FrameOfLastGossip;                     // +0x14
    bool Critical;                              // +0x18
    bool NowRelyingOnGossipee;                  // +0x19
    CTCGossip* PTCGossip;                       // +0x1c
    CTCGossip* PTCGossipOther;                  // +0x20
    virtual bool IsStillValid();
};

bool CAIStateGroup_MinionGossip::IsStillValid()
{
    if (NowRelyingOnGossipee) {
        PBrain->PCreature->GetInterface(&PTCGossip, 0xc1);
        CThing* other = PTCGossip->PGossipTarget.GetPItem();
        if (other != 0 && !(other->m_flags91 & 1)) {
            other->GetInterface(&PTCGossipOther, 0xc1);
            CThing* self = PBrain->PCreature;
            if (PTCGossipOther->PGossipTarget.GetPItem() == self)
                return true;
        }
        return false;
    }
    return true;
}