#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif
// Model of GFRemapToThingAfterSerialisation(CIntelligentPointer<CThing>&) @ 0x0088AFF0: a live thing carrying the
// interface-0xEB UID-remap component is killed and the pointer is re-aimed at the thing with the remapped UID.
struct SThingUIDRemapComponent {
    u64 RemapUID;
};
struct CInterfaceMap {                                  // sorted (type, component) entries
    struct SEntry { int type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const int* type)
    {
        SEntry* lo = first;
        int n = (int)(last - first);
        while (n > 0) {
            int half = n >> 1;
            SEntry* mid = lo + half;
            if (mid->type < *type) {
                lo = mid + 1;
                n -= half + 1;
            } else {
                n = half;
            }
        }
        return lo;
    }
};
class CThing {
public:
    unsigned int  m_presentBits[9];                     // bitset of interface types
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;                            // bit 0: dead
    int           KillCalls;
    void Kill(bool) { ++KillCalls; m_flags91 |= 1; }
    template <class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1U << (interfaceType & 31))) {
            int type = interfaceType;
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
class CThingManager {
public:
    CThing* Things;
    int     Count;
    CThing* GetPThingWithUID(u64 uid) { return (int)uid < Count ? &Things[(int)uid] : 0; }
};
static CThingManager* g_pThingManager = 0;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { }
    void* GetPItem() const { return PData; }
    void SetPItem(void* item) { PData = item; }
    void* PData;
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
    T* operator->() const { return (T*)GetPItem(); }
};
static const int TC_THING_UID_REMAP = 0xEB;
inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_flags91 & 1);
}
void __fastcall GFRemapToThingAfterSerialisation(CIntelligentPointer<CThing>& pointer)
{
    SThingUIDRemapComponent* remap;
    if (IsThingAlive((CThing*)pointer.GetPItem()) && pointer->GetInterface(&remap, TC_THING_UID_REMAP)) {
        u64 uid = remap->RemapUID;
        pointer->Kill(false);
        pointer = CIntelligentPointer<CThing>(g_pThingManager->GetPThingWithUID(uid));
    }
}
static void InitThing(CThing& t, CInterfaceMap::SEntry* entries, int count)
{
    for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    for (int j = 0; j < count; ++j)
        t.m_presentBits[entries[j].type >> 5] |= 1U << (entries[j].type & 31);
    t.m_interfaces.first = entries;
    t.m_interfaces.last = entries + count;
    t.m_flags91 = 0;
    t.KillCalls = 0;
}
int main()
{
    bool ok = true;
    CThing world[3];
    CInterfaceMap::SEntry none[1] = { { 4, 0 } };
    for (int i = 0; i < 3; ++i) InitThing(world[i], none, 1);
    CThingManager manager = { world, 3 };
    g_pThingManager = &manager;
    SThingUIDRemapComponent remap = { 2 };
    CInterfaceMap::SEntry withRemap[3] = { { 4, 0 }, { 0x48, 0 }, { TC_THING_UID_REMAP, &remap } };
    CThing placeholder;
    InitThing(placeholder, withRemap, 3);
    CIntelligentPointer<CThing> p(&placeholder);
    GFRemapToThingAfterSerialisation(p);
    if (p.GetPItem() != &world[2] || placeholder.KillCalls != 1) ok = false;
    CThing dead;                                        // already dead: left alone
    InitThing(dead, withRemap, 3);
    dead.m_flags91 = 1;
    CIntelligentPointer<CThing> q(&dead);
    GFRemapToThingAfterSerialisation(q);
    if (q.GetPItem() != &dead || dead.KillCalls != 0) ok = false;
    CIntelligentPointer<CThing> r(&world[0]);           // no remap component: left alone
    GFRemapToThingAfterSerialisation(r);
    if (r.GetPItem() != &world[0] || world[0].KillCalls != 0) ok = false;
    CIntelligentPointer<CThing> n((CThing*)0);
    GFRemapToThingAfterSerialisation(n);
    if (n.GetPItem() != 0) ok = false;
    if (!ok) { std::printf("0088aff0_TEST FAIL\n"); return 1; }
    std::printf("0088aff0_TEST PASS\n");
    return 0;
}