#include <cstdio>
// Behaviour test: ForEachIf applies the functor only to live things whose mode manager (interface 0x31) is in mode 0x42.
class CTCBase;
struct CMode { int type; int GetModeType() { return type; } };
class CTCCreatureModeManager { public: CMode* mode; CMode* GetCurrentMode() { return mode; } };
struct SEntry { int type; CTCBase* component; };
class CThing { public:
    unsigned int m_presentBits[9];
    SEntry* first; SEntry* last;
    unsigned char Dead : 1;
    bool HasTC(int type) const { if (m_presentBits[type >> 5] & (1U << (type & 31))) return true; return false; }
    template <class T> T* GetTC(int type) const
    {
        SEntry* l = last; SEntry* f = first;
        int count = (int)(l - f);
        SEntry* it = f;
        while (count > 0) {
            int half = count / 2;
            if (it[half].type < type) { it += half + 1; count -= half + 1; }
            else count = half;
        }
        if (it == l || type < it->type) it = l;
        return (T*)it->component;
    }
};
struct CThingFilter_IsThrownByExplosion {
    bool operator()(CThing* thing) const
    {
        if (thing && !thing->Dead && thing->HasTC(0x31)
            && thing->GetTC<CTCCreatureModeManager>(0x31)->GetCurrentMode()->GetModeType() == 0x42)
            return true;
        return false;
    }
};
static CThing* g_applied[8];
static int g_count = 0;
struct CApplyImpulse { float m_data[5]; void operator()(CThing* t) const { g_applied[g_count++] = t; } };
template <class It, class Pred, class Func> void ForEachIf(It first, It last, Pred pred, Func func)
{
    for (; first != last; ++first)
        if (pred(*first))
            func(*first);
}
static void Init(CThing& t, SEntry* entries, int n, bool hasModes)
{
    int i = 0;
    for (i = 0; i < 9; ++i) t.m_presentBits[i] = 0;
    if (hasModes) t.m_presentBits[1] = 1U << (0x31 & 31);
    t.first = entries; t.last = entries + n; t.Dead = 0;
}
int main()
{
    CMode thrown; thrown.type = 0x42; CMode idle; idle.type = 3;
    CTCCreatureModeManager mThrown; mThrown.mode = &thrown; CTCCreatureModeManager mIdle; mIdle.mode = &idle;
    SEntry e1[3]; e1[0].type = 4; e1[0].component = 0; e1[1].type = 0x31; e1[1].component = (CTCBase*)&mThrown; e1[2].type = 0x50; e1[2].component = 0;
    SEntry e2[1]; e2[0].type = 0x31; e2[0].component = (CTCBase*)&mIdle;
    CThing a, b, c, d;
    Init(a, e1, 3, true); Init(b, e2, 1, true); Init(c, e1, 3, true); c.Dead = 1; Init(d, e1, 3, false);
    CThing* things[5] = { &a, 0, &b, &c, &d };
    CApplyImpulse f;
    ForEachIf(things, things + 5, CThingFilter_IsThrownByExplosion(), f);
    if (g_count != 1 || g_applied[0] != &a) { std::printf("0080e530_TEST FAIL\n"); return 1; }
    std::printf("0080e530_TEST PASS\n");
    return 0;
}