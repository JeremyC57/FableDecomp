// Behaviour test for CTCAISummon::FrameUpdate @ 0x007FDEF0: every null pointer is removed from the
// summoned-creature list (live ones stay, in order), then the base frame hook runs.
#include <cstdio>

struct CThing { int unused; };
static int g_baseCalls = 0;

struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    void SetPItem(void* item) { m_pItem = item; }
};

template<class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

template<class T>
struct CSimpleList {
    void* items[8]; int count;
    void remove(const T& value)
    {
        int w = 0;
        for (int r = 0; r < count; ++r)
            if (items[r] != value.m_pItem) items[w++] = items[r];
        count = w;
    }
};

struct CTCBase { void FrameUpdate() { ++g_baseCalls; } };

struct CTCAISummon : CTCBase {
    CSimpleList<CIntelligentPointer<CThing> > MySummonedCreatures;
    void FrameUpdate()
    {
        MySummonedCreatures.remove(CIntelligentPointer<CThing>(0));
        CTCBase::FrameUpdate();
    }
};

int main()
{
    CThing a, b;
    CTCAISummon s;
    s.MySummonedCreatures.count = 4;
    s.MySummonedCreatures.items[0] = 0;
    s.MySummonedCreatures.items[1] = &a;
    s.MySummonedCreatures.items[2] = 0;
    s.MySummonedCreatures.items[3] = &b;
    s.FrameUpdate();
    bool ok = g_baseCalls == 1 && s.MySummonedCreatures.count == 2 && s.MySummonedCreatures.items[0] == &a
           && s.MySummonedCreatures.items[1] == &b;
    if (!ok) { std::printf("007fdef0_TEST FAIL\n"); return 1; }
    std::printf("007fdef0_TEST PASS\n");
    return 0;
}
