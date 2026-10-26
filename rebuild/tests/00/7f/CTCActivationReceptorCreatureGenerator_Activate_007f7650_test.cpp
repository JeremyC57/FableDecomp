#include <cstdio>
#include <cstring>

struct CComponent { int tag; };
struct CInterfaceMap {
    struct SEntry { long type; CComponent* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
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

static int g_baseActivates = 0;
class CTCCreatureGenerator : public CComponent { public: bool ExternalTrigger; int activeCalls; bool lastActive; void SetAsActive(bool a) { ++activeCalls; lastActive = a; } };
class CTCActivationReceptorBase { public: CThing* ParentThing; void Activate() { ++g_baseActivates; } };
struct CTCActivationReceptorCreatureGenerator : CTCActivationReceptorBase {
    bool TriggerOnActivate, ActivateOnActivate;
    void Activate()
    {
        CTCActivationReceptorBase::Activate();
        CTCCreatureGenerator* generator;
        if (ParentThing->GetInterface(&generator, 0x3d)) {
            if (TriggerOnActivate) generator->ExternalTrigger = true;
            else generator->SetAsActive(true);
        }
    }
};
int main()
{
    CThing thing; thing.Init();
    CTCCreatureGenerator gen; gen.tag = 3; gen.ExternalTrigger = false; gen.activeCalls = 0; gen.lastActive = false;
    thing.Add(0x3d, &gen);
    if ((thing.m_presentBits[1] & 0x20000000UL) == 0) { std::printf("007f7650_TEST FAIL\n"); return 1; }
    CTCActivationReceptorCreatureGenerator r; r.ParentThing = &thing; r.TriggerOnActivate = true; r.ActivateOnActivate = false;
    r.Activate();
    if (!gen.ExternalTrigger || gen.activeCalls != 0 || g_baseActivates != 1) { std::printf("007f7650_TEST FAIL\n"); return 1; }
    gen.ExternalTrigger = false; r.TriggerOnActivate = false; r.ActivateOnActivate = true;
    r.Activate();
    if (gen.ExternalTrigger || gen.activeCalls != 1 || !gen.lastActive || g_baseActivates != 2) { std::printf("007f7650_TEST FAIL\n"); return 1; }
    std::printf("007f7650_TEST PASS\n");
    return 0;
}