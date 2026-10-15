#include <cstdio>
#include <cstring>

struct CComponent { int tag; };
struct CInterfaceMap {
    struct SEntry { long type; CComponent* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry m_storage[4];
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void Init() { std::memset(m_presentBits, 0, sizeof(m_presentBits)); m_interfaces.first = m_storage; m_interfaces.last = m_storage; }
    void Add(long type, CComponent* c)   /* keys added in ascending order */
    {
        m_interfaces.last->type = type; m_interfaces.last->component = c; ++m_interfaces.last;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
};

static int g_baseActivates = 0, g_baseDeactivates = 0;
struct CTCChest : CComponent { int opens, closes; void Open() { ++opens; } void Close() { ++closes; } };
struct CTCActivationReceptorBase {
    CThing* ParentThing;
    void Activate() { ++g_baseActivates; }
    void Deactivate() { ++g_baseDeactivates; }
};
struct CTCActivationReceptorChest : CTCActivationReceptorBase {
    void Deactivate()
    {
        CTCActivationReceptorBase::Deactivate();
        CTCChest* chest;
        if (ParentThing->GetInterface(&chest, 0x3c))   /* retail calls through the uninitialised pointer when absent */
            chest->Close();
    }
};
int main()
{
    CThing thing; thing.Init();
    CComponent other; other.tag = 1;
    CTCChest chest; chest.tag = 2; chest.opens = chest.closes = 0;
    thing.Add(0x10, &other); thing.Add(0x3c, &chest);
    if ((thing.m_presentBits[1] & 0x10000000UL) == 0) { std::printf("007ffcd0_TEST FAIL\n"); return 1; }
    CTCActivationReceptorChest r; r.ParentThing = &thing;
    r.Deactivate();
    if (chest.closes != 1 || chest.opens != 0 || (g_baseActivates + g_baseDeactivates) != 1) { std::printf("007ffcd0_TEST FAIL\n"); return 1; }
    std::printf("007ffcd0_TEST PASS\n");
    return 0;
}