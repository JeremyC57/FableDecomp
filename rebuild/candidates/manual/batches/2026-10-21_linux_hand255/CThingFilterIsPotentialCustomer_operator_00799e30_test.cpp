#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
// CThingFilter_IsPotentialCustomer::operator()(const CThing&) const -- a villager or hero (thing type 1/2) creature that is not the shopkeeper itself, has neither component 0x90 nor 0xb4, is not busy with a village task, has not already been a customer (unless allowed), does not dislike the shopkeeper (heroes), whose 0x75 component's item is the shopper container, and is either into a container or within 6 units of Pos
// Leaf: ??RCThingFilter_IsPotentialCustomer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799E30, length 384 bytes, `ret 4`.
//   Name from Ego_r.pdb (`??RCThingFilter_IsPotentialCustomer@@QBE_NABVCThing@@@Z`).
// Callees: CThingCreatureBase::GetCreatureType @ 0x00662000, the const CVectorMap LowerBound @ 0x004365B0,
//   CTCVillageMember::GetPVillage @ 0x007EE540, CVillage::IsVillagerPerformingVillageTask @ 0x00659104,
//   CTCShopKeeper::HasBeenCustomer @ 0x00798DD0, the opinion manager's GetOpinionAttitudeToPlayer @ 0x00701840
//   (reached through [[0x013B8A1C]+0x24]+0x74), CIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition
//   const @ 0x004C73D0; 6.0f @ 0x0126C11C.
// Layout (Ego_r.pdb, matches retail): ShopperContainer +0, Pos +4, TCShopKeeper +0x10, AllowExistingShopper +0x14;
//   CTCBase::ParentThing +4. CThing (retail, 0x10 below the PDB): thing-type byte +0x10, bitset +0x20, map +0x44.
struct CTCBase { void* vfptr; struct CThing* ParentThing; };
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}
#include <math.h>
#pragma intrinsic(sqrt)
__forceinline float Length(const C3DVector& v)
{
    return (float)sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char  _pad00[0x10];
    unsigned char  ThingType;                           // +0x10
    unsigned char  _pad11[0x20 - 0x11];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    long GetCreatureType() const;                       // 0x00662000
};

