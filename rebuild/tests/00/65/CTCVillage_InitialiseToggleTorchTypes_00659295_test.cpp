// Behaviour test for 00659295: body copied from src/00659295.cpp (CTCVillage::InitialiseToggleTorchTypes), the bitset / light callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
static int g_setActive = 0; static bool g_lastActive = false; static void* g_activeThis = 0;
class CTCLight { public:
    unsigned char _pad[0x1C];
    bool          Active;
    void SetActive(bool active) { ++g_setActive; g_lastActive = active; g_activeThis = this; Active = active; }
    void ToggleActive() { SetActive(!Active); }
};
static int g_tests = 0; static bool g_bit = false;
struct CBitset { bool test(unsigned long position) const { ++g_tests; return g_bit && position == 0x27; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThingObject { public:
    unsigned char _pad0[0x20];
    CBitset       TypeBits;
    unsigned char _pad24[0x20];
    CInterfaceMap Interfaces;
    template<class T> __forceinline T* PeekTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CTCVillage { public: void InitialiseToggleTorchTypes(CThingObject& object); };
void CTCVillage::InitialiseToggleTorchTypes(CThingObject& object)
{
    if (object.TypeBits.test(0x27)) {
        object.PeekTC<CTCLight>(0x27)->ToggleActive();
    }
}
int main()
{
    CThingObject object; memset(&object, 0, sizeof(object)); CTCLight light; memset(&light, 0, sizeof(light));
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x27, (CTCBase*)&light } };
    object.Interfaces.first = entries; object.Interfaces.last = entries + 2;
    CTCVillage village;
    // 1. not a light: nothing toggled
    village.InitialiseToggleTorchTypes(object); CHECK(g_tests == 1 && g_setActive == 0);
    // 2. a light: toggled off -> on, then on -> off
    g_bit = true; village.InitialiseToggleTorchTypes(object); CHECK(g_setActive == 1 && g_lastActive == true && g_activeThis == &light && light.Active == true);
    village.InitialiseToggleTorchTypes(object); CHECK(g_setActive == 2 && g_lastActive == false && light.Active == false);
    if (fails == 0) printf("00659295_TEST PASS\n"); else printf("00659295_TEST FAIL\n");
    return fails;
}