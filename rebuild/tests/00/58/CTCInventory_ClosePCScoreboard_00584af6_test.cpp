#include <cstdio>
// Behaviour test for CTCInventory::ClosePCScoreboard (retail 0x00584AF6).
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
static CTCBase* g_closedWho = 0;
struct CTCPCScoreboard : CTCBase {
    void Close() { ++g_closed; g_closedWho = this; }
};
class CTCInventory { public:
    CThing* ParentThing;
    void ClosePCScoreboard();
};
void CTCInventory::ClosePCScoreboard()
{
    CTCPCScoreboard* scoreboard;
    ParentThing->GetInterface(0x10d, &scoreboard);
    scoreboard->Close();
}
int main()
{
    CTCPCScoreboard board; board.tag = 1;
    CTCBase other; other.tag = 2;
    CInterfaceMap::SEntry entries[2] = { { 0x12, &other }, { 0x10d, &board } };
    CThing thing;
    for (int k = 0; k < 16; ++k) thing.m_present.bits[k] = 0;
    thing.m_present.bits[0x10d >> 5] |= 1ul << (0x10d & 31);
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CTCInventory inv; inv.ParentThing = &thing;
    inv.ClosePCScoreboard();
    if (g_closed != 1 || g_closedWho != &board) { std::printf("00584af6_TEST FAIL\n"); return 1; }
    std::printf("00584af6_TEST PASS\n");
    return 0;
}