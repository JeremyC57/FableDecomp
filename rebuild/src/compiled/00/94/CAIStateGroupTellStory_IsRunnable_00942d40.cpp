// CAIStateGroup_TellStory::IsRunnable() -- a villager with a workplace (and a village) tells a story when the workplace's material is a story building (def flag bit 2), the workplace's own village says it is cook time, and a "STORY_PLACE_MARKER" thing exists (nearest one searched)
// Leaf: ?IsRunnable@CAIStateGroup_TellStory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00942D40, length 416 bytes (plain `ret`, virtual slot 11).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the creature's +0x170 workplace pointer), CVectorMap::LowerBound
//          @ 0x004365B0 (const, the creature) / @ 0x0040F020 (the workplace) for the inlined interface-0x23 (CTCVillager)
//          lookups, GFGetPVillageInterface @ 0x007EE540 (x3; manifest "IS_THING_ALIVE<...>" is wrong),
//          CThingBuilding::PeekPMaterial @ 0x0082D2D0 (CDefPointer by hidden pointer, inline release through slot 1),
//          CTCVillage::IsCookTime @ 0x00658A80, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
//          CGameDefinitionManager::Get @ 0x0044C6B0 (static __fastcall), CDefinitionManager::GetDefGlobalIndexFromName
//          @ 0x009AD410, CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
//          CThingSearchTools::GetPBestThing<CThingFilter_HasDefGlobalIndex, CThingCompare_Nearest, CThing> @ 0x009107B0.
// Model: the landed sibling CAIStateGroup_WorkAtSchool::IsRunnable (0x00942A00) shared AI state-group model (the
//          generated engine/CAIStateGroup_TellStory.h flattens the vptr and has no members used here).
// Layout: the creature's PWorkplace intelligent pointer at +0x170 (as the landed TavernKeeper 0x00968E00 / WorkAtSchool);
//          retail CThing sits 0x10 below the PDB (presence bitset +0x20, vector map +0x44).
// Shape: one nested `if` chain with a single `return false`; the name is a named CCharString in its own block (its slot
//          is reused by the filter); the search template gets a same-TU noinline stand-in that reads its arguments
//          without letting their addresses escape -- with an external declaration VC stores the comparator's x/y
//          right after each load instead of retail's load-all-three-then-interleave-with-pushes.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (this TU's non-const copy)
    SEntry* LowerBound(const long* type) const; // 0x004365B0 (the const copy: the first lookup goes through a const map)
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;                             // +0x60 (typed per use below)
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char m_flags92;                    // +0x92
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    bool TestTC(long interfaceType) const       // std::bitset::test: `shr ; and cl,1` (kept as a byte when reused)
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
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
    __forceinline CInterfaceMap::SEntry* FindEntryC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
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
    // the bitset::test form: the test's bool is materialised (`shr ; and dl,1 ; mov [esp+x],dl`) and returned
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
    // the ternary form (landed 0x00954690): the end/hit choice is ONE ternary, so the component lands straight in
    // the register of its use (ecx for a call on it); the miss path reads the never-written slot
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    // the KidCleanup form (landed 0x00915190): `if (mask) *pp = Lookup();` with an entry-value lookup
    __forceinline CTCBase* LookupValue(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap::SEntry* p = m_interfaces.LowerBound(&type);
        if (p == m_interfaces.last) p = m_interfaces.last;
        else if (p->type > interfaceType) p = m_interfaces.last;
        return p->component;
    }
    template<class T> __forceinline void GetInterfaceK(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            *ppOut = (T*)LookupValue(interfaceType);
    }
    // GetInterface<T>() returning the component: hit = the entry's component, miss = the never-written local
    template<class T> __forceinline T* PeekInterface(long interfaceType)
    {
        T* component;
        if (HasTC(interfaceType))
            component = (T*)FindEntry(interfaceType)->component;
        return component;
    }
    // the out-parameter form whose result is ignored (no bool to keep: the component lands in its use register)
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
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

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

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
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C3DVector { float x, y, z; };
struct C2DVector {
    float x, y;
    float GetLengthSquared() const { return x * x + y * y; }
};
struct CRefCounted {                            // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CMaterialDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    CRefCounted Ref;                            // +0x04
    unsigned char _pad08[0x3c - 0x08];
    unsigned long Flags;                        // +0x3c (bit 2: a school building)
    __forceinline void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T>
struct CDefPointer {
    T* Object;
    __forceinline ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};
struct CThingBuilding : CThing {
    CDefPointer<CMaterialDef> PeekPMaterial();  // 0x0082D2D0
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CThingPos : CThing {
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CTCVillager;
struct CTCVillage;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);  // 0x007EE540
struct CThingVillagerBase : CThing {
    unsigned char _pad94[0x170 - 0x94];
    CBaseIntelligentPointer PWorkplace;         // +0x170
};
struct CTCVillage {
    bool IsCookTime();                          // 0x00658A80
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();                 // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);        // 0x009AD410
};
struct CThingFilter_HasDefGlobalIndex {
    long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(long index) : DefGlobalIndex(index) {}
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}
};
struct CThingSearchTools {
    bool GetPBestThing(const CThingFilter_HasDefGlobalIndex& filter, const CThingCompare_Nearest& compare, CThing** out);   // 0x009107B0
};
long BestSink_9107B0(const void* self, long index, float x);
#pragma auto_inline(off)
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const CThingFilter_HasDefGlobalIndex& filter, const CThingCompare_Nearest& compare, CThing** out)
{
    long best = BestSink_9107B0(this, filter.DefGlobalIndex, compare.Pos.x);
    *out = (CThing*)best;
    return best != 0;
}
#pragma auto_inline(on)
struct CAIStateGroup_TellStory : CAIStateGroupBase {
    CThingVillagerBase* GetCreature() const { return (CThingVillagerBase*)PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    bool IsRunnable();
    virtual void Update();
};
bool CAIStateGroup_TellStory::IsRunnable()
{
    CThingBuilding* home = (CThingBuilding*)GetCreature()->PWorkplace.GetPItem();
    if (home && GetCreature()->HasTC(0x23)) {
        CTCVillager* villager = (CTCVillager*)GetCreature()->FindEntryC(0x23)->component;
        if (GFGetPVillageInterface(villager)) {
            CDefPointer<CMaterialDef> material = home->PeekPMaterial();
            if (material.Object) {
                if ((material->Flags & 4) && home->HasTC(0x23)) {
                    CTCVillager* homeVillager = (CTCVillager*)home->FindEntry(0x23)->component;
                    if (GFGetPVillageInterface(homeVillager) && GFGetPVillageInterface(homeVillager)->IsCookTime()) {
                        long index;
                        {
                            CCharString name("STORY_PLACE_MARKER");
                            index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
                        }
                        CThing* marker;
                        CThingCompare_Nearest compare(((CThingPos*)PBrain->PCreature)->GetPosition());
                        CThingFilter_HasDefGlobalIndex filter(index);
                        if (PeekThingSearchTools()->GetPBestThing(filter, compare, &marker))
                            return true;
                    }
                }
            }
        }
    }
    return false;
}