// CAIStateGroup_OpinionReactionRoot::WatchUpdate() -- keep looking at the thing reacted to for 30 + random(0..44)
// frames: 1 when the look action finishes (4) or the time is up, 0 on any other result (a long result).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00984A20, length 149 bytes.
// Callees: CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, GFGetCurrentFrame @ 0x0049D870
// (twice), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @
// 0x00A01B90 (the by-value CIntelligentPointer<const CThing> argument, vtable 0x01237F1C, converted in its argument
// slot from the member at +0x6b: the landed 0x0095EDF0 TurnToFace shape), CAIStateGroupBase::AIAction_LookAtThing
// @ 0x0097D6F0 (repeat 1, flags 0x21, turn speed 0.0027777f).
// Shape: the random step with the RANGE PASSED BY CONST REFERENCE (`mov ecx,2Dh ; div ecx`), the end frame in
// ebp (`lea ebp,[eax+esi+1Eh]`), the member address in ebx; a `do { r = LookAtThing(...); if (r != 2) return
// r == 4 ...; } while (GFGetCurrentFrame() < end); return true;` loop with the full-register results
// (`mov eax,1` / `xor eax,eax`). The member is unaligned at +0x6b (engine header, pack 1), so the class is
// modelled packed.
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

struct CBaseIntelligentPointerCtor {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointerCtor { };   // member type

struct CIntelligentPointerConstThing : CBaseIntelligentPointerCtor {   // vtable 0x01237F1C (argument type)
    CIntelligentPointerConstThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointerCtor() { SetPItem(other.GetPItem()); }
};

struct CTCTalk;

struct CAIStateGroupBaseL : CAIStateGroupBase {
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float turnSpeed);   // 0x0097D6F0
    // a random frame count below `range`: the range arrives by const reference, so the literal is materialised
    // in a register and divided with `div` (the landed seed step: seed*0x24A1+0x24DF, GFROR13)
    __forceinline unsigned long RandomFrames(const unsigned long& range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

#pragma pack(push, 1)
struct CAIStateGroup_OpinionReactionRoot : CAIStateGroupBaseL {
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    CIntelligentPointerThing ThingReactingTo;   // +0x6b
    long WatchUpdate();                         // full-register 1 / 0 result (`mov eax,1` / `xor eax,eax`)
};
#pragma pack(pop)

long CAIStateGroup_OpinionReactionRoot::WatchUpdate()
{
    long endFrame = GFGetCurrentFrame() + RandomFrames(45) + 30;
    do {
        long result = AIAction_LookAtThing(1, ThingReactingTo, 0x21, 0.0027777f);
        if (result != 2) {
            if (result != 4)
                return 0;
            break;                              // shares the `mov eax,1` exit of the timed-out loop
        }
    } while (GFGetCurrentFrame() < endFrame);
    return 1;
}