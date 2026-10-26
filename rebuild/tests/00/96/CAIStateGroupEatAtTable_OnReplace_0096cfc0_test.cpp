// Behaviour test for 0096cfc0: src/0096cfc0.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CAIStateGroup_EatAtTable::OnReplace
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_EatAtTable::OnReplace() -- drop modes 0x28, free the table seat (getting up when seated), drop/replace the carried tankard, then drop the sitting mode unless the get-up took it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096CFC0, length 463 bytes.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interfaces 0x31 / 0xb8 / 0x46 / 0xba), CTCCreatureModeManager::RemoveMode
//          @ 0x0073ABA0, GetPItem @ 0x00A01B50, seat-table IsSeatTaken @ 0x007B8570 (manifest "CTCInventoryBase::
//          GetSelectedCategory" is propagated: a byte-vector at +0x34), FreeSeat @ 0x007B8AA0, SetOccupant @ 0x007B85F0
//          (manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//          GetUpFromSitting @ 0x0085FF10 (__fastcall), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440 /
//          RemoveThingFromPrimarySlot @ 0x006A1E00, CTCTavernTankard::ReplaceTankard @ 0x00947C30, CThing::Kill @ 0x004C9B80.
// Layout: PDB CAIStateGroup_EatAtTable: Table +0x14 (CIntelligentPointer<CThing>), SeatIndex +0x1c, SeatActionPointIndex +0x20;
//          no engine header, modelled locally.
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void Kill(bool b);                          // 0x004C9B80
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
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
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
bool __fastcall GetUpFromSitting(CThing* creature, long movement, const CCharString* anim, CAIBrain* brain);   // 0x0085FF10
class CTCCreatureModeManager { public:                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
class CTCCarrying { public:                            // interface 0x46
    CThing* GetThingInPrimarySlot();            // 0x006A1440
    void RemoveThingFromPrimarySlot(bool b);    // 0x006A1E00
};
class CTCTavernTankard { public:                       // interface 0xba
    void ReplaceTankard();                      // 0x00947C30
};
struct CTCSeatTable {                           // interface 0xb8
    unsigned char _pad[0x10];
    CThing** SeatUsers;                         // +0x10
    bool IsSeatTaken(long seat);                // 0x007B8570
    void FreeSeat(long seat, long actionPoint); // 0x007B8AA0
    void SetOccupant(long seat, CThing* occupant);   // 0x007B85F0
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_EatAtTable : CAIStateGroupBase {
    CBaseIntelligentPointer Table;              // +0x14
    long SeatIndex;                             // +0x1c
    long SeatActionPointIndex;                  // +0x20
    virtual void OnReplace();
};

void CAIStateGroup_EatAtTable::OnReplace()
{
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x28);
    bool removeSitMode = true;
    if (Table.GetPItem()) {
        CTCSeatTable* table;
        if (!Table.GetPItem()->GetInterface(&table, 0xb8))
            return;
        if (table->IsSeatTaken(SeatIndex)) {
            bool gotUp;
            {
                CCharString anim("SIT_AT_TABLE_OUTOF");
                gotUp = GetUpFromSitting(GetCreature(), 11, &anim, PBrain);
            }
            if (gotUp)
                removeSitMode = false;
        }
        table->FreeSeat(SeatIndex, SeatActionPointIndex);
        table->SeatUsers[SeatIndex] = 0;
        CTCCarrying* carrying;
        GetCreature()->GetInterface(&carrying, 0x46);
        CThing* item = carrying->GetThingInPrimarySlot();
        if (item && !(item->m_flags91 & 1)) {
            carrying->RemoveThingFromPrimarySlot(false);
            if (item->HasTC(0xba))
                item->GetTC<CTCTavernTankard>(0xba)->ReplaceTankard();
            else {
                item->Kill(true);
                table->SetOccupant(SeatIndex, 0);
            }
        }
    }
    if (removeSitMode)
        modes->RemoveMode(11);
}

// ---- stubs ----
void CBaseIntelligentPointer::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CAIStateGroupBase::~CAIStateGroupBase() {}
static int g_kills; static CThing* g_killed; static bool g_killFlag;
void CThing::Kill(bool b) { ++g_kills; g_killed = this; g_killFlag = b; }
static int g_strs; static const char* g_strText;
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; ++g_strs; g_strText = text; }
CCharString::~CCharString() { --g_strs; }
static int g_getUp; static CThing* g_getUpWho; static long g_getUpMove; static const char* g_getUpAnim; static CAIBrain* g_getUpBrain; static bool g_getUpRet;
static int g_getUpLive;
bool __fastcall GetUpFromSitting(CThing* creature, long movement, const CCharString* anim, CAIBrain* brain)
{ ++g_getUp; g_getUpWho = creature; g_getUpMove = movement; g_getUpAnim = (const char*)anim->m_pData; g_getUpBrain = brain; g_getUpLive = g_strs; return g_getUpRet; }
static unsigned long g_removed[8]; static int g_nRemoved; static CTCCreatureModeManager* g_modesThis;
void CTCCreatureModeManager::RemoveMode(unsigned long mode) { g_removed[g_nRemoved++ & 7] = mode; g_modesThis = this; }
static CThing* g_primary; static int g_removePrimary; static bool g_removePrimaryFlag;
CThing* CTCCarrying::GetThingInPrimarySlot() { return g_primary; }
void CTCCarrying::RemoveThingFromPrimarySlot(bool b) { ++g_removePrimary; g_removePrimaryFlag = b; }
static int g_replaced; static CTCTavernTankard* g_replacedThis;
void CTCTavernTankard::ReplaceTankard() { ++g_replaced; g_replacedThis = this; }
static bool g_taken; static long g_takenSeat;
bool CTCSeatTable::IsSeatTaken(long seat) { g_takenSeat = seat; return g_taken; }
static int g_freed; static long g_freeSeat, g_freeAp; static int g_freeLive;
void CTCSeatTable::FreeSeat(long seat, long actionPoint) { ++g_freed; g_freeSeat = seat; g_freeAp = actionPoint; g_freeLive = g_strs; }
static int g_occ; static long g_occSeat; static CThing* g_occWho;
void CTCSeatTable::SetOccupant(long seat, CThing* occupant) { ++g_occ; g_occSeat = seat; g_occWho = occupant; }
static void Reset()
{
    g_kills = 0; g_killed = 0; g_killFlag = false; g_strs = 0; g_strText = 0; g_getUp = 0; g_getUpWho = 0; g_getUpMove = 0; g_getUpAnim = 0;
    g_getUpBrain = 0; g_getUpLive = 0; memset(g_removed, 0, sizeof g_removed); g_nRemoved = 0; g_modesThis = 0; g_removePrimary = 0;
    g_removePrimaryFlag = true; g_replaced = 0; g_replacedThis = 0; g_takenSeat = -1; g_freed = 0; g_freeSeat = g_freeAp = -1; g_freeLive = -1;
    g_occ = 0; g_occSeat = -1; g_occWho = (CThing*)1;
}
static void SetInterfaces(CThing* t, CInterfaceMap::SEntry* e, int n)
{
    memset(t->m_presentBits, 0, sizeof t->m_presentBits);
    for (int i = 0; i < n; ++i) t->m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
    t->m_interfaces.first = e; t->m_interfaces.last = e + n;
}
int main()
{
    static CThing creature, tableThing, item; static CTCCreatureModeManager modes; static CTCCarrying carrying; static CTCSeatTable table;
    static CTCTavernTankard tankard; static CThing* seatUsers[4];
    CInterfaceMap::SEntry ce[] = { { 3, 0 }, { 0x31, (CTCBase*)&modes }, { 0x46, (CTCBase*)&carrying } }; SetInterfaces(&creature, ce, 3);
    CInterfaceMap::SEntry te[] = { { 0xb8, (CTCBase*)&table } }; SetInterfaces(&tableThing, te, 1);
    CInterfaceMap::SEntry ie[] = { { 0x10, 0 }, { 0xba, (CTCBase*)&tankard } }; SetInterfaces(&item, ie, 2);
    table.SeatUsers = seatUsers;
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_EatAtTable g; g.PBrain = &brain; g.Table.m_pItem = &tableThing; g.SeatIndex = 2; g.SeatActionPointIndex = 5;
    // seated, gets up (the get-up takes the sitting mode); carried tankard replaced
    Reset(); seatUsers[2] = &creature; g_taken = true; g_getUpRet = true; g_primary = &item;
    g.OnReplace();
    CHECK(g_nRemoved == 1 && g_removed[0] == 0x28 && g_modesThis == &modes);
    CHECK(g_takenSeat == 2 && g_getUp == 1 && g_getUpWho == &creature && g_getUpMove == 11 && g_getUpBrain == &brain);
    CHECK(g_getUpAnim && !strcmp(g_getUpAnim, "SIT_AT_TABLE_OUTOF") && g_getUpLive == 1 && g_freeLive == 0 && g_strs == 0);
    CHECK(g_freed == 1 && g_freeSeat == 2 && g_freeAp == 5 && seatUsers[2] == 0);
    CHECK(g_removePrimary == 1 && !g_removePrimaryFlag && g_replaced == 1 && g_replacedThis == &tankard && g_kills == 0 && g_occ == 0);
    // seated but the get-up fails: sitting mode removed here; carried non-tankard killed and the seat occupant cleared
    SetInterfaces(&item, ie, 1);
    Reset(); g_getUpRet = false;
    g.OnReplace();
    CHECK(g_nRemoved == 2 && g_removed[0] == 0x28 && g_removed[1] == 11);
    CHECK(g_kills == 1 && g_killed == &item && g_killFlag && g_occ == 1 && g_occSeat == 2 && g_occWho == 0 && g_replaced == 0);
    // not seated: no get-up, sitting mode removed; dead item left alone
    item.m_flags91 = 1;
    Reset(); g_taken = false;
    g.OnReplace();
    CHECK(g_getUp == 0 && g_freed == 1 && g_nRemoved == 2 && g_removed[1] == 11 && g_removePrimary == 0 && g_kills == 0);
    // nothing carried
    item.m_flags91 = 0; Reset(); g_primary = 0;
    g.OnReplace();
    CHECK(g_removePrimary == 0 && g_nRemoved == 2);
    // table without the seat interface: return right after the 0x28 removal
    SetInterfaces(&tableThing, te, 0);
    Reset();
    g.OnReplace();
    CHECK(g_nRemoved == 1 && g_removed[0] == 0x28 && g_freed == 0 && g_takenSeat == -1);
    // no table: only the two mode removals
    g.Table.m_pItem = 0; Reset();
    g.OnReplace();
    CHECK(g_nRemoved == 2 && g_removed[0] == 0x28 && g_removed[1] == 11 && g_freed == 0);
    if (fails == 0) printf("0096cfc0_TEST PASS\n"); else printf("0096cfc0_TEST FAIL\n");
    return fails;
}