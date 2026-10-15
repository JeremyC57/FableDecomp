// CAIStateGroup_PrepareMeal::Cleanup() -- release the cooking pot's action point this creature had claimed.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946100, length 130 bytes (+ int3 padding). Vtable slot 9.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on PCookingPot at +0x14: the inlined
// IS_THING_ALIVE pair, then the object for the lookup), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface
// on the pot, type 0x2D = CTCActionPoints, bit 13 of the bitset dword at +0x24: `test ch,20h`), the action-point
// release @ 0x007FEF40 (`ret 4`; the manifest's "EditCyclePreviousKeyCamera" label is propagated) with
// ActionPointIndex.
// Shape: `if (ActionPointIndex != -1) if (IS_THING_ALIVE(PCookingPot)) { PCookingPot.GetPItem()->GetInterface(&ap,
// 0x2d); ap->FreeActionPoint(ActionPointIndex); }` -- the component is an uninitialised local filled by the inlined
// lookup (the miss path reads the slot and duplicates the call). PCookingPot +0x14 and ActionPointIndex +0x1c are the
// engine header names (the header flattens the base so the class is modelled locally).
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

struct CTCActionPoints {                        // interface type 0x2D
    void FreeActionPoint(long index);           // 0x007FEF40
};

struct CAIStateGroup_PrepareMeal : CAIStateGroupBase {
    CBaseIntelligentPointer PCookingPot;        // +0x14 CIntelligentPointer<CThing>
    long ActionPointIndex;                      // +0x1c
    float ActionPointPos_X, ActionPointPos_Y, ActionPointPos_Z;    // +0x20
    virtual void Cleanup();
};

void CAIStateGroup_PrepareMeal::Cleanup()
{
    if (ActionPointIndex != -1) {
        if (IS_THING_ALIVE(PCookingPot)) {
            CTCActionPoints* pActionPoints;
            PCookingPot.GetPItem()->GetInterface(&pActionPoints, 0x2d);
            pActionPoints->FreeActionPoint(ActionPointIndex);
        }
    }
}