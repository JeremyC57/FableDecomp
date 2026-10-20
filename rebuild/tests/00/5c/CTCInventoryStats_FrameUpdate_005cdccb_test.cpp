// Behaviour test for 005cdccb: src/005cdccb.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryStats::FrameUpdate()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryStats::FrameUpdate() -- counts the creatures the hero killed last frame: walks the owner's entity-event list and, for every kill event (type 0x22) stamped with the previous world frame, bumps that creature's entry in CreaturesKilled (inserting it at 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CDCCB, length 187 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: std::bitset::test @ 0x00410DE0 and CVectorMap::LowerBound const @ 0x004365B0 (the inline GetInterface for type 0x42 =
// CTCEntityEvents), CWorld::GetWorldFrame @ 0x0049D870, the event's virtual slot 1 (GetType), map<unsigned long,long>::find
// @ 0x005D83B3 (catalog "CMemoryAllocatorVariableSize_FindNode", propagated) and insert(const value_type&) @ 0x0045CA09 (the landed
// CRetailTree unique insert; pair<iterator,bool> through a hidden pointer).
// Layout: retail CTCInventoryStats ParentThing +0x04, CreaturesKilled +0x134 (PDB +0x14C; the tree's header pointer first, node key +0x10,
// value +0x14); the entity-events component's list head at +0x0C (STLport list: next at +0, element at +8); the event's frame at +0x04
// and subject at +0x08.
// Shape: the events component is a named local re-read from the frame for the end() test each pass (`cmp ebx,[eax+0Ch]`); the world
// frame is read once into the frame; the subject is kept in edi and copied into the find key and the insert pair; `this` is spilled
// to [ebp-8] for the tree address.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
struct CEntityEvent {
    virtual void v00();
    virtual long GetType();                          // slot 1 (+0x04)
    long Frame;                                      // +0x04
    long Subject;                                    // +0x08
};
struct SEventNode {                                  // STLport _List_node<CEntityEvent*>
    SEventNode*   _M_next;
    SEventNode*   _M_prev;
    CEntityEvent* _M_data;
};
struct CEventList {                                  // std::list<CEntityEvent*>: the header node
    SEventNode* _M_node;
    SEventNode* begin() const { return _M_node->_M_next; }
    SEventNode* end() const { return _M_node; }
};
struct CTCEntityEvents {                             // interface type 0x42
    unsigned char _pad00[0x0c];
    CEventList    Events;                            // +0x0c
};
struct CWorld {
    static long GetWorldFrame();                     // 0x0049D870
};
struct SKillNode {                                   // the tree node: key +0x10, value +0x14
    unsigned char _base[0x10];
    unsigned long Key;
    long          Value;
};
struct SKillPair {                                   // pair<const unsigned long, long>
    unsigned long first;
    long          second;
};
struct CKillMap {                                    // std::map<unsigned long, long> (the header pointer first)
    typedef SKillNode* iterator;
    struct SInsertResult { iterator first; bool second; };   // pair<iterator, bool>
    SKillNode* _M_header;
    iterator end() { return _M_header; }
    iterator find(const unsigned long& key);         // 0x005D83B3
    SInsertResult insert(const SKillPair& value);    // 0x0045CA09
};
SKillNode* InsertKillNode(CKillMap* map, unsigned long key, long value, bool* inserted);
// Same-TU stand-ins for the STLport map members 0x005D83B3 / 0x0045CA09 (template instantiations whose bodies retail's TU
// had): both only read their const-reference argument, so VC7.1 knows the key / pair temporaries go no further.
#pragma auto_inline(off)
__declspec(noinline) CKillMap::iterator CKillMap::find(const unsigned long& key)
{
    SKillNode* y = _M_header;
    SKillNode* x = *(SKillNode**)(_M_header->_base + 4);
    while (x) {
        if (!(x->Key < key)) { y = x; x = *(SKillNode**)(x->_base + 8); }
        else x = *(SKillNode**)(x->_base + 0xc);
    }
    return (y == _M_header || key < y->Key) ? _M_header : y;
}
__declspec(noinline) CKillMap::SInsertResult CKillMap::insert(const SKillPair& value)
{
    SInsertResult r;
    r.first = InsertKillNode(this, value.first, value.second, &r.second);
    return r;
}
#pragma auto_inline(on)
enum { ENTITY_EVENT_CREATURE_KILLED = 0x22 };

