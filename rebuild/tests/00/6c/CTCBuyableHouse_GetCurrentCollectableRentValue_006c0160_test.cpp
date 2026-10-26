// Behaviour test for 006c0160: src/006c0160.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCBuyableHouse::GetCurrentCollectableRentValue() const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCBuyableHouse::GetCurrentCollectableRentValue() const -- the rent waiting in the house: the gold of every living small gold bag ("OBJECT_GOLDBAG_SMALL_WITH_COINS") among the building's contents
//   name and the 0xCC key (total +0, shared +4, gold index +8); ours gives the 0x21 key its own word (0x10 frame), so
//   every frame offset after it is 4 higher
// Leaf: GetCurrentCollectableRentValue
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C0160, 271 bytes (+1 int3 padding), `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (both lookups), CCharString(const char*,
//   long) 0x0099EBF0, ~CCharString 0x0099EAE0, CTCBase::GetDefinitions() const 0x006869D0, CDefinitionManager::
//   GetDefGlobalIndexFromName(const CCharString&) 0x009AD410, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CTCGold::GetGold 0x007CBDC0. Literal 0x0125D760.
// Layout: engine/CTCBuyableHouse.h is the PDB view of the class (only ParentThing +4 is read here, so `this` is the
//   CTCBase prefix); the building contents component (interface 0x21) keeps a VC6 list of CIntelligentPointer<CThing>
//   with its head at +0x0c (landed CAIStateGroup_SitForMeal::GetPFreeChair 0x00945B10); CThing dead bit +0x91 bit 0,
//   global def index (unsigned short) +0x8c; interface 0xCC is CTCGold.
// Shape: the total is declared first and zero-initialised (the early returns reuse the zero register, the loop sum
//   stays in its frame word); the contents component is null-tested; the gold bag's def name is a named CCharString in
//   its own block (homed in the lookup key's word); both lookups are const (`const CThing*`), the gold lookup the
//   ternary entry pick with its own null test.
// Tried: out-parameter GetInterface for the contents, an implicit CCharString conversion temporary instead of the named
//   block local, the contents through a `__forceinline` accessor: the 0x21 key never shares the name's word.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound template member (compiled in the caller's TU):
// binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
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
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                    // +0x8c global definition index
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  m_stateFlags;                // +0x91 (bit 0: dead)
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThing>: vptr + item
    void*   __vftable;
    CThing* m_pItem;
    const CThing* GetPItem() const;             // 0x00A01B50
};
struct SThingListNode {
    SThingListNode*         next;
    SThingListNode*         prev;
    CBaseIntelligentPointer value;              // +0x08
};
struct CTCBuildingContents {                    // interface 0x21
    unsigned char   _pad00[0x0c];
    SThingListNode* ThingsHead;                 // +0x0c (VC6 list head)
};
class CTCGold { public:                                // interface 0xCC
    long GetGold();                             // 0x007CBDC0
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};

class CTCBuyableHouse {
public:
    void*         __vftable;                    // +0x00 (CTCBase)
    const CThing* ParentThing;                  // +0x04
    CDefinitionManager* GetDefinitions() const; // 0x006869D0 (CTCBase)
    long GetCurrentCollectableRentValue() const;
};

long CTCBuyableHouse::GetCurrentCollectableRentValue() const
{
    long total = 0;
    if (ParentThing->HasTC(0x21)) {
        CTCBuildingContents* contents = ParentThing->GetTC<CTCBuildingContents>(0x21);
        if (contents) {
            long goldIndex;
            {
                CCharString name("OBJECT_GOLDBAG_SMALL_WITH_COINS", -1);
                goldIndex = GetDefinitions()->GetDefGlobalIndexFromName(name);
            }
            for (SThingListNode* node = contents->ThingsHead->next; node != contents->ThingsHead; node = node->next) {
                const CThing* thing = node->value.GetPItem();
                if (thing && !(thing->m_stateFlags & 1) && thing->DefIndex == goldIndex && thing->HasTC(0xcc)) {
                    CTCGold* gold = thing->GetTC<CTCGold>(0xcc);
                    if (gold)
                        total += gold->GetGold();
                }
            }
        }
    }
    return total;
}

// ---- stubs and driver ----
static long g_goldIndex = 42; static int g_lookup = 0; static CDefinitionManager g_defs;
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::~CCharString() {}
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { ++g_lookup; CHECK(strcmp((const char*)name.PStringData, "OBJECT_GOLDBAG_SMALL_WITH_COINS") == 0); return g_goldIndex; }
CDefinitionManager* CTCBuyableHouse::GetDefinitions() const { return &g_defs; }
long CTCGold::GetGold() { return *(long*)this; }
const CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static void addTC(CThing& t, CInterfaceMap::SEntry* e, int n) { t.m_interfaces.first = e; t.m_interfaces.last = e + n; for (int i = 0; i < n; ++i) t.m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31); }
int main()
{
    static CThing house, bag1, bag2, other; static CTCBuildingContents contents; static long gold1 = 5, gold2 = 7, gold3 = 100;
    CInterfaceMap::SEntry he[1] = { { 0x21, (CTCBase*)&contents } }; addTC(house, he, 1);
    CInterfaceMap::SEntry e1[2] = { { 0x10, 0 }, { 0xcc, (CTCBase*)&gold1 } }; addTC(bag1, e1, 2); bag1.DefIndex = 42;
    CInterfaceMap::SEntry e2[1] = { { 0xcc, (CTCBase*)&gold2 } }; addTC(bag2, e2, 1); bag2.DefIndex = 42;
    CInterfaceMap::SEntry e3[1] = { { 0xcc, (CTCBase*)&gold3 } }; addTC(other, e3, 1); other.DefIndex = 43;
    static SThingListNode head, n1, n2, n3, n4; contents.ThingsHead = &head;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &n4; n4.next = &head;
    n1.value.m_pItem = &bag1; n2.value.m_pItem = &other; n3.value.m_pItem = &bag2; n4.value.m_pItem = 0;
    CTCBuyableHouse h; h.ParentThing = &house;
    CHECK(h.GetCurrentCollectableRentValue() == 12 && g_lookup == 1);
    bag2.m_stateFlags = 1; CHECK(h.GetCurrentCollectableRentValue() == 5);   // dead bag skipped
    bag1.m_presentBits[0xcc >> 5] = 0; CHECK(h.GetCurrentCollectableRentValue() == 0);   // no gold component
    house.m_presentBits[0x21 >> 5] = 0; CHECK(h.GetCurrentCollectableRentValue() == 0 && g_lookup == 3);   // not a building: no lookup
    if (fails == 0) printf("006c0160_TEST PASS\n"); else printf("006c0160_TEST FAIL\n");
    return fails;
}