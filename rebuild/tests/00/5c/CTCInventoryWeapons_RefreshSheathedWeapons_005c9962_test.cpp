#include <cstdio>
// Behaviour of CTCInventoryWeapons::RefreshSheathedWeapons (0x005C9962).
class CTCBase { public: bool m_weaponsSheathed; };
struct CPresenceBits { unsigned long m_bits[2]; bool test(long bit) { return (m_bits[bit >> 5] >> (bit & 31)) & 1; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing { public:
    CPresenceBits m_present; CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_present.test(type)) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
typedef CTCBase CTCCarrier;
class CTCInventoryWeapons { public:
    CThing* ParentThing; long m_put; long m_taken;
    void PutWeaponsInSheathes() { ++m_put; }
    void TakeWeaponsOutOfSheathes() { ++m_taken; }
    void RefreshSheathedWeapons()
    {
        CTCCarrier* carrier = 0;
        ParentThing->GetInterface(0x29, &carrier);
        if (carrier->m_weaponsSheathed)
            PutWeaponsInSheathes();
        else
            TakeWeaponsOutOfSheathes();
    }
};
int main()
{
    CTCCarrier carrier; carrier.m_weaponsSheathed = true;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x29, &carrier } };
    CThing thing; thing.m_present.m_bits[0] = 0; thing.m_present.m_bits[1] = 1UL << (0x29 - 32);
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CTCInventoryWeapons w; w.ParentThing = &thing; w.m_put = 0; w.m_taken = 0;
    w.RefreshSheathedWeapons();
    int bad = (w.m_put != 1 || w.m_taken != 0);
    carrier.m_weaponsSheathed = false; w.RefreshSheathedWeapons();
    bad |= (w.m_put != 1 || w.m_taken != 1);
    if (bad) { std::printf("005c9962_TEST FAIL\n"); return 1; }
    std::printf("005c9962_TEST PASS\n");
    return 0;
}