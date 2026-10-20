// CAIStateGroup_BanditSitRoundCampfire::InitCreaturePosition() -- place the bandit next to a random live bandit already sitting at its campfire
// Grade: DIFFER(ours v retail) -- retail keeps the copied list's sentinel in esi from the copy to the final free (across six calls, incl. clear()); ours reloads it into ecx and re-reads it for the dtor
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093AA40, 298 bytes (`ret`).
// Callees: IsRunnable (vtable slot 11, +0x2c), the const CVectorMap::LowerBound 0x004365B0 (inline GetTC 0x23 after the
// inline HasTC bit test), CTCVillageMember::GetCampfire 0x007EE540 (twice; manifest "IS_THING_ALIVE<..>" is propagated),
// list<CIntelligentPointer<CThing> > copy ctor 0x00618DA0 / size 0x00615220 / clear 0x004B8050, CAIStateGroupBase::
// GetPWorldSeed 0x008FCFD0, GFRandom(range, seed) 0x004CDAE0 (`__fastcall`), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CThing::GetPosition 0x004C73D0, CThingCreatureBase::AutoPlaceNear(const C3DVector&, float) 0x00665DF0,
// free 0x00BFEA14 (the list sentinel).
// Shape: the landed AI state-group model; a copy of the campfire's bandit list, size() inlined once (the count loop)
// and called out of line for GFRandom's range (VC7.1's inline-once band), the random node reached by a signed
// `for (n > 0; --n)` walk, IS_THING_ALIVE then AutoPlaceNear(position, 6.0f), the list dtor (clear + free) on both exits.
// Leaf: InitCreaturePosition@CAIStateGroup_BanditSitRoundCampfire
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
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
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
};


extern "C" void __cdecl free(void* p);          // 0x00BFEA14

struct CIntelligentPointerList {                // STLport list<CIntelligentPointer<CThing> >: the sentinel node
    struct SNode { SNode* _M_next; SNode* _M_prev; CBaseIntelligentPointer _M_data; };
    SNode* _M_node;
    CIntelligentPointerList(const CIntelligentPointerList& other);   // 0x00618DA0
    void clear();                               // 0x004B8050
    ~CIntelligentPointerList() { SNode* node = _M_node; clear(); free(node); }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
};

struct C3DVector { float X, Y, Z; };
struct CThingPos {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCreatureBase {
    bool AutoPlaceNear(const C3DVector& pos, float radius);   // 0x00665DF0
};

struct CCampfire {
    unsigned char _pad[0x44];
    CIntelligentPointerList Bandits;            // +0x44
};
struct CTCVillageMember {
    CCampfire* GetCampfire();                   // 0x007EE540
};

struct CConstInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0 (const)
};
struct CThingC {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CConstInterfaceMap m_interfaces;            // +0x44
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    CTCBase* GetTC(long type) const
    {
        long key = type;
        const CConstInterfaceMap* map = &m_interfaces;
        const CConstInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CConstInterfaceMap::SEntry* sel;
        if (it == map->last)
            sel = map->last;
        else if (it->type <= type)
            sel = it;
        else
            sel = map->last;
        return sel->component;
    }
};

unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);  // 0x004CDAE0

struct CAIStateGroupSeed : CAIStateGroupBase {
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
};

struct CAIStateGroup_BanditSitRoundCampfire : CAIStateGroupSeed {
    bool InitCreaturePosition();
};

bool CAIStateGroup_BanditSitRoundCampfire::InitCreaturePosition()
{
    if (IsRunnable() && ((CThingC*)PBrain->PCreature)->HasTC(0x23)) {
        CTCVillageMember* member = (CTCVillageMember*)((CThingC*)PBrain->PCreature)->GetTC(0x23);
        if (member->GetCampfire()) {
            CIntelligentPointerList bandits(member->GetCampfire()->Bandits);
            if (bandits.size()) {
                CIntelligentPointerList::SNode* node = bandits._M_node->_M_next;
                for (long n = GFRandom(bandits.size(), GetPWorldSeed()); n > 0; --n)
                    node = node->_M_next;
                if (IS_THING_ALIVE(node->_M_data)) {
                    CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
                    bool placed = creature->AutoPlaceNear(((CThingPos*)node->_M_data.GetPItem())->GetPosition(), 6.0f);
                    return placed;
                }
            }
        }
    }
    return false;
}
