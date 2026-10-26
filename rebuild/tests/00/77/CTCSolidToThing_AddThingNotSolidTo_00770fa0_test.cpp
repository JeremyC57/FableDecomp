// Behaviour test for CTCSolidToThing::AddThingNotSolidTo (00770fa0): a thing is appended once (through a temporary intelligent pointer), never twice.
#include <cstdio>

class CThing { public: int id; };
static int g_live = 0;                                      // live intelligent pointers

struct CBaseIntelligentPointer {
    const CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_live; }
    CBaseIntelligentPointer(const CBaseIntelligentPointer& o) : m_pItem(o.m_pItem) { ++g_live; }
    ~CBaseIntelligentPointer() { --g_live; }
    void SetPItem(const CThing* item) { m_pItem = item; }
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(const CThing* thing) : CBaseIntelligentPointer() { SetPItem(thing); }
};

struct CThingPointerContainer {
    const CThing* items[8];
    int           count;
    void push_back(const CIntelligentPointer& value) { items[count++] = value.m_pItem; }
};

class CTCSolidToThing { public:
    CThingPointerContainer NotSolidToThings;
    bool IsSolidToThing(const CThing* thing) const
    {
        for (int i = 0; i < NotSolidToThings.count; ++i)
            if (NotSolidToThings.items[i] == thing)
                return false;
        return true;
    }
    void AddThingNotSolidTo(const CThing* thing);
};

void CTCSolidToThing::AddThingNotSolidTo(const CThing* thing)
{
    if (IsSolidToThing(thing)) {
        CIntelligentPointer pointer(thing);
        NotSolidToThings.push_back(pointer);
    }
}

int main()
{
    CThing a, b;
    CTCSolidToThing c; c.NotSolidToThings.count = 0;
    c.AddThingNotSolidTo(&a); c.AddThingNotSolidTo(&b); c.AddThingNotSolidTo(&a);
    if (c.NotSolidToThings.count != 2 || c.NotSolidToThings.items[0] != &a || c.NotSolidToThings.items[1] != &b || g_live != 0) {
        std::printf("00770fa0_TEST FAIL\n"); return 1;
    }
    std::printf("00770fa0_TEST PASS\n");
    return 0;
}