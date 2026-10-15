// CAIStateGroup_WanderAroundInitialPos::IsRunnable() -- runnable once the creature strays more than 10 units from its initial position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00911A00, length 137 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (the second instantiation of the interface-map search; inlined
// GetInterface on the brain's creature, type 8 = bit 8 of the presence word at +0x20), the type-8 component's
// bool query @ 0x007F6150 (manifest label "CTCUserActionPointBase::IsActionPointFree" is propagated across a
// byte-identical body; kept descriptive here), CThing::GetPosition @ 0x004C73D0 (landed name).
//
// Shape: `GetInterface(&c, 8) && c->IsEnabled()`, then the creature (re-read into esi) compares its current
// position with the planar initial position stored at +0xe8/+0xec: dx and dy are both left on the x87 stack,
// `d = dx*dx; d += dy*dy;` squares dx first (`fld st(1) ; fmul st(2)` twice; the `dx*dx + dy*dy` form
// squares the top of stack first), `> 100.0f` is the
// `test ah,41h ; jne` false branch. Separate `mov al,1` / `xor al,al` exits. No engine header exists.
struct CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x004365B0 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0 (a second, byte-identical instantiation)
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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

struct CBaseIntelligentPointer {                // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                          // +0x00
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

struct C3DVector {
    float X, Y, Z;
};

struct CTCWanderControl {                       // interface type 8 component (descriptive)
    bool IsEnabled();                           // 0x007F6150
};

struct CThingAICreature : CThing {
    unsigned char _pad2[0xe8 - sizeof(CThing)];
    float InitialPosX;                          // +0xe8 (descriptive: the wander centre)
    float InitialPosY;                          // +0xec
    C3DVector* GetPosition();                   // 0x004C73D0
};

struct CAIBrainCreature {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrainCreature* PBrain;                   // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    bool Terminated;                            // +0x10
    unsigned char Priority;                     // +0x11
    bool Enabled;                               // +0x12
};

struct CAIStateGroup_WanderAroundInitialPos : CAIStateGroupBase {
    bool IsRunnable();
};

bool CAIStateGroup_WanderAroundInitialPos::IsRunnable()
{
    CTCWanderControl* control;
    if (PBrain->PCreature->GetInterface(&control, 8) && control->IsEnabled()) {
        CThingAICreature* creature = PBrain->PCreature;
        C3DVector* position = creature->GetPosition();
        float dx = creature->InitialPosX - position->X;
        float dy = creature->InitialPosY - position->Y;
        float distanceSq = dx * dx;
        distanceSq += dy * dy;
        if (distanceSq > 100.0f)
            return true;
    }
    return false;
}