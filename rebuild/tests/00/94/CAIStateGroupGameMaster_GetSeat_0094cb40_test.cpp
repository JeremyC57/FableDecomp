// Behaviour test for 0094cb40: src/0094cb40.cpp copied verbatim, out-of-line callees stubbed below.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_GameMaster::GetSeat() -- collect the pub's live tavern tables, then try them in order: take the table, and if it gives a game-master seat claim its food as the tankard and register the creature with the table's coin-golf game
// Grade: RELOCATION_MATCH (retry A 11b) -- the STLport list<CThing*> destructor (0x00792240) gets a same-TU auto_inline(off) body, so VC sees the local list's address never escapes and keeps its header node in esi for the whole function (this spilled to [esp+0x1c]); push_back links through STLport insert's n/prev locals; operator= reads the source item into a local before SetPItem
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094CB40, 800 bytes (aligned frame, no EH frame).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (list nodes), ~list 0x00792240, CBaseIntelligentPointer GetPItem 0x00A01B50 /
//   ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<CThingObject> vtable 0x0125A858,
//   CIntelligentPointer<CThing> vtable 0x01238C6C), CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 0x21, 0xB8,
//   0xCF), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCTavernTable::GetSeat 0x007B8840 /
//   GetTankard 0x007B8590, CTCTavernGameCoinGolf's game-master setter 0x0094D430. Literal "ACTION_POINT_GAME_MASTER" @ 0x0129319C.
// Layout: Ego_r.pdb CAIStateGroup_GoToPub base: PPub +0x14, PTable +0x24, PTankard +0x34, SeatIndex +0x3c, Pos +0x40, RHSet
//   +0x4c, ActionPointIndex +0x64; CThing interface bitset +0x20, map +0x44, Dead +0x91 bit 0; table list component
//   (interface 0x21) STLport list node at +0x0c.
#include <stdlib.h>
#include <new>
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroupBase.h"
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: float M[6]; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)(it == map->last || type < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingObject : public CThing { public: };
class CThingAICreature : public CThing { public: };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v01();
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1);
}
template <class T> struct CListNode {
    CListNode* _M_next;
    CListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CStlList {               // STLport list<T> (node from malloc)
    CListNode<T>* _M_node;
    CStlList()
    {
        CListNode<T>* node = (CListNode<T>*)malloc(sizeof(CListNode<T>));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CStlList();                                   // 0x00792240 (body below)
    unsigned long size() const
    {
        unsigned long n = 0;
        for (CListNode<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    T& front() { return _M_node->_M_next->_M_data; }
    void push_back(const T& x)
    {
        CListNode<T>* node = (CListNode<T>*)malloc(sizeof(CListNode<T>));
        new (&node->_M_data) T(x);
        CListNode<T>* n = _M_node;
        CListNode<T>* prev = n->_M_prev;
        node->_M_next = n;
        node->_M_prev = prev;
        prev->_M_next = node;
        n->_M_prev = node;
    }
    void pop_front()
    {
        CListNode<T>* node = _M_node->_M_next;
        CListNode<T>* next = node->_M_next;
        CListNode<T>* prev = node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(node);
    }
};
struct CPointerListNode {
    CPointerListNode* _M_next;
    CPointerListNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};
struct CTCTableList {                           // interface 0x21
    unsigned char _pad00[0x0c];
    CPointerListNode* _M_node;                  // +0x0c STLport list
};
class CTCTavernTable { public:                         // interface 0xB8
    bool GetSeat(CThing& creature, C3DVector& pos, CRightHandedSet& set, long& actionPoint, long& seat,
                 const CCharString* actionPointName);   // 0x007B8840
    CThingObject* GetTankard(long seat);           // 0x007B8590
};
class CTCTavernGameCoinGolf { public:                  // interface 0xCF
    void SetGameMaster(CThing* master);         // 0x0094D430
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    CBaseIntelligentPointer PPub;               // +0x14
    CBaseIntelligentPointer PBar;               // +0x1c
    CIntelligentPointer<CThingObject> PTable;   // +0x24
    CBaseIntelligentPointer PBarman;            // +0x2c
    CIntelligentPointer<CThing> PTankard;       // +0x34
    long SeatIndex;                             // +0x3c
    C3DVector Pos;                              // +0x40
    CRightHandedSet RHSet;                      // +0x4c
    long ActionPointIndex;                      // +0x64
};
struct CAIStateGroup_GameMaster : CAIStateGroup_GoToPub {
    bool GetSeat();
};

// STLport list<CThing*>::~list() @ 0x00792240 (clear the nodes, free the header), compiled in this TU but kept
// out of line as in retail.
#pragma auto_inline(off)
template <> CStlList<CThing*>::~CStlList()
{
    CListNode<CThing*>* node = _M_node->_M_next;
    while (node != _M_node) {
        CListNode<CThing*>* next = node->_M_next;
        free(node);
        node = next;
    }
    free(_M_node);
}
#pragma auto_inline(on)

bool CAIStateGroup_GameMaster::GetSeat()
{
    CStlList<CThing*> tables;
    CTCTableList* tableList;
    if (!IS_THING_ALIVE(PPub) || !PPub.GetPItem()->GetInterface(&tableList, 0x21))
        return false;
    for (CPointerListNode* node = tableList->_M_node->_M_next; node != tableList->_M_node; node = node->_M_next) {
        if (IS_THING_ALIVE(node->_M_data) && node->_M_data.GetPItem()->HasTC(0xb8))
            tables.push_back(node->_M_data.GetPItem());
    }
    if (tables.size() == 0)
        return false;
    PTable = CIntelligentPointer<CThingObject>(0);
    while (tables.size() != 0) {
        PTable = CIntelligentPointer<CThingObject>(tables.front());
        tables.pop_front();
        CCharString name("ACTION_POINT_GAME_MASTER");
        CTCTavernTable* table;
        if (IS_THING_ALIVE(PTable) && PTable.GetPItem()->GetInterface(&table, 0xb8)
            && table->GetSeat(*GetCreature(), Pos, RHSet, ActionPointIndex, SeatIndex, &name)) {
            PTankard = CIntelligentPointer<CThing>(table->GetTankard(SeatIndex));
            CTCTavernGameCoinGolf* golf;
            PTable.GetPItem()->GetInterface(&golf, 0xcf);
            golf->SetGameMaster(GetCreature());
            return true;
        }
    }
    return false;
}

// ---- stubs ----
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
void CBaseIntelligentPointer::v00() { }
template <class T> void CIntelligentPointer<T>::v01() { }
template class CIntelligentPointer<CThing>;
template class CIntelligentPointer<CThingObject>;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* p = first;
    while (p != last && p->type < *type)
        ++p;
    return p;
}
static char g_lastName[64];
static int g_names = 0;
CCharString::CCharString(const char* text, long) : PStringData(0) { strcpy(g_lastName, text); ++g_names; }
CCharString::~CCharString() { --g_names; }

struct TableInfo { bool seatOk; CThingObject* food; int tried; };
static TableInfo g_tables[4];
static CTCTavernTable g_tableTC[4];
static CThing* g_seatCreature = 0;
static const CCharString* g_seatName = 0;
bool CTCTavernTable::GetSeat(CThing& creature, C3DVector& pos, CRightHandedSet&, long& actionPoint, long& seat,
                             const CCharString* actionPointName)
{
    TableInfo& info = g_tables[this - g_tableTC];
    ++info.tried;
    g_seatCreature = &creature;
    g_seatName = actionPointName;
    if (!info.seatOk)
        return false;
    pos.X = 1.0f; pos.Y = 2.0f; pos.Z = 3.0f;
    actionPoint = 11;
    seat = 3;
    return true;
}
static long g_foodSeat = -1;
CThingObject* CTCTavernTable::GetTankard(long seat) { g_foodSeat = seat; return g_tables[this - g_tableTC].food; }
static CTCTavernGameCoinGolf g_golf[4];
static CThing* g_master[4];
void CTCTavernGameCoinGolf::SetGameMaster(CThing* master) { g_master[this - g_golf] = master; }

// a thing whose interface map holds up to three components
struct TestThing {
    CThingObject thing;
    CInterfaceMap::SEntry entries[3];
    void Init(bool dead)
    {
        memset(this, 0, sizeof(*this));
        thing.m_flags91 = dead ? 1 : 0;
        thing.m_interfaces.first = entries;
        thing.m_interfaces.last = entries;
    }
    void Add(long type, void* component)        // add in ascending type order
    {
        thing.m_presentBits[type >> 5] |= 1UL << (type & 31);
        CInterfaceMap::SEntry* e = thing.m_interfaces.last++;
        e->type = type;
        e->component = (CTCBase*)component;
    }
};

static CPointerListNode g_head, g_nodes[4];
static void BuildTableList(CTCTableList& list, CThing** items, int count)
{
    list._M_node = &g_head;
    CPointerListNode* prev = &g_head;
    for (int i = 0; i < count; ++i) {
        g_nodes[i]._M_data.m_pItem = items[i];
        prev->_M_next = &g_nodes[i];
        g_nodes[i]._M_prev = prev;
        prev = &g_nodes[i];
    }
    prev->_M_next = &g_head;
    g_head._M_prev = prev;
}

int main()
{
    static TestThing pub, tables[4], creatureThing, food0, food1;
    static CTCTableList tableList;
    CAIBrain brain;
    memset(&brain, 0, sizeof(brain));
    creatureThing.Init(false);
    brain.Creature = (CThingAICreature*)&creatureThing.thing;
    food0.Init(false);
    food1.Init(false);

    // the state group's intelligent pointers have no default constructor; GetSeat makes no virtual call
    // on the group or its pointers, so a zeroed record stands in for a constructed one
    static double groupStorage[(sizeof(CAIStateGroup_GameMaster) + 7) / 8];
    memset(groupStorage, 0, sizeof(groupStorage));
    CAIStateGroup_GameMaster& g = *(CAIStateGroup_GameMaster*)groupStorage;
    g.PBrain = &brain;
    g.SeatIndex = -1;
    g.ActionPointIndex = -1;

    // 1. no pub -> false
    CHECK(!g.GetSeat());
    CHECK(g_names == 0);

    // 2. live pub without a table list -> false
    pub.Init(false);
    g.PPub.m_pItem = &pub.thing;
    CHECK(!g.GetSeat());

    // 3. table list whose entries are dead or not tavern tables -> false, PTable untouched
    pub.Add(0x21, &tableList);
    tables[0].Init(true);  tables[0].Add(0xb8, &g_tableTC[0]);
    tables[1].Init(false);                                  // no 0xb8
    CThing* items3[2] = { &tables[0].thing, &tables[1].thing };
    BuildTableList(tableList, items3, 2);
    g.PTable.m_pItem = &food1.thing;
    CHECK(!g.GetSeat());
    CHECK(g.PTable.m_pItem == &food1.thing);
    CHECK(g_tables[0].tried == 0);

    // 4. three tables, the first refuses the seat, the second gives it -> true
    for (int i = 0; i < 3; ++i) {
        tables[i].Init(false);
        tables[i].Add(0xb8, &g_tableTC[i]);
        tables[i].Add(0xcf, &g_golf[i]);
        g_tables[i].seatOk = false;
        g_tables[i].tried = 0;
        g_master[i] = 0;
    }
    g_tables[1].seatOk = true;
    g_tables[1].food = &food0.thing;
    g_tables[2].seatOk = true;
    CThing* items4[4] = { &tables[0].thing, &tables[3].thing, &tables[1].thing, &tables[2].thing };
    tables[3].Init(true);                                   // dead: skipped
    BuildTableList(tableList, items4, 4);
    CHECK(g.GetSeat());
    CHECK(g_tables[0].tried == 1 && g_tables[1].tried == 1 && g_tables[2].tried == 0);
    CHECK(g.PTable.m_pItem == &tables[1].thing);
    CHECK(g.PTankard.m_pItem == &food0.thing);
    CHECK(g_foodSeat == 3 && g.SeatIndex == 3 && g.ActionPointIndex == 11);
    CHECK(g.Pos.X == 1.0f && g.Pos.Y == 2.0f && g.Pos.Z == 3.0f);
    CHECK(g_master[1] == &creatureThing.thing && g_master[0] == 0);
    CHECK(g_seatCreature == &creatureThing.thing);
    CHECK(strcmp(g_lastName, "ACTION_POINT_GAME_MASTER") == 0);
    CHECK(g_names == 0);

    // 5. every table refuses -> false, PTable left on the last one tried
    for (int i = 0; i < 3; ++i) { g_tables[i].seatOk = false; g_tables[i].tried = 0; }
    g.PTankard.m_pItem = 0;
    CHECK(!g.GetSeat());
    CHECK(g_tables[0].tried == 1 && g_tables[1].tried == 1 && g_tables[2].tried == 1);
    CHECK(g.PTable.m_pItem == &tables[2].thing);
    CHECK(g.PTankard.m_pItem == 0);
    CHECK(g_names == 0);

    if (fails == 0) printf("0094cb40_TEST PASS\n");
    return fails;
}