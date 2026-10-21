// Behaviour test for 006bf9e0: src/006bf9e0.cpp copied verbatim; the def lookups (through the same-TU stand-ins), the
// building-contents list and the thing calls stubbed so the scripted flag, the max dress level, the persisted marking
// of the house and of its live contents with interface 0x26, and the def release are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCBuyableHouse::OnCreate() -- take the house's buyable-house sub-def from the parent thing's def; note whether the house is scripted; for a buyable house set the top dress level from the price table, persist the house in the game state, and persist every living thing inside it that has interface 0x26
// Retry 10b (accessor sweep, FIXES B-4/B-5): the parent thing is read through an inline accessor; the plain member read gives the parked register difference.
//   tried: direct test, early return, a `const unsigned short&` accessor for the push, an /Os probe (frame-pointer + `and` stores; wrong) | next: an inline CThing accessor returning the index by value for the push, or the test through a bool accessor
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BF9E0, 301 bytes (+3 int3 padding), `ret`. CTCBase vtable slot 4 (the catalog's
//   "vfunc_4"; slot 4 is OnCreate, as the landed CTCHeroSpecialMovement::OnCreate 0x007E0910).
// Callees: CTCBase::GetDefinitions() 0x006869D0, CDefinitionManager::GetDef(long, CDefPointer*) 0x0042AF3C and
//   CDefClassBase::GetSubDef<CBuyableHouseDef>(CDefPointer*) 0x006C1B00 (both manifest labels propagated instantiations;
//   same-TU stand-in bodies as the landed 0x007E0910), CThing::SetAsGameStatePersisted(bool) 0x004C7880,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (non-const), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   the def's slot 1 on release.
// Shape: the def index is re-read for the call in retail (no local; ours caches it); the GetDef out pointer starts null in a frame word and is
//   released by hand at the end (`dec [ebx+4] ; jne`); the scripted flag is `def && def->IsScripted ? true : false` (a bare `&&` materialises in ecx);
//   the contents component (0x21) is the inline presence test + the ternary entry pick; the loop re-fetches the item
//   for each test (null, dead bit +0x91, presence bit 0x26).
// Retail layout: engine/CTCBuyableHouse.h (PDef +0x0c, IsScripted +0x10, MaxDressLevel +0x20); CBuyableHouseDef (retail
//   12-byte vectors, PDB offsets less 0x04 past the first vector): IsBuyable +0x25, Price +0x28, IsScripted +0x48,
//   IsBuyableWithoutSignpost +0x49; CTCBuildingContents (0x21) VC6 list head +0x0c.
struct CDefPointeeBase {
    virtual void Destroy();
    virtual void Delete();                      // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return (T*)Object; }
};
template <class T> struct CArray {              // retail std::vector (12 bytes)
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
};
struct CBuyableHouseDef : CDefPointeeBase {
    unsigned char _pad08[0x25 - 0x08];
    bool          IsBuyable;                    // +0x25
    bool          InitiallyEmpty;               // +0x26
    unsigned char _pad27;
    CArray<long>  Price;                        // +0x28
    CArray<long>  Rent;                         // +0x34
    long          DaysBetweenRentPayments;      // +0x40
    long          MaxRentBags;                  // +0x44
    bool          IsScripted;                   // +0x48
    bool          IsBuyableWithoutSignpost;     // +0x49
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CBuyableHouseDef>* out);   // 0x006C1B00
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CBuyableHouseDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager {
public:
    bool GetDef(long index, CDefPointer<const CDefClassBase>* out);     // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                    // +0x8c
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  m_stateFlags;                // +0x91 (bit 0: dead)
    void SetAsGameStatePersisted(bool persisted);   // 0x004C7880
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThing>: vptr + item
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
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
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    CThing* GetParentThing() const { return ParentThing; }
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CDefinitionManager* GetDefinitions();       // 0x006869D0
};
class CTCBuyableHouse : public CTCBase {
public:
    CDefPointer<const CBuyableHouseDef> PDef;   // +0x0c
    bool          IsScripted;                   // +0x10
    bool          OwnedByPlayer;                // +0x11
    bool          Rented;                       // +0x12
    long          DaySomeoneWillMoveIn;         // +0x14
    long          DayNextRentIsDue;             // +0x18
    long          CurrentDressLevel;            // +0x1c
    long          MaxDressLevel;                // +0x20
    virtual void OnCreate();
};

void CTCBuyableHouse::OnCreate()
{
    if (GetParentThing()->DefIndex == 0)
        return;
    {
        CDefPointer<const CDefClassBase> out;
        GetDefinitions()->GetDef(GetParentThing()->DefIndex, &out);
        CDefClassBase* def = (CDefClassBase*)out.Object;
        def->GetSubDef(&PDef);
        IsScripted = PDef.Object && PDef->IsScripted ? true : false;
        if (PDef.Object && (PDef->IsBuyable || PDef->IsBuyableWithoutSignpost)) {
            MaxDressLevel = PDef->Price.size() - 1;
            GetParentThing()->SetAsGameStatePersisted(true);
            CThing* parent = ParentThing;
            if (parent->HasTC(0x21)) {
                CTCBuildingContents* contents = parent->GetTC<CTCBuildingContents>(0x21);
                for (SThingListNode* node = contents->ThingsHead->next; node != contents->ThingsHead; node = node->next) {
                    if (node->value.GetPItem() && !(node->value.GetPItem()->m_stateFlags & 1) && node->value.GetPItem()->HasTC(0x26))
                        node->value.GetPItem()->SetAsGameStatePersisted(true);
                }
            }
        }
        if (def && --def->m_refCount == 0)
            def->Delete();
    }
}
// ---- end source ----

static int g_deletes = 0;
void CDefPointeeBase::Destroy() {}
void CDefPointeeBase::Delete() { ++g_deletes; }
static unsigned char g_classRaw[sizeof(CDefClassBase)]; static CBuyableHouseDef g_house;
static bool g_haveHouse = true;
void* FindDef_CDefinitionManager_GetDef_0() { ((CDefClassBase*)g_classRaw)->m_refCount++; return g_classRaw; }
void* FindDef_CDefClassBase_GetSubDef_1() { return g_haveHouse ? &g_house : 0; }
static CDefinitionManager g_dm;
CDefinitionManager* CTCBase::GetDefinitions() { return &g_dm; }
CTCBase::~CTCBase() {}
static CThing* g_persisted[8]; static int g_nPersisted = 0;
void CThing::SetAsGameStatePersisted(bool p) { if (p) g_persisted[g_nPersisted++] = this; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static CTCBuildingContents g_contents;
static CInterfaceMap::SEntry g_entries[1] = { { 0x21, (CTCBase*)&g_contents } };
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return *type <= 0x21 ? first : last; }
int main()
{
    ((CDefClassBase*)g_classRaw)->m_refCount = 1;
    long prices[3] = { 1, 2, 3 };
    memset(&g_house, 0, sizeof(g_house)); g_house.IsBuyable = true; g_house.IsScripted = true;
    g_house.Price.m_pFirst = prices; g_house.Price.m_pLast = prices + 3;
    static CThing parent, a, b, c; memset(&parent, 0, sizeof(parent)); memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c));
    parent.DefIndex = 7; parent.m_interfaces.first = g_entries; parent.m_interfaces.last = g_entries + 1;
    parent.m_presentBits[0x21 >> 5] |= 1UL << (0x21 & 31);
    a.m_presentBits[0x26 >> 5] |= 1UL << (0x26 & 31);                     // live, has 0x26
    b.m_presentBits[0x26 >> 5] |= 1UL << (0x26 & 31); b.m_stateFlags = 1; // dead
    static SThingListNode head, n1, n2, n3;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &head;
    n1.value.m_pItem = &a; n2.value.m_pItem = &b; n3.value.m_pItem = &c;     // c lacks 0x26
    g_contents.ThingsHead = &head;
    static unsigned char raw[sizeof(CTCBuyableHouse)]; memset(raw, 0, sizeof(raw));
    CTCBuyableHouse* h = (CTCBuyableHouse*)raw; h->ParentThing = &parent;
    h->CTCBuyableHouse::OnCreate();
    CHECK(h->PDef.Object == &g_house && h->IsScripted == true && h->MaxDressLevel == 2);
    CHECK(g_nPersisted == 2 && g_persisted[0] == &parent && g_persisted[1] == &a);
    CHECK(((CDefClassBase*)g_classRaw)->m_refCount == 1 && g_deletes == 0);
    // not buyable: no persistence, flag still read
    g_house.IsBuyable = false; g_house.IsScripted = false; g_nPersisted = 0;
    h->CTCBuyableHouse::OnCreate();
    CHECK(h->IsScripted == false && g_nPersisted == 0);
    // no def index: nothing happens
    parent.DefIndex = 0; h->IsScripted = true;
    h->CTCBuyableHouse::OnCreate(); CHECK(h->IsScripted == true);
    if (fails == 0) printf("006bf9e0_TEST PASS\n"); else printf("006bf9e0_TEST FAIL\n");
    return fails;
}