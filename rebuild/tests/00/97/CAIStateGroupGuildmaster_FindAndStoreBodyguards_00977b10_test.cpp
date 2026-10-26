// Behaviour test for 00977b10: src/00977b10.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAIStateGroup_Guildmaster::FindAndStoreBodyguards()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_Guildmaster::FindAndStoreBodyguards() -- find the guild bodyguards within 55 units of the guildmaster and register each live one as a helper of his boss component
// Retail: Fable.exe (TLC, VC7.1) @ 0x00977B10, 298 bytes (`ret`). Ego_r.pdb: ?FindAndStoreBodyguards@CAIStateGroup_Guildmaster@@IAEXXZ.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("CREATURE_GUILDMASTER_BODYGUARD" 0x01285C70) / ~CCharString
// 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0 (`__fastcall`; the string argument of the following call is pushed
// first), CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CThing::GetPosition 0x004C73D0, __ftol2 0x00BFEA70,
// CAIStateGroupBase::PeekThingSearchTools() const 0x008FD000, CThingSearchTools::GetAllThingsInRoughArea<CThingAICreature,
// vector<CThingAICreature*>, CThingFilter_HasDefGlobalIndex>(const C2DBoxI&, const filter&, vector&) const 0x008E3EB0
// (Ego_r.pdb names this instantiation; its body tests the creature type bits and calls the filter 0x006A70B0),
// CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0xad), CTCBoss::SetHelperCreature(CThingAICreature&)
// 0x007AF310, free 0x00BFEA14 (the vector buffer).
// Shape: the landed AI state-group model; a zeroed vector<CThingAICreature*> local, then an inner block holding the
// def-index filter, the C2DCoordI centre (X then Y truncation) and the search over a C2DBoxI temporary of +-55. The
// filter escapes into the search (its address reaches the filter call), so only the block lets the boss's out-pointer
// and the GetInterface key temporary reuse its word (which is the string temporary's word too). The search is a
// template instantiation in retail's TU, so VC7.1 sees that the vector's address does not escape: the stand-in below
// hands the extern only the bounds and writes them back, which keeps the loop's start/count in ebx/edi across the
// SetHelperCreature calls while the boss is re-read from its frame word.
// Leaf: FindAndStoreBodyguards@CAIStateGroup_Guildmaster
class C3DVector { public: float X, Y, Z; };
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
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
    const class C3DVector& GetPosition() const;   // 0x004C73D0
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
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

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();            // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};


class C2DCoordI { public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
class C2DBoxI { public:
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {}
};

class CThingFilter_HasDefGlobalIndex { public:
    long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(long index) : DefGlobalIndex(index) {}
};

class CThingAICreature : public CThing { public:
};

struct CCreatureVector {                        // STLport vector<CThingAICreature*> {start, finish, end_of_storage}
    CThingAICreature** _M_start;
    CThingAICreature** _M_finish;
    CThingAICreature** _M_end_of_storage;
    CCreatureVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CCreatureVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    CThingAICreature* operator[](unsigned long i) const { return _M_start[i]; }
};

class CThingSearchTools { public:
    // GetAllThingsInRoughArea<CThingAICreature, vector<CThingAICreature*>, CThingFilter_HasDefGlobalIndex> @ 0x008E3EB0
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CCreatureVector& out) const;
};
// Stand-in for the 0x008E3EB0 instantiation: same TU, the vector's bounds pass by value and come back through locals.
void GatherRoughArea_008e3eb0(const CThingSearchTools* tools, long minX, long minY, long maxX, long maxY, const CThingFilter_HasDefGlobalIndex& filter,
                              CThingAICreature*** start, CThingAICreature*** finish, CThingAICreature*** endOfStorage);
#pragma auto_inline(off)
__declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CCreatureVector& out) const
{
    CThingAICreature** start = out._M_start;
    CThingAICreature** finish = out._M_finish;
    CThingAICreature** endOfStorage = out._M_end_of_storage;
    GatherRoughArea_008e3eb0(this, area.MinX, area.MinY, area.MaxX, area.MaxY, filter, &start, &finish, &endOfStorage);
    out._M_start = start;
    out._M_finish = finish;
    out._M_end_of_storage = endOfStorage;
}
#pragma auto_inline(on)

class CTCBoss { public:                                // the interface-0xad component
    void SetHelperCreature(CThingAICreature& creature);   // 0x007AF310
};

struct CAIStateGroup_Guildmaster : CAIStateGroupBase {
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000 (CAIStateGroupBase)
    void FindAndStoreBodyguards();
};

