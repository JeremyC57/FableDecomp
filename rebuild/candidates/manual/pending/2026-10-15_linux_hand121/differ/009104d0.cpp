// CAIStateGroup_MinionSitOnSeatBase::GetSeat(CThing* owner) -- collect the creatures that pass the owner-bound filter
// Grade: DIFFER(158v152) -- register roles rotated: retail zeroes esi for the vector words and keeps the count in edi and `first` in ebp (`mov edi,[esp+14h] ; mov ebp,[esp+10h]`), ours zeroes edi, counts in esi and loads `first` through eax (`mov ebp,eax`, +2 bytes); the rest (filter in the dead argument slot, seed step, free behind the null test) lines up.
// into a temporary vector and make a random one the seat.
// Leaf: ?GetSeat@CAIStateGroup_MinionSitOnSeatBase@@MAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009104D0, length 150 bytes (+ int3 padding), `ret 4`. Vtable slot 18.
// Callees: CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetAllLeafThings<CThingAICreature,
// vector, filter>(const filter&, vector&) @ 0x00910A90 (vector pushed first: right to left), the second world
// accessor @ 0x008FCFA0 and CWorld::GetPRandomSeed @ 0x0049D810 (manifest "CVertexBufferWin32::DoSizeof" is
// propagated), GFROR13 @ 0x00497890, the seat setter @ 0x009106E0 (`ret 4`, landed as "AssignTarget": a
// CIntelligentPointer assignment helper), free @ 0x00BFEA14 (`add esp,4`) from the vector's destructor.
// Shape: the one-pointer filter built from `owner` is homed in the dead ARGUMENT slot (`mov [esp+28h],eax`); the
// STLport vector is three words zeroed from one `xor esi,esi` (`_Vector_base` + `_STLP_alloc_proxy`, landed
// 0x007CC7D0 model); `count = (last - first) >> 2`, `if (count) { seed step; SetSeat(*(first + seed % count)); }`,
// then `~_Vector_base` frees the storage behind a null test (the zero register reused for the compare).
// Layout local (the engine header flattens the base and has no vtable).
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

extern "C" void free(void* block);              // 0x00BFEA14

struct SAllocator { SAllocator() {} };          // empty allocator (user constructor: no zero fill of the temporary)

template<class T> struct SAllocProxy : SAllocator { // STLport _STLP_alloc_proxy: the end-of-storage pointer over the allocator
    T* m_data;
    SAllocProxy(const SAllocator& a, T* data) : SAllocator(a), m_data(data) {}
};

template<class T> struct CVectorBase {              // STLport _Vector_base
    T*             m_pFirst;
    T*             m_pLast;
    SAllocProxy<T> m_end;
    CVectorBase(const SAllocator& a) : m_pFirst(0), m_pLast(0), m_end(a, 0) {}
    ~CVectorBase()
    {
        if (m_pFirst != 0)
            free(m_pFirst);
    }
};

template<class T> struct CRetailVector : CVectorBase<T> {
    CRetailVector() : CVectorBase<T>(SAllocator()) {}
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    T& operator[](unsigned long n) { return *(m_pFirst + n); }
};

struct CThingFilter_OwnedBy {                   // the one-pointer filter bound to the owner
    CThing* m_pOwner;
    CThingFilter_OwnedBy(CThing* owner) : m_pOwner(owner) { }
};

struct CThingSearchTools {
    void GetAllLeafThings(const CThingFilter_OwnedBy& filter, CRetailVector<CThing*>& out);    // 0x00910A90
};

struct CWorld2 {
    unsigned long* GetPRandomSeed();            // 0x0049D810
};

struct CAIStateGroupBaseS : CAIStateGroupBase {
    CWorld2* GetPThingWorld();                  // 0x008FCFA0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    void SetSeat(CThing* seat);                 // 0x009106E0
};

struct CAIStateGroup_MinionSitOnSeatBase : CAIStateGroupBaseS {
    unsigned char _seat[0x20];                  // +0x14 CAIStateGroup_UsesSeat members
    CBaseIntelligentPointer POwner;             // +0x34
    virtual bool IsTimeToSit();                 // slot 14 (+0x38)
    virtual void v15();                         // slot 15
    virtual bool GetPOwnerObject(CThing** ppOwner);     // slot 16 (+0x40)
    virtual bool UsesGatherPoint();             // slot 17 (+0x44)
    virtual void GetSeat(CThing* owner);        // slot 18 (+0x48)
};

void CAIStateGroup_MinionSitOnSeatBase::GetSeat(CThing* owner)
{
    CRetailVector<CThing*> candidates;
    PeekThingSearchTools()->GetAllLeafThings(CThingFilter_OwnedBy(owner), candidates);
    CThing** first = candidates.m_pFirst;        // named pointer local: first in ebp
    unsigned long count = (unsigned long)(candidates.m_pLast - first);
    if (count != 0) {
        unsigned long* seed = GetPThingWorld()->GetPRandomSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        SetSeat(*(first + rotated % count));
    }
}