namespace NInventory {
struct CTCInventoryStats {
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[0x134 - 0x08];
    CKillMap      CreaturesKilled;                   // +0x134
    void FrameUpdate();
};
void CTCInventoryStats::FrameUpdate()
{
    CTCEntityEvents* events;
    if (ParentThing->GetInterface(&events, 0x42)) {
        long frame = CWorld::GetWorldFrame();
        for (SEventNode* it = events->Events.begin(); it != events->Events.end(); it = it->_M_next) {
            CEntityEvent* event = it->_M_data;
            if (event->GetType() == ENTITY_EVENT_CREATURE_KILLED && event->Frame == frame - 1) {
                long creature = event->Subject;
                CKillMap* killed = &CreaturesKilled;
                SKillNode* found = killed->find(creature);
                if (found != killed->end()) {
                    ++found->Value;
                } else {
                    SKillPair value;
                    value.first = creature;
                    value.second = 1;
                    killed->insert(value);
                }
            }
        }
    }
}
}

// ---- stubs and driver ----
using namespace NInventory;
static bool g_hasEvents = true; static long g_frame = 10;
bool CPresenceBits::test(long bit) { return bit == 0x42 && g_hasEvents; }
void CEntityEvent::v00() {}
long CEntityEvent::GetType() { return 0; }
struct CEvent : CEntityEvent { long type; long GetType() { return type; } };
long CWorld::GetWorldFrame() { return g_frame; }
static SKillNode g_nodes[8]; static int g_nNodes = 0; static unsigned long g_insKey = 0; static long g_insVal = 0; static int g_inserts = 0;
SKillNode* InsertKillNode(CKillMap* map, unsigned long key, long value, bool* inserted)
{
    // append as a right-leaning chain under the header's root pointer (keys inserted in increasing order in this test)
    ++g_inserts; g_insKey = key; g_insVal = value;
    SKillNode* n = &g_nodes[g_nNodes++]; memset(n, 0, sizeof(*n)); n->Key = key; n->Value = value;
    SKillNode** slot = (SKillNode**)(map->_M_header->_base + 4);
    while (*slot) slot = (SKillNode**)((*slot)->_base + 0xc);
    *slot = n; *inserted = true; return n;
}
int main()
{
    SKillNode header; memset(&header, 0, sizeof(header));
    SEventNode list; list._M_next = list._M_prev = &list;
    CEvent e1, e2, e3; e1.type = 0x22; e1.Frame = 9; e1.Subject = 100; e2.type = 0x22; e2.Frame = 9; e2.Subject = 200; e3.type = 0x21; e3.Frame = 9; e3.Subject = 300;
    SEventNode n1, n2, n3, n4; n1._M_data = &e1; n2._M_data = &e2; n3._M_data = &e3; n4._M_data = &e1;
    list._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &n4; n4._M_next = &list;
    CTCEntityEvents events; memset(&events, 0, sizeof(events)); events.Events._M_node = &list;
    CInterfaceMap::SEntry entries[2]; entries[0].type = 0x42; entries[0].component = &events; entries[1].type = 0x50; entries[1].component = 0;
    static CThing thing; memset(&thing, 0, sizeof(thing)); thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    static CTCInventoryStats stats; memset(&stats, 0, sizeof(stats)); stats.ParentThing = &thing; stats.CreaturesKilled._M_header = &header;
    stats.FrameUpdate();
    // e1 twice (inserted, then counted), e2 once, e3 is the wrong type
    CHECK(g_inserts == 2); CHECK(g_nNodes == 2); CHECK(g_nodes[0].Key == 100 && g_nodes[0].Value == 2); CHECK(g_nodes[1].Key == 200 && g_nodes[1].Value == 1);
    // events from another frame are ignored
    g_frame = 20; stats.FrameUpdate(); CHECK(g_inserts == 2); CHECK(g_nodes[0].Value == 2);
    // no entity-events interface: nothing
    g_frame = 10; g_hasEvents = false; stats.FrameUpdate(); CHECK(g_nodes[0].Value == 2);
    g_hasEvents = true; stats.FrameUpdate(); CHECK(g_nodes[0].Value == 4); CHECK(g_nodes[1].Value == 2); CHECK(g_inserts == 2);
    if (fails == 0) printf("005cdccb_TEST PASS\n"); else printf("005cdccb_TEST FAIL\n");
    return fails;
}