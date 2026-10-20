// CAIStateGroup_Guildmaster::FindAndStoreBodyguards() -- find the guild bodyguards within 55 units of the guildmaster and register each live one as a helper of his boss component
// Grade: DIFFER(ours v retail) -- the def filter gets its own frame word (retail reuses the string temporary's), and the loop re-reads the vector bounds each pass with the boss in edi (retail caches start/count in ebx/edi and re-reads the boss from its frame word)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00977B10, 298 bytes (`ret`). Ego_r.pdb: ?FindAndStoreBodyguards@CAIStateGroup_Guildmaster@@IAEXXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("CREATURE_GUILDMASTER_BODYGUARD" 0x01285C70) / ~CCharString
// 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0 (`__fastcall`; the string argument of the following call is pushed
// first), CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CThing::GetPosition 0x004C73D0, __ftol2 0x00BFEA70,
// PeekThingSearchTools 0x008FD000, the search tool's rough-area gather 0x008E3EB0 (template instantiation; the manifest
// label names another instantiation), CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0xad), CTCBoss::
// SetHelperCreature 0x007AF310, free 0x00BFEA14 (the vector buffer).
// Shape: the landed AI state-group model; a zeroed vector<CThing*> local, the def-index filter, a C2DCoordI centre (X
// then Y truncation) and a C2DBoxI temporary of +-55; the boss pointer's home is the dead filter word.
// Leaf: FindAndStoreBodyguards@CAIStateGroup_Guildmaster
struct C3DVector { float X, Y, Z; };
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
    const struct C3DVector& GetPosition() const;   // 0x004C73D0
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

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();            // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};


struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {}
};

struct CThingFilter_HasDefGlobalIndex {
    long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(long index) : DefGlobalIndex(index) {}
};

struct CThingVector {                           // STLport vector<CThing*> {start, finish, end_of_storage}
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    CThing* operator[](unsigned long i) const { return _M_start[i]; }
};

struct CThingSearchTools {
    void GetThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CThingVector& out);   // 0x008E3EB0
};

struct CTCBoss {                                // the interface-0xad component
    void SetHelperCreature(CThing* creature);   // 0x007AF310
};

struct CAIStateGroup_Guildmaster : CAIStateGroupBase {
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000 (CAIStateGroupBase)
    void FindAndStoreBodyguards();
};

void CAIStateGroup_Guildmaster::FindAndStoreBodyguards()
{
    CThingVector bodyguards;
    long bodyguardDef = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_GUILDMASTER_BODYGUARD");
    CThingFilter_HasDefGlobalIndex filter(bodyguardDef);
    const C3DVector& pos = PBrain->PCreature->GetPosition();
    C2DCoordI centre((long)pos.X, (long)pos.Y);
    PeekThingSearchTools()->GetThingsInRoughArea(C2DBoxI(centre.X - 55, centre.Y - 55, centre.X + 55, centre.Y + 55), filter, bodyguards);
    CTCBoss* boss;
    PBrain->PCreature->GetInterface(&boss, 0xad);
    for (unsigned long i = 0; i < bodyguards.size(); ++i) {
        CThing* guard = bodyguards[i];
        if (guard && !(guard->m_flags91 & 1))
            boss->SetHelperCreature(guard);
    }
}