struct CVillage {
    bool IsVillagerPerformingVillageTask(const CThing* thing) const;   // 0x00659104
};
struct CTCVillageMember : CTCBase {
    CVillage* GetPVillage() const;                      // 0x007EE540
};
struct CTCShopKeeper : CTCBase {
    bool HasBeenCustomer(const CThing* thing) const;    // 0x00798DD0
};
struct CIntelligentPointer {
    void* Ptr;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CTCWithItem : CTCBase {                          // component 0x75
    unsigned char _pad08[4];
    CIntelligentPointer Item;                           // +0xc
};
struct COpinionManager {
    long GetOpinionAttitudeToPlayer(const CThing* thing) const;   // 0x00701840
};
struct CGameSub { unsigned char _pad[0x74]; COpinionManager* Opinions; };
struct CGameTop { unsigned char _pad[0x24]; CGameSub* Sub; };
extern CGameTop* g_013B8A1C;

struct CThingFilter_IsPotentialCustomer {
    CThing*        ShopperContainer;                    // +0x00
    C3DVector      Pos;                                 // +0x04
    CTCShopKeeper* TCShopKeeper;                        // +0x10
    bool           AllowExistingShopper;                // +0x14
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsPotentialCustomer::operator()(const CThing& thing) const
{
    if ((thing.ThingType == 1 || thing.ThingType == 2)
        && ((const CThingCreatureBase&)thing).GetCreatureType()
        && &thing != TCShopKeeper->ParentThing
        && !thing.HasTC(0x90) && !thing.HasTC(0xb4)) {
        if (thing.HasTC(0x23)) {
            const CTCVillageMember* member = (const CTCVillageMember*)thing.GetTC(0x23);
            if (member->GetPVillage() && thing.ThingType == 1 && member->GetPVillage()->IsVillagerPerformingVillageTask(&thing))
                return false;
        }
        if (!TCShopKeeper->HasBeenCustomer(&thing) || AllowExistingShopper) {
            if (thing.ThingType == 2) {
                long attitude = g_013B8A1C->Sub->Opinions->GetOpinionAttitudeToPlayer(TCShopKeeper->ParentThing);
                if (attitude >= 3 && (attitude <= 4 || attitude == 8))
                    return false;
            }
            if (thing.HasTC(0x75)) {
                const CTCWithItem* holder = (const CTCWithItem*)thing.GetTC(0x75);
                if (holder->Item.GetPItem() != ShopperContainer)
                    return false;
            }
            if (ShopperContainer || Length(Pos - thing.GetPosition()) < 6.0f)
                return true;
        }
    }
    return false;
}


CTCBase* g_compFor[0x100];
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{ const SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static C3DVector g_pos;
const C3DVector& CThing::GetPosition() const { return g_pos; }
static long g_creatureType = 1;
long CThingCreatureBase::GetCreatureType() const { return g_creatureType; }
static bool g_busy = false; static CVillage g_village; static CVillage* g_pv = 0;
bool CVillage::IsVillagerPerformingVillageTask(const CThing*) const { return g_busy; }
CVillage* CTCVillageMember::GetPVillage() const { return g_pv; }
static bool g_was = false;
bool CTCShopKeeper::HasBeenCustomer(const CThing*) const { return g_was; }
CThing* CIntelligentPointer::GetPItem() const { return (CThing*)Ptr; }
static long g_att = 0;
long COpinionManager::GetOpinionAttitudeToPlayer(const CThing*) const { return g_att; }
static COpinionManager g_om; static CGameSub g_sub; static CGameTop g_top;
CGameTop* g_013B8A1C = &g_top;
#define CHECK(c) if (!(c)) { std::printf("00799e30_TEST FAIL line %d\n", __LINE__); return 1; }

int main()
{
    g_sub.Opinions = &g_om; g_top.Sub = &g_sub;
    static CThing keeperThing; static CTCShopKeeper keeper; keeper.ParentThing = &keeperThing;
    static CTCVillageMember vm; static CTCWithItem holder; static CThing container;
    static CInterfaceMap::SEntry entries[3];
    entries[0].type = 0x23; entries[0].component = &vm;
    entries[1].type = 0x75; entries[1].component = &holder;
    entries[2].type = 0x7fffffff; entries[2].component = 0;
    static CThingCreatureBase t;
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + 2;
    CThingFilter_IsPotentialCustomer f; f.ShopperContainer = 0; f.Pos = C3DVector(0.0f, 0.0f, 0.0f);
    f.TCShopKeeper = &keeper; f.AllowExistingShopper = false;
    g_pos = C3DVector(3.0f, 4.0f, 0.0f);                       // distance 5
    t.ThingType = 1;
    CHECK(f(t));
    t.ThingType = 3; CHECK(!f(t)); t.ThingType = 1;
    g_creatureType = 0; CHECK(!f(t)); g_creatureType = 1;
    CHECK(!f(keeperThing) || keeperThing.ThingType == 0);
    keeperThing.ThingType = 1; CHECK(!f(keeperThing)); keeperThing.ThingType = 0;
    t.m_presentBits[4] = 0x10000; CHECK(!f(t)); t.m_presentBits[4] = 0;
    t.m_presentBits[5] = 0x100000; CHECK(!f(t)); t.m_presentBits[5] = 0;
    t.m_presentBits[1] = 8; g_pv = &g_village; g_busy = true;
    CHECK(!f(t));                                              // villager busy in village
    t.ThingType = 2; CHECK(f(t)); t.ThingType = 1;             // hero: village task ignored
    g_pv = 0; CHECK(f(t)); g_pv = &g_village; g_busy = false; CHECK(f(t));
    g_was = true; CHECK(!f(t)); f.AllowExistingShopper = true; CHECK(f(t)); g_was = false;
    t.ThingType = 2;
    g_att = 3; CHECK(!f(t)); g_att = 4; CHECK(!f(t)); g_att = 8; CHECK(!f(t));
    g_att = 5; CHECK(f(t)); g_att = 2; CHECK(f(t));
    t.ThingType = 1; g_att = 3; CHECK(f(t));                   // villagers skip the attitude test
    t.m_presentBits[3] = 0x200000; holder.Item.Ptr = &container;
    CHECK(!f(t));                                              // holds an item that is not the container
    f.ShopperContainer = &container; CHECK(f(t));
    g_pos = C3DVector(30.0f, 40.0f, 0.0f); CHECK(f(t));        // container: no distance test
    f.ShopperContainer = 0; t.m_presentBits[3] = 0; CHECK(!f(t));   // 50 away
    g_pos = C3DVector(3.0f, 4.0f, 4.0f); CHECK(!f(t));         // 6.4 away
    g_pos = C3DVector(1.0f, 2.0f, 5.0f); CHECK(f(t));          // 5.48 away
    std::printf("00799e30_TEST PASS\n");
    return 0;
}
