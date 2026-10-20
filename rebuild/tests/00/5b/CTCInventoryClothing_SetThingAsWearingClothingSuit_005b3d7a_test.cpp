// Behaviour test for 005b3d7a: body copied from src/005b3d7a.cpp (CTCInventoryClothing::SetThingAsWearingClothingSuit), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CPresenceBits {
    unsigned long bits[9];
    bool test(long bit) { return (bits[bit >> 5] & (1UL << (bit & 31))) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;
    unsigned char _pad_0x21[0x23 + 0x20 - 0x24];
    CInterfaceMap m_interfaces;
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
enum { APPEARANCE_MODIFIER_CLOTHING = 2 };
static const char* g_log[32]; static long g_logArg[32]; static int g_logN = 0;
static void Log(const char* s, long a) { g_log[g_logN] = s; g_logArg[g_logN++] = a; }
struct CTCHeroAttachableAppearanceModifiers {
    void RemoveAllAppearanceModifiers(long type) { Log("remove", type); }
    void AddAppearanceModifierWithReplace(long defIndex) { Log("add", defIndex); }
    void EditFrameUpdate() { Log("update", 0); }
};
struct SNode {
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   first;
    long   second;
};
static SNode g_nodes[3]; static SNode g_header;
SNode* __cdecl _M_increment(SNode* node) { for (int i = 0; i < 3; ++i) if (node == &g_nodes[i]) return i + 1 < 3 ? &g_nodes[i + 1] : &g_header; return &g_header; }
struct CSuitPartMap {
    SNode* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    SNode* begin() { return _M_header->_M_left; }
    SNode* end() { return _M_header; }
};
static int g_destroyed = 0;
struct CDefPointeeBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CHeroSuitDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    CSuitPartMap  SuitParts;
};
static CHeroSuitDef* g_suit = 0;
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CHeroSuitDef>* out);
};
bool CDefClassBase::GetSubDef(CDefPointer<CHeroSuitDef>* out)
{
    if (!g_suit) return false;
    ++g_suit->m_refCount; out->Object = g_suit; return true;
}
static CDefClassBase* g_classDef = 0; static long g_askedIndex = 0;
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);
};
bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    g_askedIndex = index;
    if (!g_classDef) return false;
    ++g_classDef->m_refCount; out->Object = g_classDef; return true;
}
static CDefinitionManager g_manager; static long g_counts[64];
namespace NInventory {
struct CTCInventoryBase {
    unsigned char _pad00[0x130];
    CDefinitionManager* GetDefinitions() { return &g_manager; }
    long GetNumberOfItemsOfTypeInInventory(long itemType) { Log("count", itemType); return g_counts[itemType]; }
};
struct CTCInventoryClothing : CTCInventoryBase {
    void SetThingAsWearingClothingSuit(CThing* thing, long suitDefIndex);
};
void CTCInventoryClothing::SetThingAsWearingClothingSuit(CThing* thing, long suitDefIndex)
{
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (thing->GetInterface(&modifiers, 0x5E)) {
        modifiers->RemoveAllAppearanceModifiers(APPEARANCE_MODIFIER_CLOTHING);
        CDefPointer<CDefClassBase> def;
        def.Object = 0;
        GetDefinitions()->GetDef(suitDefIndex, &def);
        CDefPointer<CHeroSuitDef> suit;
        suit.Object = 0;
        def->GetSubDef(&suit);
        CHeroSuitDef* suitDef = suit.Object;
        for (SNode* it = suitDef->SuitParts.begin(); it != suitDef->SuitParts.end(); it = _M_increment(it)) {
            if (GetNumberOfItemsOfTypeInInventory(it->second) > 0)
                modifiers->AddAppearanceModifierWithReplace(it->second);
        }
        modifiers->EditFrameUpdate();
        if (--suitDef->m_refCount == 0)
            suitDef->Destroy();
        CDefClassBase* owner = def.Object;
        if (owner && --owner->m_refCount == 0)
            owner->Destroy();
    }
}
}
int main()
{
    static CThing thing; static CInterfaceMap::SEntry entries[2]; static CTCHeroAttachableAppearanceModifiers mods; static CHeroSuitDef suit; static CDefClassBase classDef;
    memset(&thing, 0, sizeof(thing)); entries[0].type = 0x10; entries[1].type = 0x5E; entries[1].component = &mods;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2; thing.m_present.bits[0x5E >> 5] |= 1UL << (0x5E & 31);
    g_header._M_left = &g_nodes[0]; for (int i = 0; i < 3; ++i) { g_nodes[i].first = i; g_nodes[i].second = 20 + i; }
    suit.m_refCount = 1; suit.SuitParts._M_header = &g_header; classDef.m_refCount = 1; g_suit = &suit; g_classDef = &classDef;
    g_counts[20] = 1; g_counts[21] = 0; g_counts[22] = 3;
    NInventory::CTCInventoryClothing inv; memset(&inv, 0, sizeof(inv));
    inv.SetThingAsWearingClothingSuit(&thing, 42);
    CHECK(g_askedIndex == 42); CHECK(suit.m_refCount == 1 && classDef.m_refCount == 1 && g_destroyed == 0);
    CHECK(g_logN == 7); CHECK(strcmp(g_log[0], "remove") == 0 && g_logArg[0] == 2);
    CHECK(strcmp(g_log[1], "count") == 0 && g_logArg[1] == 20); CHECK(strcmp(g_log[2], "add") == 0 && g_logArg[2] == 20);
    CHECK(strcmp(g_log[3], "count") == 0 && g_logArg[3] == 21); CHECK(strcmp(g_log[4], "count") == 0 && g_logArg[4] == 22);
    CHECK(strcmp(g_log[5], "add") == 0 && g_logArg[5] == 22); CHECK(strcmp(g_log[6], "update") == 0);
    // defs held only by the lookups are destroyed
    suit.m_refCount = 0; classDef.m_refCount = 0; g_logN = 0;
    inv.SetThingAsWearingClothingSuit(&thing, 42); CHECK(g_destroyed == 2);
    // no modifiers component: nothing happens
    thing.m_present.bits[0x5E >> 5] = 0; g_logN = 0; g_askedIndex = 0;
    inv.SetThingAsWearingClothingSuit(&thing, 42); CHECK(g_logN == 0 && g_askedIndex == 0);
    if (fails == 0) printf("005b3d7a_TEST PASS\n"); else printf("005b3d7a_TEST FAIL\n");
    return fails;
}