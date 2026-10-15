// CAIStateGroup_MinionSitAtHome::IsRunnable() -- unless the idle scheduler prefers another idle, schedule the
// Grade: DIFFER(161v155) -- the random step divides by the literal 150 after the frame call (ours keeps the rotated seed in ebx across GFGetCurrentFrame and does `mov edi,96h ; div edi` after it, +push ebx) where retail divides first (`mov ecx,96h ; div ecx ; mov edi,edx`) and then calls; the const-reference range that fixed the same literal-divisor shape in 0x00984A20 does not fix the ordering here.
// sit-down 225 + random(0..149) frames from now and defer to the seat base.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00939180, length 155 bytes (+ int3 padding). Vtable slot 11.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0xD3 = CTCIdleScheduler, bit 19 of the
// bitset dword at +0x38), CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase*) @ 0x00801BE0, CAIStateGroupBase::
// GetPWorld @ 0x008FCFC0 (called for nothing, result dropped, as in the landed siblings), GetPWorldSeed @ 0x008FCFD0,
// GFROR13 @ 0x00497890, GFGetCurrentFrame @ 0x0049D870, CAIStateGroup_MinionSitOnSeatBase::IsRunnable @ 0x0090F9C0
// (tail, its bool returned).
// Shape: `if (GetInterface(&sched, 0xd3) && !sched->IsPreferredIdle(this)) return false;` (the false exit reuses
// the callee's al), then the random step with the RANGE PASSED BY CONST REFERENCE (`mov ecx,96h ; div ecx`: a
// constant divisor VC7.1 would otherwise turn into a magic multiply), `FrameToSitDown = GFGetCurrentFrame() + r
// + 225` (`lea edx,[eax+edi+0E1h]`), `return Base::IsRunnable();`. FrameToSitDown is +0x3c (engine header name;
// the header flattens the base so the class is modelled locally).
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

struct CTCIdleScheduler {                       // interface type 0xD3
    bool IsPreferredIdle(CAIStateGroupBase* group);     // 0x00801BE0
};

struct CAIStateGroup_MinionSitOnSeatBase : CAIStateGroupBase {
    unsigned char _seat[0x28];                  // +0x14..+0x3b UsesSeat members + POwner
    virtual bool IsRunnable();                  // 0x0090F9C0
    // a random frame count below `range`: the range arrives by const reference, so the literal is materialised
    // in a register and divided with `div` (the landed seed step: seed*0x24A1+0x24DF, GFROR13)
    __forceinline unsigned long RandomFrames(CWorld* world, const unsigned long& range)   // the world argument is unused (its accessor call is kept)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

struct CAIStateGroup_MinionSitAtHome : CAIStateGroup_MinionSitOnSeatBase {
    long FrameToSitDown;                        // +0x3c
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionSitAtHome::IsRunnable()
{
    CTCIdleScheduler* pScheduler;
    if (PBrain->PCreature->GetInterface(&pScheduler, 0xd3)) {
        if (!pScheduler->IsPreferredIdle(this))
            return false;
    }
    FrameToSitDown = GFGetCurrentFrame() + RandomFrames(GetPWorld(), 150) + 225;
    return CAIStateGroup_MinionSitOnSeatBase::IsRunnable();
}
