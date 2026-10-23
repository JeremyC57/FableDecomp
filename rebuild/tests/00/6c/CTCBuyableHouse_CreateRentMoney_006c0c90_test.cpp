// 006c0c90 behaviour test: CTCBuyableHouse::CreateRentMoney (control flow + data of the final.cpp body)
#ifndef _MSC_VER
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>
struct CTCBase;
struct CThing;
struct C3DVector { float x, y, z; };
struct CCharString {
    const char* Text;
    CCharString(const char* text, long = -1) : Text(text) {}
    ~CCharString() {}
};
struct CTCParamsBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* lo = first;
        long count = last - first;
        while (count > 0) {
            long half = count >> 1;
            SEntry* mid = lo + half;
            if (mid->type < *type) { lo = mid + 1; count -= half + 1; } else count = half;
        }
        return lo;
    }
};
struct CTCOwnedEntity { CThing* Owner; void SetPOwner(CThing* owner) { Owner = owner; } };
struct CTCGold { long Gold; };
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry Entries[2];
    CTCOwnedEntity Owned;
    CTCGold GoldTC;
    bool HasGold;
    long DefIndex;
    C3DVector Pos;
    CTCBase* AddTC(const CCharString& name, bool, CTCParamsBase*)
    {
        if (strcmp(name.Text, "CTCOwnedEntity") == 0) {
            m_presentBits[0x33 >> 5] |= 1UL << (0x33 & 31);
            Entries[0].type = 0x33; Entries[0].component = (CTCBase*)&Owned;
        }
        return 0;
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingObject : CThing {
    static CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long player, void*, unsigned long, const CCharString&);
};
static CThingObject g_bags[16];
static int g_bagCount;
static bool g_bagsHaveGold = true;
CThingObject* __fastcall CThingObject::Create(long defIndex, const C3DVector* position, long player, void*, unsigned long, const CCharString&)
{
    CThingObject* b = &g_bags[g_bagCount++];
    memset(b, 0, sizeof(*b));
    b->DefIndex = defIndex; b->Pos = *position; (void)player;
    b->Entries[1].type = 0xcc; b->Entries[1].component = (CTCBase*)&b->GoldTC;
    if (g_bagsHaveGold) b->m_presentBits[0xcc >> 5] |= 1UL << (0xcc & 31);
    b->m_interfaces.first = b->Entries; b->m_interfaces.last = b->Entries + 2;
    b->GoldTC.Gold = -1;
    return b;
}
static long g_goldIndex = 7;
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name) { return strcmp(name.Text, "OBJECT_GOLDBAG_SMALL_WITH_COINS") == 0 ? g_goldIndex : 0; }
};
static CDefinitionManager g_defs;
struct CGameTime { long Day; };
struct CWorld { CGameTime* Time; };
static CGameTime g_time; static CWorld g_world = { &g_time };
struct CBuyableHouseDef {
    long* Rent_First;
    long* Rent_Last;
    long* Rent_End;
    long  DaysBetweenRentPayments;
    long  MaxRentBags;
};
template<class T> inline const T& min_of(const T& a, const T& b) { return b < a ? b : a; }
static long g_existingBags, g_destroyed, g_trophies;
static int g_posFailIndex = -1;
struct CTCBuyableHouse {
    CThing*           ParentThing;
    CBuyableHouseDef* PDef;
    bool              Rented;
    long              DayNextRentIsDue;
    long              CurrentDressLevel;
    long              VirtualMoneyBags;
    CWorld* GetWorld() const { return &g_world; }
    CDefinitionManager* GetDefinitions() const { return &g_defs; }
    long CountMoneyBags() const { return g_existingBags; }
    void DestroyMoneyBags(bool) { g_destroyed += g_existingBags; g_existingBags = 0; }
    bool GetRentMoneyPos(C3DVector* pos, long index, long count)
    {
        if (index == g_posFailIndex) return false;
        pos->x = (float)index; pos->y = (float)count; pos->z = 0; return true;
    }
    long GetValueOfTrophies() const { return g_trophies; }
    CBuyableHouseDef* GetDef() const { return PDef; }
    long GetNumberOfRentBagsDue() const
    {
        if (PDef && Rented && GetWorld()->Time->Day >= DayNextRentIsDue) {
            long due = (GetWorld()->Time->Day - DayNextRentIsDue) / PDef->DaysBetweenRentPayments + 1;
            if (due > 0)
                return due;
        }
        return 0;
    }
    long GetRentPerBag() const
    {
        const long* rents = PDef->Rent_First;
        return (long)(GetValueOfTrophies() * 0.1f + rents[CurrentDressLevel]);
    }
    void CreateRentMoney();
};
void CTCBuyableHouse::CreateRentMoney()
{
    VirtualMoneyBags += CountMoneyBags();
    DestroyMoneyBags(false);
    VirtualMoneyBags += GetNumberOfRentBagsDue();
    DayNextRentIsDue += PDef->DaysBetweenRentPayments * GetNumberOfRentBagsDue();
    VirtualMoneyBags = min_of(VirtualMoneyBags, PDef->MaxRentBags);
    if (PDef->Rent_Last - PDef->Rent_First == 0)
        return;
    for (long i = 0; i < VirtualMoneyBags; ++i) {
        C3DVector pos;
        if (!GetRentMoneyPos(&pos, i, GetDef()->MaxRentBags))
            continue;
        long index;
        {
            CCharString name("OBJECT_GOLDBAG_SMALL_WITH_COINS");
            index = GetDefinitions()->GetDefGlobalIndexFromName(name);
        }
        if (index <= 0)
            continue;
        CThingObject* bag = CThingObject::Create(index, &pos, 5, 0, 0, "");
        bag->AddTC("CTCOwnedEntity", false, 0);
        CTCOwnedEntity* owned;
        bag->GetInterface(&owned, 0x33);
        owned->SetPOwner(ParentThing);
        CTCGold* gold;
        bag->GetInterface(&gold, 0xcc);
        if (gold)
            gold->Gold = GetRentPerBag();
    }
    VirtualMoneyBags = 0;
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }
int main()
{
    long rents[3] = { 10, 20, 30 };
    CBuyableHouseDef def = { rents, rents + 3, rents + 3, 5, 4 };
    CThing parent; memset(&parent, 0, sizeof(parent));
    CTCBuyableHouse h; h.ParentThing = &parent; h.PDef = &def; h.Rented = true; h.DayNextRentIsDue = 10; h.CurrentDressLevel = 1; h.VirtualMoneyBags = 0;
    // day 20, due day 10, every 5 days: (20-10)/5+1 = 3 bags due; 1 existing bag -> 4 bags, next due = 10 + 5*3 = 25
    g_time.Day = 20; g_existingBags = 1; g_trophies = 55;
    h.CreateRentMoney();
    Check("bags", g_bagCount == 4 && h.VirtualMoneyBags == 0 && g_destroyed == 1);
    Check("due day", h.DayNextRentIsDue == 25);
    Check("bag data", g_bags[0].DefIndex == 7 && g_bags[3].Pos.x == 3.0f && g_bags[3].Pos.y == 4.0f);
    Check("owner", g_bags[0].Owned.Owner == &parent && g_bags[3].Owned.Owner == &parent);
    Check("gold", g_bags[2].GoldTC.Gold == 25);   // 55*0.1 + 20 = 25.5 -> 25
    // capped at MaxRentBags (4), one position fails
    g_bagCount = 0; g_time.Day = 60; g_existingBags = 2; g_posFailIndex = 1;
    h.CreateRentMoney();
    Check("cap + skip", g_bagCount == 3 && h.DayNextRentIsDue == 25 + 5 * 8 && h.VirtualMoneyBags == 0);
    g_posFailIndex = -1;
    // not rented: only existing bags (gold uses dress level 1)
    g_bagCount = 0; h.Rented = false; g_existingBags = 2;
    h.CreateRentMoney();
    Check("not rented", g_bagCount == 2 && g_bags[0].GoldTC.Gold == 25 && h.DayNextRentIsDue == 65);
    g_bagsHaveGold = true;
    // unknown def index: no bags created, virtual count still cleared
    g_bagCount = 0; g_goldIndex = 0; g_existingBags = 3;
    h.CreateRentMoney();
    Check("no def", g_bagCount == 0 && h.VirtualMoneyBags == 0);
    g_goldIndex = 7;
    // empty rent table: returns early keeping the virtual count
    def.Rent_Last = rents; g_existingBags = 2;
    h.CreateRentMoney();
    Check("empty rent", g_bagCount == 0 && h.VirtualMoneyBags == 2);
    if (g_fail) return 1;
    printf("006c0c90_TEST PASS\n");
    return 0;
}