#include <cstdio>
// ---- test model: a thing's interface bitset + sorted {type, component} table, intelligent pointers ----
struct CThing;
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CInterfaceMap::SEntry m_table[8];
    int m_count;
    void Init() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_flags91 = 0; m_count = 0; Sync(); }
    void Sync() { m_interfaces.first = m_table; m_interfaces.last = m_table + m_count; }
    void Add(long type, void* comp)       // keep the table sorted
    {
        int i = m_count;
        while (i > 0 && m_table[i - 1].type > type) { m_table[i] = m_table[i - 1]; --i; }
        m_table[i].type = type; m_table[i].component = comp; ++m_count;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        Sync();
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
    void SetPItem(CThing* p) { m_pItem = p; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain { CThing* PCreature; };

struct CAnimInstance { float m_fraction; };
struct CTCAnim { CAnimInstance* cur; CAnimInstance* GetPCurrentAnim() { return cur; } };
static long g_FramesPerSecond = 15;
static long g_frame = 0;
static int g_idles = 0, g_terminated = 0, g_stopAt = -1;
static CAnimInstance* g_anim;

struct CAIStateGroup_JackDragon {
    CAIBrain* PBrain;
    CBaseIntelligentPointer PEnemy;
    long AIAction_DoNothing() { ++g_idles; ++g_frame; g_anim->m_fraction += 0.25f; return 0; }
    bool IsStopping() { return g_idles == g_stopAt; }
    void Terminate() { ++g_terminated; }
    long GetCurrentFrame() { return g_frame; }
    void WaitForAnimLoop(float seconds)
    {
        CTCAnim* anim = 0;
        PBrain->PCreature->GetInterface(&anim, 0x5a);
        CAnimInstance* current = anim->GetPCurrentAnim();
        long start = GetCurrentFrame();
        float wait = (float)g_FramesPerSecond * seconds;
        long elapsed;
        do {
            AIAction_DoNothing();
            if (!IS_THING_ALIVE(PEnemy) || IsStopping()) {
                Terminate();
                return;
            }
            elapsed = GetCurrentFrame() - start;
        } while (current->m_fraction < 0.995f || (float)elapsed < wait);
    }
};

int main()
{
    CThing dragon, enemy; dragon.Init(); enemy.Init();
    CAnimInstance inst = { 0.0f }; g_anim = &inst;
    CTCAnim anim = { &inst };
    dragon.Add(0x5a, &anim);
    CAIBrain brain = { &dragon };
    CAIStateGroup_JackDragon d; d.PBrain = &brain; d.PEnemy.SetPItem(&enemy);
    d.WaitForAnimLoop(0.0f);                    // anim reaches 1.0 after 4 idles
    bool ok = g_idles == 4 && g_terminated == 0;
    g_idles = 0; inst.m_fraction = 0.0f;
    d.WaitForAnimLoop(1.0f);                    // 15 frames
    ok = ok && g_idles == 15 && g_terminated == 0;
    g_idles = 0; inst.m_fraction = 0.0f; g_stopAt = 2;
    d.WaitForAnimLoop(1.0f);
    ok = ok && g_idles == 2 && g_terminated == 1;
    if (!ok) { std::printf("00950950_TEST FAIL\n"); return 1; }
    std::printf("00950950_TEST PASS\n");
    return 0;
}
