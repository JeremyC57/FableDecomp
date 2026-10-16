// Grade: DIFFER(163v163) -- register swap only: retail holds the list node in edi and &node->value in esi, ours the reverse | tried: raw node for-loop, while loop, STL-style iterator struct
// CAIStateGroup_MinionReactToCrime::IsGuardOnLevel() -- true when the creature's village lists a live villager that
// has the guard interface (0xB4).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00958C00, length 165 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of the bitset
// dword at +0x24), GFGetPVillageInterface @ 0x007EE540 (twice), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (three times per element).
// Shape: the village's std::list<CIntelligentPointer> at +0x2c walked with begin()/end() (end re-read every pass:
// `cmp edi,[ebp]`), IS_THING_ALIVE inlined, then the inline HasTC(0xB4) (bit 20 of the dword at +0x34).
// Engine header names for the state group; the header flattens the base, so the class is modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
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

inline bool IS_THING_ALIVE(const CBaseIntelligentPointerCtor& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CThingPointerList {                      // std::list<CIntelligentPointer<CThing>>
    struct SNode {
        SNode* next;                            // +0x00
        SNode* prev;                            // +0x04
        CBaseIntelligentPointerCtor value;      // +0x08
    };
    struct iterator {
        SNode* _Ptr;
        iterator(SNode* p) : _Ptr(p) { }
        CBaseIntelligentPointerCtor& operator*() const { return _Ptr->value; }
        iterator& operator++() { _Ptr = _Ptr->next; return *this; }
        bool operator!=(const iterator& o) const { return _Ptr != o._Ptr; }
    };
    SNode* head;                                // sentinel
    iterator begin() const { return iterator(head->next); }
    iterator end() const { return iterator(head); }
};

struct CTCVillager;
struct CTCVillage {
    unsigned char _pad[0x2c];
    CThingPointerList Villagers;                // +0x2c
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CAIStateGroup_MinionReactToCrime : CAIStateGroupBase {
    long OpinionDeedHandle;                     // +0x14
    bool GuardTold;                             // +0x18
    bool AllowDuplicateReactToCrime;            // +0x19
    bool DeedMaskApplied;                       // +0x1a
    bool IsGuardOnLevel();
};

bool CAIStateGroup_MinionReactToCrime::IsGuardOnLevel()
{
    CTCVillager* villager;
    if (PBrain->PCreature->GetInterface(&villager, 0x23)) {
        if (GFGetPVillageInterface(villager) != 0) {
            CThingPointerList& list = GFGetPVillageInterface(villager)->Villagers;
            for (CThingPointerList::iterator it = list.begin(); it != list.end(); ++it) {
                if (IS_THING_ALIVE(*it) && (*it).GetPItem()->HasTC(0xb4))
                    return true;
            }
        }
    }
    return false;
}
