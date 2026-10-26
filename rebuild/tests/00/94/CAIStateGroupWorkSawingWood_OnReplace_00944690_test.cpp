#include <cstdio>

// Test model of a thing: presence bitset + sorted {type, component} map + dead flag.
class CTCBase { public: long tag; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};

class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CInterfaceMap::SEntry m_store[8];
    int m_count;
    void Reset()
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_flags91 = 0; m_count = 0;
        m_interfaces.first = m_store; m_interfaces.last = m_store;
        m_store[0].type = 0; m_store[0].component = 0;
    }
    void Add(long type, CTCBase* c)     // append in ascending type order
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        m_store[m_count].type = type; m_store[m_count].component = c; ++m_count;
        m_interfaces.last = m_store + m_count;
        m_store[m_count].type = 0; m_store[m_count].component = 0;
    }
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
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
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public: CThing* PCreature; };
#include <cstring>

class CCharString { public:
    const char* text;
    CCharString(const char* t, long) : text(t) {}
    ~CCharString() {}
};

struct CTCUserActionPointBase : CTCBase {
    long released;
    void ReleaseActionPoint(long index) { released = index; }
};

struct CStateGroup {
    bool TookActionPoint; CBaseIntelligentPointer PSawhorse; long ActionPointIndex; int destroyed;
    void DestroyItem(const CCharString& slot) { if (std::strcmp(slot.text, "CARRY_SLOT_RIGHT_HAND") == 0) ++destroyed; }
    void OnReplace()
    {
        if (IS_THING_ALIVE(PSawhorse) && TookActionPoint) {
            CTCUserActionPointBase* actionPoint;
            if (PSawhorse.GetPItem()->GetInterface(&actionPoint, 0x2D))
                actionPoint->ReleaseActionPoint(ActionPointIndex);
        }
        {
            CCharString slot("CARRY_SLOT_RIGHT_HAND", -1);
            DestroyItem(slot);
        }
    }
};

static int fail() { std::printf("00944690_TEST FAIL\n"); return 1; }

int main()
{
    CThing obj; obj.Reset();
    CTCUserActionPointBase ap; ap.released = -5;
    obj.Add(3, 0); obj.Add(0x2D, &ap);
    CStateGroup g; g.TookActionPoint = true; g.PSawhorse.m_pItem = &obj; g.ActionPointIndex = 2; g.destroyed = 0;
    g.OnReplace();
    if (ap.released != 2 || g.destroyed != 1) return fail();
    ap.released = -5; g.TookActionPoint = false; g.OnReplace();
    if (ap.released != -5 || g.destroyed != 2) return fail();
    g.TookActionPoint = true; obj.m_flags91 = 1; g.OnReplace();
    if (ap.released != -5 || g.destroyed != 3) return fail();
    g.PSawhorse.m_pItem = 0; g.OnReplace();
    if (ap.released != -5 || g.destroyed != 4) return fail();
    std::printf("00944690_TEST PASS\n");
    return 0;
}