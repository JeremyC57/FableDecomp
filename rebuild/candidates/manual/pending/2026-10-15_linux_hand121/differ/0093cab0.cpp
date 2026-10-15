// CAIStateGroup_NonVillageGuardPatrol::FindNextGuardPatrolPosition() -- advance the patrol node pointer to the
// Grade: DIFFER(104v104) -- register swap only: retail keeps the member pointer's address in esi and the successor in edi (`lea esi,[ecx+14h]` between the pushes), ours the other way round; every instruction otherwise identical.
// current node's successor, wrapping to the track's start node; stay put when there is no track.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093CAB0, length 104 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice on PNode at +0x14: the next pointer read at
// +0xD0, then the object for the start-node call), CThingTrackNode::PeekTrackStartNode @ 0x00836DA0, then the
// converting CIntelligentPointer temporary (vtable 0x0125A894 = CIntelligentPointer<CThingTrackNode>):
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice), GetPItem @
// 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: `next = PNode.GetPItem()->m_pNext; if (!next) { next = PNode.GetPItem()->PeekTrackStartNode(); if (!next)
// return; } PNode = next;` with the member's address kept in esi for both GetPItem calls and the final SetPItem.
// Layout local (no engine header for this class; PNode is +0x14, the node's successor pointer +0xd0: descriptive).
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
    virtual void InitStateGroup();              // slot 13 (+0x34)
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

struct CBaseIntelligentPointerCtor {            // the full CBaseIntelligentPointer interface (converting temporaries)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment (the landed
// 0x00902045 shape): base constructor, the derived vtable store, SetPItem on the temporary, GetPItem from it,
// SetPItem on the member, destructor.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CThingTrackNode {
    unsigned char _pad[0xd0];
    CThing* m_pNextNode;                        // +0xd0
    CThing* PeekTrackStartNode();               // 0x00836DA0
};

struct CAIStateGroup_NonVillageGuardPatrol : CAIStateGroupBase {
    CIntelligentPointerThing PNode;             // +0x14 CIntelligentPointer<CThingTrackNode>
    void FindNextGuardPatrolPosition();
};

void CAIStateGroup_NonVillageGuardPatrol::FindNextGuardPatrolPosition()
{
    CThing* next = ((CThingTrackNode*)PNode.GetPItem())->m_pNextNode;
    if (next == 0) {
        next = ((CThingTrackNode*)PNode.GetPItem())->PeekTrackStartNode();
        if (next == 0)
            return;
    }
    PNode = next;
}