void CAIStateGroup_Guildmaster::FindAndStoreBodyguards()
{
    CCreatureVector bodyguards;
    long bodyguardDef = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_GUILDMASTER_BODYGUARD");
    {
        CThingFilter_HasDefGlobalIndex filter(bodyguardDef);
        const C3DVector& pos = PBrain->PCreature->GetPosition();
        C2DCoordI centre((long)pos.X, (long)pos.Y);
        PeekThingSearchTools().GetAllThingsInRoughArea(C2DBoxI(centre.X - 55, centre.Y - 55, centre.X + 55, centre.Y + 55), filter, bodyguards);
    }
    CTCBoss* boss;
    PBrain->PCreature->GetInterface(&boss, 0xad);
    for (unsigned long i = 0; i < bodyguards.size(); ++i) {
        CThingAICreature* guard = bodyguards[i];
        if (guard && !(guard->m_flags91 & 1))
            boss->SetHelperCreature(*guard);
    }
}

// ---- stubs and driver ----
#include <cstdlib>
static int g_strings = 0, g_lookups = 0, g_gathers = 0, g_peeks = 0, g_helpers = 0, g_positions = 0;
static CThingAICreature* g_helper[8]; static CTCBoss* g_helperBoss[8];
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; ++g_strings; CHECK(length == -1); }
CCharString::~CCharString() { --g_strings; }
static CGameDefinitionManager g_defs;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_defs; }
long CGameDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name)
{
    ++g_lookups; CHECK(this == &g_defs);
    return strcmp((const char*)name.m_pData, "CREATURE_GUILDMASTER_BODYGUARD") == 0 ? 42 : -1;
}
static C3DVector g_pos = { 10.7f, -20.2f, 3.0f };
const C3DVector& CThing::GetPosition() const { ++g_positions; return g_pos; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
static CThingSearchTools g_tools;
const CThingSearchTools& CAIStateGroup_Guildmaster::PeekThingSearchTools() const { ++g_peeks; return g_tools; }
static long g_box[4]; static long g_filterIndex = 0; static int g_resultCount = 0;
static CThingAICreature* g_results[4];
void GatherRoughArea_008e3eb0(const CThingSearchTools* tools, long minX, long minY, long maxX, long maxY, const CThingFilter_HasDefGlobalIndex& filter,
                              CThingAICreature*** start, CThingAICreature*** finish, CThingAICreature*** endOfStorage)
{
    ++g_gathers; CHECK(tools == &g_tools);
    g_box[0] = minX; g_box[1] = minY; g_box[2] = maxX; g_box[3] = maxY; g_filterIndex = filter.DefGlobalIndex;
    CHECK(*start == 0 && *finish == 0 && *endOfStorage == 0);
    if (g_resultCount == 0) return;
    CThingAICreature** buf = (CThingAICreature**)malloc(sizeof(CThingAICreature*) * 8);
    for (int i = 0; i < g_resultCount; ++i) buf[i] = g_results[i];
    *start = buf; *finish = buf + g_resultCount; *endOfStorage = buf + 8;
}
void CTCBoss::SetHelperCreature(CThingAICreature& creature) { g_helperBoss[g_helpers] = this; g_helper[g_helpers++] = &creature; }
int main()
{
    static unsigned char sgRaw[sizeof(CAIStateGroup_Guildmaster)];
    CAIStateGroup_Guildmaster* sg = (CAIStateGroup_Guildmaster*)sgRaw;
    static CAIBrain brain; static CThing master;
    static CThingAICreature guards[3];
    CTCBoss* bossComponent = (CTCBoss*)0x5150;
    CInterfaceMap::SEntry entries[2] = { { 0x40, (CTCBase*)0x1111 }, { 0xad, (CTCBase*)bossComponent } };
    master.m_interfaces.first = entries; master.m_interfaces.last = entries + 2;
    master.m_presentBits[0xad >> 5] |= 1UL << (0xad & 31);
    brain.PCreature = &master; sg->PBrain = &brain;
    guards[1].m_flags91 = 1;   // dead
    g_results[0] = &guards[0]; g_results[1] = 0; g_results[2] = &guards[1]; g_results[3] = &guards[2]; g_resultCount = 4;
    sg->FindAndStoreBodyguards();
    CHECK(g_strings == 0 && g_lookups == 1 && g_positions == 1 && g_peeks == 1 && g_gathers == 1);
    CHECK(g_filterIndex == 42);
    CHECK(g_box[0] == 10 - 55 && g_box[1] == -20 - 55 && g_box[2] == 10 + 55 && g_box[3] == -20 + 55);
    CHECK(g_helpers == 2 && g_helper[0] == &guards[0] && g_helper[1] == &guards[2]);
    CHECK(g_helperBoss[0] == bossComponent && g_helperBoss[1] == bossComponent);
    // nobody in range: no helpers, nothing to free
    g_resultCount = 0;
    sg->FindAndStoreBodyguards();
    CHECK(g_strings == 0 && g_lookups == 2 && g_gathers == 2 && g_helpers == 2);
    if (fails == 0) printf("00977b10_TEST PASS\n");
    return fails;
}
