// Behaviour test for 008ed170: src/008ed170.cpp copied verbatim; Create, the item-vector erase and AddItemToInventory
// stubbed so only pending items (def index > 0) are created at the parent's position and added, then the list is cleared.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCContainer::AddItemsToInventory() -- create every pending container item (def index > 0) at the container's position, add it to the inventory, then clear the item list; false when there was nothing to add
// Retry 10b (accessor sweep, FIXES B-4/B-5): the parent thing is read through an inline accessor; the plain member read gives the parked register difference.
//
// Retail 0x008ED170, 122 bytes (plain `ret`). Callees: CThing::GetPosition() const @ 0x004C73D0,
// CThingObject::Create(long, const C3DVector&, long, const CRightHandedSet*, unsigned long,
// const CCharString&) @ 0x00703210 (`__fastcall`), CTCContainer::AddItemToInventory(CThing*)
// @ 0x008ED090, vector<pair<long,CCharString>>::erase(iterator, iterator) @ 0x0074FB20 (out of line;
// manifest "std::vector_CopyAssignSegment").
// Layout: Ego_r.pdb CTCContainer: ContainerItems (vector<pair<long,CCharString>>, 8-byte elements) at
// +0x0c, the retail STLport {first, last, end}; ParentThing +0x04.
// Shape: an empty() early return (separate `xor al,al` exit); an unsigned index loop `i < size()`
// (`sar ecx,3`, recomputed) over `*(begin() + i)` (element address in esi); Create's arguments are
// evaluated right to left, so GetPosition runs after the constant pushes; the inline clear() is
// erase(begin(), end()) with the vector's address formed once in edi.
struct C3DVector;
struct CRightHandedSet;
struct CCharString { void* PStringData; };

struct CThing {
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

struct CThingObject;
CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long createFlags,
                                const CRightHandedSet* orientation, unsigned long uid,
                                const CCharString& scriptName);   // 0x00703210

struct SContainerItem {                             // pair<long, CCharString>
    long        first;                              // def index (count-like, > 0 when pending)
    CCharString second;
};

struct CContainerItemVector {                       // STLport vector<pair<long,CCharString>>
    SContainerItem* _M_start;
    SContainerItem* _M_finish;
    SContainerItem* _M_end_of_storage;
    SContainerItem* begin() { return _M_start; }
    SContainerItem* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    SContainerItem& operator[](unsigned long n) { return *(begin() + n); }
    SContainerItem* erase(SContainerItem* first, SContainerItem* last);   // 0x0074FB20
    void clear() { erase(begin(), end()); }
};

struct CTCContainer {
    virtual void v00();
    CThing*              ParentThing;               // +0x04 (CTCBase)
    CThing* GetParentThing() const { return ParentThing; }
    unsigned char        _pad08[4];
    CContainerItemVector ContainerItems;            // +0x0c
    void AddItemToInventory(CThing* item);          // 0x008ED090
    bool AddItemsToInventory();
};

bool CTCContainer::AddItemsToInventory()
{
    if (!ContainerItems.empty())
    {
        for (unsigned long i = 0; i < ContainerItems.size(); ++i)
        {
            SContainerItem& item = ContainerItems[i];
            if (item.first > 0)
            {
                CThing* created = (CThing*)Create(item.first, GetParentThing()->GetPosition(), 5, 0, 0, item.second);
                if (created)
                    AddItemToInventory(created);
            }
        }
        ContainerItems.clear();
        return true;
    }
    return false;
}
// ---- end source ----
static int g_pos; static const C3DVector* g_lastPos = 0;
const C3DVector& CThing::GetPosition() const { return *(const C3DVector*)&g_pos; }
static long g_created[8]; static int g_nCreated = 0; static void* g_names[8];
static unsigned char g_objects[8][4];
CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long createFlags, const CRightHandedSet* orientation,
                                unsigned long uid, const CCharString& scriptName)
{
    g_lastPos = &position;
    if (createFlags != 5 || orientation != 0 || uid != 0) return 0;
    g_names[g_nCreated] = scriptName.PStringData; g_created[g_nCreated] = defIndex;
    if (defIndex == 99) return 0;                        // creation failure
    return (CThingObject*)g_objects[g_nCreated++];
}
static int g_added = 0; static CThing* g_addedThings[8];
void CTCContainer::AddItemToInventory(CThing* item) { g_addedThings[g_added++] = item; }
void CTCContainer::v00() {}
static int g_erases = 0;
SContainerItem* CContainerItemVector::erase(SContainerItem* first, SContainerItem* last) { ++g_erases; _M_finish = first; return first; }
int main()
{
    static CThing parent;
    SContainerItem items[4] = { { 3, { (void*)"a" } }, { 0, { (void*)"b" } }, { 99, { (void*)"c" } }, { 7, { (void*)"d" } } };
    CTCContainer c; c.ParentThing = &parent; c.ContainerItems._M_start = c.ContainerItems._M_finish = c.ContainerItems._M_end_of_storage = 0;
    CHECK(c.AddItemsToInventory() == false && g_erases == 0);           // empty
    c.ContainerItems._M_start = items; c.ContainerItems._M_finish = items + 4; c.ContainerItems._M_end_of_storage = items + 4;
    CHECK(c.AddItemsToInventory() == true);
    CHECK(g_nCreated == 2 && g_created[0] == 3 && g_created[1] == 7 && strcmp((const char*)g_names[1], "d") == 0);
    CHECK(g_added == 2 && g_addedThings[0] == (CThing*)g_objects[0] && g_addedThings[1] == (CThing*)g_objects[1]);
    CHECK(g_lastPos == (const C3DVector*)&g_pos && g_erases == 1 && c.ContainerItems.empty());
    if (fails == 0) printf("008ed170_TEST PASS\n"); else printf("008ed170_TEST FAIL\n");
    return fails;
}