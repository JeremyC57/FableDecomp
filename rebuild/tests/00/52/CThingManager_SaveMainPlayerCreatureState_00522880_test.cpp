// Behaviour test for 0x00522880: the reconstructed body (pragmas stripped) driven through recording stubs.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
// CThingManager::SaveMainPlayerCreatureState(CCharString& script) -- collect the hero plus every persisted inventory item the hero owns on the hero's map and write them as a thing list
// Leaf: SaveMainPlayerCreatureState
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroCreature @ 0x00487DC0, vector<CThing*> grow @ 0x00436AC0,
// CThing::IsGameStatePersisted @ 0x004C8280, CThing::IsLevelLess @ 0x004C7930, CThing::SetCurrentMapNumber @ 0x004C7580,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (non-const copy), CTCInventoryItem::GetInventoryOwner @ 0x005D8E90, thing-list writer @ 0x00521EB0,
// ~CCharString @ 0x0099EAE0, free @ 0x00BFEA14.
// Shape: both vectors stay in the frame (the grow template's same-TU body keeps the vector's address; the writer's
// same-TU stand-in fills the string vector's first/last and reads its end, so the vector's words are not reloaded around
// the ~CCharString loop); push_back keeps `last` in a local; the hero is a CThingCreature* upcast into a CThing* temporary
// (the separate [esp+0x10] slot), the loop thing is pushed through an rvalue cast (its own [esp+0x1c] slot).
// Layout (retail CThingManager): Things list sentinel +0x18, PlayerManager +0x30, Saving flag +0x9a;
// CThing UsingComponents +0x20, Components +0x44, CurrentMapNumber +0x8e, flags byte +0x91.
#include <new>

class CCharString { public:
    void* m_pData;
    ~CCharString();                                            // 0x0099EAE0
};
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };

template <class T> struct CRetailVector {
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) { }
    ~CRetailVector()
    {
        for (T* p = first; p != last; ++p)
            p->~T();
        if (first) free(first);
    }
};
template <class T> __forceinline void Construct(T* p, const T& value) { new (p) T(value); }
template <class T> struct CRetailPtrVector {
    T* first;
    T* last;
    T* end;
    CRetailPtrVector() : first(0), last(0), end(0) { }
    ~CRetailPtrVector() { if (first) free(first); }
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00436AC0
    void push_back(const T& value)
    {
        T* finish = last;
        if (finish != end) {
            Construct(finish, value);
            last = finish + 1;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};

void* GrowVectorStorage_00436ac0(void* vec, unsigned long count);
template <class T>
 void CRetailPtrVector<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    T* p = (T*)GrowVectorStorage_00436ac0(this, count);
    if (p)
        *p = value;
}

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                   // +0x20
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad4c[0x8e - 0x4c];
    short CurrentMapNumber;                             // +0x8e
    unsigned char _pad90;
    unsigned char Flags91;                              // +0x91
    bool IsGameStatePersisted();                        // 0x004C8280
    bool IsLevelLess();                                 // 0x004C7930
    void SetCurrentMapNumber(long map);                 // 0x004C7580
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CTCInventoryItem { public:
    CThing* GetInventoryOwner();                        // 0x005D8E90
};
class CThingCreature : public CThing { };
class CPlayer { public: CThingCreature* GetHeroCreature(); };          // 0x00487DC0
class CPlayerManager { public: CPlayer* GetMainPlayer(); };    // 0x00449970

struct SThingListNode {
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
    CThing*         _M_data;
};

class CThingManager {
public:
    void*           __vftable;                          // +0x00
    unsigned char   _pad04[0x18 - 0x04];
    SThingListNode* Things;                             // +0x18
    unsigned char   _pad1c[0x30 - 0x1c];
    CPlayerManager* PlayerManager;                      // +0x30
    unsigned char   _pad34[0x9a - 0x34];
    bool            Saving;                             // +0x9a
    void SaveThingList(CCharString& script, long mode, CRetailPtrVector<CThing*>& things, CRetailVector<CCharString>* names);   // 0x00521EB0
    void SaveMainPlayerCreatureState(CCharString& script);
};


CCharString* WriteThings(CCharString& script, long mode, CThing** first, CThing** last, CCharString* capacity, unsigned long* count);
 void CThingManager::SaveThingList(CCharString& script, long mode, CRetailPtrVector<CThing*>& things, CRetailVector<CCharString>* names)
{
    unsigned long count;
    CCharString* written = WriteThings(script, mode, things.first, things.last, names->end, &count);
    names->first = written;
    names->last = written + count;
}

void CThingManager::SaveMainPlayerCreatureState(CCharString& script)
{
    Saving = true;
    CThingCreature* hero = PlayerManager->GetMainPlayer()->GetHeroCreature();
    long map = hero->CurrentMapNumber;
    CRetailPtrVector<CThing*> things;
    things.push_back(hero);
    for (SThingListNode* node = Things->_M_next; node != Things; node = node->_M_next) {
        CThing* thing = node->_M_data;
        if (thing->IsGameStatePersisted()) {
            if (thing->IsLevelLess() || (thing->Flags91 & 0x20))
                thing->SetCurrentMapNumber(map);
            if (thing->CurrentMapNumber == map && thing->HasTC(0x1a)
                && ((CTCInventoryItem*)thing->GetTC(0x1a))->GetInventoryOwner() == hero)
                things.push_back((CThing*)thing);
        }
    }
    CRetailVector<CCharString> names;
    Saving = true;
    SaveThingList(script, 2, things, &names);
    Saving = false;
}

// ---- recording stubs ----
static int g_dtors = 0;
static int g_grows = 0;
static int g_setMap = 0;
static int g_written = 0;
static CThing* g_order[16];
static bool g_savingDuringWrite = false;
static CThingManager* g_mgr = 0;
static CThingCreature g_hero;
static CPlayer g_player;
CCharString::~CCharString() { ++g_dtors; }
CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
CThingCreature* CPlayer::GetHeroCreature() { return &g_hero; }
static bool g_persisted[8];
static bool g_levelLess[8];
static CThing g_things[8];
static CThing* g_owner[8];
static CTCInventoryItem g_items[8];
static int IndexOf(const CThing* t) { for (int i = 0; i < 8; ++i) if (&g_things[i] == t) return i; return -1; }
bool CThing::IsGameStatePersisted() { int i = IndexOf(this); return i >= 0 && g_persisted[i]; }
bool CThing::IsLevelLess() { int i = IndexOf(this); return i >= 0 && g_levelLess[i]; }
void CThing::SetCurrentMapNumber(long m) { ++g_setMap; CurrentMapNumber = (short)m; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
CThing* CTCInventoryItem::GetInventoryOwner() { return g_owner[this - g_items]; }
void* GrowVectorStorage_00436ac0(void* vec, unsigned long count)
{
    ++g_grows;
    CRetailPtrVector<CThing*>* v = (CRetailPtrVector<CThing*>*)vec;
    unsigned long size = (unsigned long)(v->last - v->first);
    CThing** p = (CThing**)malloc((size + count + 1) * sizeof(CThing*));
    for (unsigned long i = 0; i < size; ++i) p[i] = v->first[i];
    if (v->first) free(v->first);
    v->first = p;
    v->last = p + size + count;
    v->end = p + size + count + 1;
    return p + size;
}
CCharString* WriteThings(CCharString& script, long mode, CThing** first, CThing** last, CCharString* capacity, unsigned long* count)
{
    g_savingDuringWrite = g_mgr->Saving && mode == 2 && capacity == 0;
    g_written = (int)(last - first);
    for (int i = 0; i < g_written && i < 16; ++i) g_order[i] = first[i];
    CCharString* names = (CCharString*)malloc(3 * sizeof(CCharString));
    *count = 3;
    return names;
}

static SThingListNode g_nodes[9];
static CInterfaceMap::SEntry g_entries[8][2];

int main()
{
    CThingManager mgr;
    memset(&mgr, 0, sizeof(mgr));
    g_mgr = &mgr;
    CPlayerManager pm;
    mgr.PlayerManager = &pm;
    g_hero.CurrentMapNumber = 3;
    memset(g_things, 0, sizeof(g_things));
    for (int i = 0; i < 8; ++i) {
        g_things[i].CurrentMapNumber = 3;
        g_entries[i][0].type = 5;  g_entries[i][0].component = 0;
        g_entries[i][1].type = 0x1a; g_entries[i][1].component = (CTCBase*)&g_items[i];
        g_things[i].Components.first = g_entries[i];
        g_things[i].Components.last = g_entries[i] + 2;
        g_owner[i] = &g_hero;
        g_persisted[i] = true;
        g_things[i].UsingComponents[0] = 1UL << 26;
    }
    g_levelLess[0] = true; g_things[0].CurrentMapNumber = 7;      // level-less: moved to map 3, kept
    g_things[1].UsingComponents[0] = 0;                            // no inventory item: dropped
    g_persisted[2] = false;                                        // not persisted: dropped
    g_owner[3] = &g_things[0];                                     // owned by someone else: dropped
    g_things[4].Flags91 = 0x20; g_things[4].CurrentMapNumber = 9;  // flagged: moved to map 3, kept
    g_things[5].CurrentMapNumber = 4;                              // other map: dropped
    // things 6 and 7 kept
    SThingListNode* sentinel = &g_nodes[8];
    SThingListNode* prev = sentinel;
    for (int i = 0; i < 8; ++i) {
        g_nodes[i]._M_data = &g_things[i];
        prev->_M_next = &g_nodes[i];
        prev = &g_nodes[i];
    }
    prev->_M_next = sentinel;
    mgr.Things = sentinel;

    CCharString script;
    script.m_pData = 0;
    mgr.SaveMainPlayerCreatureState(script);

    bool ok = g_written == 5
        && g_order[0] == &g_hero && g_order[1] == &g_things[0] && g_order[2] == &g_things[4]
        && g_order[3] == &g_things[6] && g_order[4] == &g_things[7]
        && g_setMap == 2 && g_things[0].CurrentMapNumber == 3 && g_things[4].CurrentMapNumber == 3
        && g_savingDuringWrite && mgr.Saving == false
        && g_dtors >= 3 && g_grows >= 2;
    if (ok) printf("00522880_TEST PASS\n");
    else printf("00522880_TEST FAIL written=%d setMap=%d dtors=%d grows=%d\n", g_written, g_setMap, g_dtors, g_grows);
    return ok ? 0 : 1;
}