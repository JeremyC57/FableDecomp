// Behaviour test for 005c3947: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCInventoryWeapons::CTCInventoryWeapons(CThing* thing) -- construct the weapons inventory: the inventory base, the screen base at +0x130, both active-weapon pointers, the primitive handle vector, the menu item map and the entries list
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif

struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                 // +0x04
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    CIntelligentPointer_CThing(CThing* thing);       // 0x004A96F0
};

struct SAllocator { SAllocator() { } };
struct SComponentListNode;
struct CComponentList {                              // std::list<NUISystem::CComponent*> (head pointer)
    SComponentListNode* m_pNode;
    CComponentList(const SAllocator& allocator = SAllocator());   // 0x0042ABAF
};
struct CComponentItemMap {                           // std::map<NUISystem::CComponent*, long> (12 bytes)
    void* _M_header; long _M_node_count; long _M_compare;
    CComponentItemMap();                             // 0x004DF81D
};
struct CEnginePrimitiveHandleVector {                // std::vector<CEnginePrimitiveHandle> (12 bytes)
    void* m_first; void* m_last; void* m_end;
    CEnginePrimitiveHandleVector(unsigned long count);   // 0x00485D30
};

namespace NInventory {
class CTCInventoryBase {                             // retail 0x130 bytes
public:
    CTCInventoryBase(CThing* thing);                 // 0x005BFF07
    virtual ~CTCInventoryBase();
    unsigned char _pad04[0x130 - 0x04];
};
}
namespace NHeroInformationScreens {
class CBase {                                        // secondary base, vtable stored by its out-of-line ctor
public:
    CBase();                                         // 0x0066E734
    virtual void v00();
};
}

struct CDefIndex {                                   // a def table index
    long Index;
    CDefIndex(long index) : Index(index) {}
};

struct NUISystemComponent;
class CTCInventoryWeapons : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    CIntelligentPointer_CThing   PActiveMeleeWeapon;            // +0x134
    CDefIndex                    ActiveMeleeWeaponDefIndex;     // +0x13C
    bool                         MeleeWeaponCarriedOnLoad;      // +0x140
    CIntelligentPointer_CThing   PActiveRangedWeapon;           // +0x141
    CDefIndex                    ActiveRangedWeaponDefIndex;    // +0x14C
    bool                         RangedWeaponCarriedOnLoad;     // +0x150
    CEnginePrimitiveHandleVector ActiveWeaponPrimitiveHandles;  // +0x154
    long                         ConfiscatedActiveMeleeWeaponDefIndex;    // +0x160
    long                         ConfiscatedActiveRangedWeaponDefIndex;   // +0x164
    long                         PreviouslyWieldedMeleeWeaponDefIndex;    // +0x168
    long                         PreviouslyWieldedRangedWeaponDefIndex;   // +0x16C
    CComponentItemMap            WeaponMenuItemMap;             // +0x170
    NUISystemComponent*          WeaponsList;                   // +0x17C
    NUISystemComponent*          PAssignedRangedIcon;           // +0x180
    NUISystemComponent*          PAssignedMeleeIcon;            // +0x184
    NUISystemComponent*          PAssignedRangedMenuEntry;      // +0x188
    NUISystemComponent*          PAssignedMeleeMenuEntry;       // +0x18C
    NUISystemComponent*          PRangedList;                   // +0x190
    NUISystemComponent*          PMeleeList;                    // +0x194
    CComponentList               AvailableWeaponsEntries;       // +0x198
    CTCInventoryWeapons(CThing* thing);
    virtual ~CTCInventoryWeapons();
    virtual void v00();
};

CTCInventoryWeapons::CTCInventoryWeapons(CThing* thing)
    : NInventory::CTCInventoryBase(thing),
      PActiveMeleeWeapon(0), ActiveMeleeWeaponDefIndex(0), MeleeWeaponCarriedOnLoad(false),
      PActiveRangedWeapon(0), ActiveRangedWeaponDefIndex(0), RangedWeaponCarriedOnLoad(false),
      ActiveWeaponPrimitiveHandles(2),
      ConfiscatedActiveMeleeWeaponDefIndex(0), ConfiscatedActiveRangedWeaponDefIndex(0),
      PreviouslyWieldedMeleeWeaponDefIndex(0), PreviouslyWieldedRangedWeaponDefIndex(0),
      WeaponsList(0)
{
}

// ---- stubs and driver ----
#include <new>
static CThing* g_baseThing; static int g_order; static int g_baseAt, g_screenAt, g_vectorAt, g_mapAt, g_listAt;
static unsigned long g_count; static const SAllocator* g_alloc;
void CBaseIntelligentPointer::v00() {}
CIntelligentPointer_CThing::CIntelligentPointer_CThing(CThing* thing) { m_pItem = thing; }
CComponentList::CComponentList(const SAllocator& allocator) : m_pNode(0) { g_alloc = &allocator; g_listAt = ++g_order; }
CComponentItemMap::CComponentItemMap() : _M_header(0), _M_node_count(0), _M_compare(0) { g_mapAt = ++g_order; }
CEnginePrimitiveHandleVector::CEnginePrimitiveHandleVector(unsigned long count) : m_first(0), m_last(0), m_end(0) { g_count = count; g_vectorAt = ++g_order; }
NInventory::CTCInventoryBase::CTCInventoryBase(CThing* thing) { g_baseThing = thing; g_baseAt = ++g_order; }
NInventory::CTCInventoryBase::~CTCInventoryBase() {}
NHeroInformationScreens::CBase::CBase() { g_screenAt = ++g_order; }
void NHeroInformationScreens::CBase::v00() {}
CTCInventoryWeapons::~CTCInventoryWeapons() {}
void CTCInventoryWeapons::v00() {}

static double g_buffer[(sizeof(CTCInventoryWeapons) + 7) / 8];

int main()
{
    std::memset(g_buffer, 0xcd, sizeof(g_buffer));
    CThing* thing = (CThing*)0x1234;
    CTCInventoryWeapons* w = new (g_buffer) CTCInventoryWeapons(thing);
    if (g_baseThing != thing || g_baseAt != 1 || g_screenAt != 2 || g_vectorAt != 3 || g_mapAt != 4 || g_listAt != 5)
        { std::printf("005c3947_TEST FAIL (bases/order)\n"); return 1; }
    if (g_count != 2 || !g_alloc) { std::printf("005c3947_TEST FAIL (vector/list args)\n"); return 1; }
    if (w->PActiveMeleeWeapon.m_pItem || w->PActiveRangedWeapon.m_pItem || w->ActiveMeleeWeaponDefIndex.Index
        || w->ActiveRangedWeaponDefIndex.Index || w->MeleeWeaponCarriedOnLoad || w->RangedWeaponCarriedOnLoad)
        { std::printf("005c3947_TEST FAIL (active weapons)\n"); return 1; }
    if (w->ConfiscatedActiveMeleeWeaponDefIndex || w->ConfiscatedActiveRangedWeaponDefIndex
        || w->PreviouslyWieldedMeleeWeaponDefIndex || w->PreviouslyWieldedRangedWeaponDefIndex || w->WeaponsList)
        { std::printf("005c3947_TEST FAIL (members)\n"); return 1; }
    // the other component pointers are left for the screen set-up to fill
    unsigned char untouched;
    std::memcpy(&untouched, &w->PAssignedRangedIcon, 1);
    if (untouched != 0xcd) { std::printf("005c3947_TEST FAIL (untouched)\n"); return 1; }
    std::printf("005c3947_TEST PASS\n");
    return 0;
}