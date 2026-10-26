#include <cstdio>
// Behaviour test for CTCInventory::ClosePCMapMenu (retail 0x00584FE3).
class CTCBase { public: long tag; };

struct CPresenceBits {
    unsigned long bits[16];
    bool test(long bit) { return (bits[bit >> 5] >> (bit & 31)) & 1; }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};

class CThing { public:
    CPresenceBits m_present;
    CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(long wanted, T** out)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

static int g_closed = 0;
struct CTCMapMenu : CTCBase {
    void Close() { ++g_closed; }
};
class CTCInventory { public:
    CThing* ParentThing;
    void ClosePCMapMenu();
};
void CTCInventory::ClosePCMapMenu()
{
    CTCMapMenu* mapMenu;
    if (ParentThing->GetInterface(0x1b, &mapMenu) && mapMenu)
        mapMenu->Close();
}
static int Fail() { std::printf("00584fe3_TEST FAIL\n"); return 1; }
int main()
{
    CTCMapMenu menu; menu.tag = 3;
    CInterfaceMap::SEntry entries[2] = { { 0x1b, &menu }, { 0x30, 0 } };
    CThing thing;
    for (int k = 0; k < 16; ++k) thing.m_present.bits[k] = 0;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    CTCInventory inv; inv.ParentThing = &thing;
    inv.ClosePCMapMenu();                         // interface absent: nothing
    if (g_closed != 0) return Fail();
    thing.m_present.bits[0] |= 1ul << 0x1b;
    inv.ClosePCMapMenu();
    if (g_closed != 1) return Fail();
    entries[0].component = 0;                     // present but null component: skipped
    inv.ClosePCMapMenu();
    if (g_closed != 1) return Fail();
    std::printf("00584fe3_TEST PASS\n");
    return 0;
}