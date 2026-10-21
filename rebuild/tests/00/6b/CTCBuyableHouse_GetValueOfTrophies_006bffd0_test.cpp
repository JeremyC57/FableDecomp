// Behaviour test for 006bffd0: src/006bffd0.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCBuyableHouse::GetValueOfTrophies
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCBuyableHouse::GetValueOfTrophies() const -- sum the value of every live trophy mounted in the house (the trophy def's value, or 50 when the trophy has no trophy def)
// Leaf: GetValueOfTrophies
// Retail 0x006BFFD0, 336 bytes, `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (const) 0x004365B0 (interfaces 0x21 thing list, 0xDA wall
// mount), CBaseIntelligentPointer::GetPItem 0x00A01B50, the wall mount's mounted-thing accessor 0x007DE4F0,
// CDefClassBase::GetSubDef<CTrophyDef> 0x0059350C; the defs' slot 1 release.
// Layout: CTCBase ParentThing +4; the interface-0x21 component's list header +0x0C (landed CountMoneyBags 0x006C04D0
// model); CThing bitset +0x20 (0xDA = word 6 bit 26), map +0x44, def +0x70, dead flag +0x91; CTrophyDef value +0x28.
// Shape: the list walk of CountMoneyBags; the trophy's def is an inline-copied CDefPointer (add-ref after the out pointer
// is zeroed, register-held, released right after GetSubDef); the out CDefPointer is re-read from the frame for the value
// and its release.
// Shape (stand-ins): CDefClassBase::GetSubDef<CTrophyDef> (a template instantiation, out of line in retail) has a same-TU
//   stand-in body that writes the out slot only, and the CVectorMap LowerBound (const) instantiation has its same-TU
//   binary-search body; with both visible VC7.1 keeps the running value in ebp and releases the out def with one load.
struct CTCBase;
struct CBaseObject;
struct CDefPointeeBase {
    virtual void Destroy();                             // slot 0
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CTrophyDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long          Value;                                // +0x28
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CTrophyDef>* out);       // 0x0059350C
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CTrophyDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
struct CThing;
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* Get() const { return (T*)GetPItem(); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;         // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
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

struct SThingNode {
    SThingNode*                 next;
    SThingNode*                 prev;
    CIntelligentPointer<CThing> thing;                  // +0x08
};
struct CTCThingList_0x21 {
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;                               // +0x0c
};
struct CThing {
    unsigned char              _pad0[0x20];
    unsigned long              m_presentBits[9];        // +0x20
    CInterfaceMap              m_interfaces;            // +0x44
    unsigned char              _pad_0x4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;                    // +0x70
    unsigned char              _pad_0x74[0x91 - 0x74];
    unsigned char              m_stateFlags;            // +0x91 (bit 0 = dead)
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CTCWallMount {
    CThing* GetMountedThing() const;                    // 0x007DE4F0
};
inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.Get() && !(p.Get()->m_stateFlags & 1);
}
class CTCBuyableHouse {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    long GetValueOfTrophies() const;
};

long CTCBuyableHouse::GetValueOfTrophies() const
{
    long value = 0;
    CTCThingList_0x21* list;
    if (ParentThing->GetInterface(&list, 0x21)) {
        for (SThingNode* node = list->m_head->next; node != list->m_head; node = node->next) {
            CTCWallMount* mount;
            if (IsThingAlive(node->thing) && node->thing.Get()->GetInterface(&mount, 0xda)) {
                CThing* trophy = mount->GetMountedThing();
                if (trophy && !(trophy->m_stateFlags & 1)) {
                    CDefPointer<CTrophyDef> def;
                    if (mount->GetMountedThing()->GetDef()->GetSubDef(&def))
                        value += def->Value;
                    else
                        value += 50;
                }
            }
        }
    }
    return value;
}

// ---- stubs ----
static int g_deleted;
void CDefPointeeBase::Destroy() {}
void CDefPointeeBase::Delete() { ++g_deleted; }
static CTrophyDef* g_trophyDef;
void* FindDef_CDefClassBase_GetSubDef_0() { if (g_trophyDef) ++g_trophyDef->m_refCount; return g_trophyDef; }
void CBaseIntelligentPointer::v00() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static CThing* g_mounted[8]; static int g_mountCalls;
CThing* CTCWallMount::GetMountedThing() const { ++g_mountCalls; return g_mounted[this - (const CTCWallMount*)0 == 0 ? 0 : 0]; }
void CTCBuyableHouse::v00() {}
static long g_buf[8][sizeof(CThing) / 4 + 1];
static CThing* NewThing(int i) { memset(g_buf[i], 0, sizeof g_buf[i]); return (CThing*)g_buf[i]; }
static void SetInterfaces(CThing* t, CInterfaceMap::SEntry* e, int n)
{
    memset(t->m_presentBits, 0, sizeof t->m_presentBits);
    for (int i = 0; i < n; ++i) t->m_presentBits[e[i].type >> 5] |= 1UL << (e[i].type & 31);
    t->m_interfaces.first = e; t->m_interfaces.last = e + n;
}
int main()
{
    static CTCThingList_0x21 list; static SThingNode head, nodes[3]; static CTCWallMount mounts[3];
    static CDefClassBase thingDef; static CTrophyDef tdef;
    thingDef.m_refCount = 100; tdef.Value = 300; tdef.m_refCount = 1;
    CThing* house = NewThing(0); CThing* m0 = NewThing(1); CThing* m1 = NewThing(2); CThing* plain = NewThing(3); CThing* trophy = NewThing(4);
    trophy->PDef.Object = &thingDef;
    CInterfaceMap::SEntry he[] = { { 3, 0 }, { 0x21, (CTCBase*)&list } }; SetInterfaces(house, he, 2);
    CInterfaceMap::SEntry e0[] = { { 0xda, (CTCBase*)&mounts[0] } }; SetInterfaces(m0, e0, 1);
    CInterfaceMap::SEntry e1[] = { { 4, 0 }, { 0xda, (CTCBase*)&mounts[1] }, { 0xff, 0 } }; SetInterfaces(m1, e1, 3);
    CInterfaceMap::SEntry ep[] = { { 4, 0 } }; SetInterfaces(plain, ep, 1);
    CThing* things[3] = { m0, plain, m1 };
    SThingNode* prev = &head;
    for (int i = 0; i < 3; ++i) { nodes[i].thing.m_pItem = (CBaseObject*)things[i]; prev->next = &nodes[i]; nodes[i].prev = prev; prev = &nodes[i]; }
    prev->next = &head; head.prev = prev; list.m_head = &head;
    CTCBuyableHouse h; h.ParentThing = house;
    g_mounted[0] = trophy;
    // two live mounts with a trophy that has a trophy def: 2 * 300, def references balanced
    g_trophyDef = &tdef; g_deleted = 0;
    CHECK(h.GetValueOfTrophies() == 600 && tdef.m_refCount == 1 && thingDef.m_refCount == 100 && g_deleted == 0);
    // trophy without a trophy def: 50 each
    g_trophyDef = 0;
    CHECK(h.GetValueOfTrophies() == 100 && thingDef.m_refCount == 100);
    // dead trophy: nothing; empty mount: nothing
    trophy->m_stateFlags = 1;
    CHECK(h.GetValueOfTrophies() == 0);
    trophy->m_stateFlags = 0; g_mounted[0] = 0;
    CHECK(h.GetValueOfTrophies() == 0);
    // dead mount thing skipped
    g_mounted[0] = trophy; m1->m_stateFlags = 1; g_trophyDef = &tdef;
    CHECK(h.GetValueOfTrophies() == 300 && tdef.m_refCount == 1);
    // last reference released through Delete
    m1->m_stateFlags = 0; tdef.m_refCount = 0;
    CHECK(h.GetValueOfTrophies() == 600 && g_deleted == 2);
    // house without the list
    SetInterfaces(house, he, 1);
    CHECK(h.GetValueOfTrophies() == 0);
    if (fails == 0) printf("006bffd0_TEST PASS\n"); else printf("006bffd0_TEST FAIL\n");
    return fails;
}