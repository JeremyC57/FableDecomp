// Behaviour test for CTCTargetingAI::AddThingToNotAttack (007f61b0): a thing is appended once (through a temporary intelligent pointer), never twice.
#include <cstdio>

struct CThing { int id; };
static int g_live = 0;                                      // live intelligent pointers

struct CBaseIntelligentPointer {
    const CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_live; }
    CBaseIntelligentPointer(const CBaseIntelligentPointer& o) : m_pItem(o.m_pItem) { ++g_live; }
    ~CBaseIntelligentPointer() { --g_live; }
    void SetPItem(const CThing* item) { m_pItem = item; }
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CThing* thing) : CBaseIntelligentPointer() { SetPItem(thing); }
};

struct CThingPointerContainer {
    const CThing* items[8];
    int           count;
    void push_back(const CIntelligentPointer& value) { items[count++] = value.m_pItem; }
};

struct CTCTargetingAI {
    CThingPointerContainer ThingsToNotAttack;
    bool IsThingAttackable(const CThing* thing) const
    {
        for (int i = 0; i < ThingsToNotAttack.count; ++i)
            if (ThingsToNotAttack.items[i] == thing)
                return false;
        return true;
    }
    void AddThingToNotAttack(CThing* thing);
};

void CTCTargetingAI::AddThingToNotAttack(CThing* thing)
{
    if (IsThingAttackable(thing)) {
        CIntelligentPointer pointer(thing);
        ThingsToNotAttack.push_back(pointer);
    }
}

int main()
{
    CThing a, b;
    CTCTargetingAI c; c.ThingsToNotAttack.count = 0;
    c.AddThingToNotAttack(&a); c.AddThingToNotAttack(&b); c.AddThingToNotAttack(&a);
    if (c.ThingsToNotAttack.count != 2 || c.ThingsToNotAttack.items[0] != &a || c.ThingsToNotAttack.items[1] != &b || g_live != 0) {
        std::printf("007f61b0_TEST FAIL\n"); return 1;
    }
    std::printf("007f61b0_TEST PASS\n");
    return 0;
}
