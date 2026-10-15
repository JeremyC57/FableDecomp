// Behaviour test for 0x004CE4E0: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model: a global def holding an index->text map, released through its reference count when the by-value pointer dies.
struct Str { char s[32]; Str() { s[0] = 0; } Str(const char* t) { std::strcpy(s, t); } };
struct Def { int RefCount; int released; int keys[3]; Str texts[3]; int n;
    int Find(int k) const { for (int i = 0; i < n; ++i) if (keys[i] == k) return i; return -1; } };
struct DefPtr { Def* p; DefPtr(Def* d) : p(d) { ++p->RefCount; } DefPtr(const DefPtr& o) : p(o.p) { ++p->RefCount; }
    ~DefPtr() { if (--p->RefCount == 0) p->released++; } };
struct Msg {
    float lastTime; Str lastText; int added;
    Msg() : lastTime(0), added(0) {}
    bool AddCameraTagFromText(float t, const Str& s) { lastTime = t; lastText = s; ++added; return s.s[0] != 'x'; }
    bool AddCameraTagFromIndex(int index, float time, DefPtr def)
    {
        int it = def.p->Find(index);
        if (it != -1)
            return AddCameraTagFromText(time, def.p->texts[it]);
        return false;
    }
};
static void Run()
{
    Def d; d.RefCount = 0; d.released = 0; d.n = 2; d.keys[0] = 3; d.texts[0] = Str("CAM_A"); d.keys[1] = 8; d.texts[1] = Str("xCAM");
    Msg m;
    {
        DefPtr hold(&d);
        CHECK(m.AddCameraTagFromIndex(3, 1.5f, hold) && m.lastTime == 1.5f && std::strcmp(m.lastText.s, "CAM_A") == 0);
        CHECK(!m.AddCameraTagFromIndex(8, 2.0f, hold) && m.added == 2);
        CHECK(!m.AddCameraTagFromIndex(5, 2.0f, hold) && m.added == 2);
        CHECK(d.RefCount == 1 && d.released == 0);
    }
    CHECK(d.RefCount == 0 && d.released == 1);
    d.RefCount = 0;
    bool r = m.AddCameraTagFromIndex(1, 0.0f, DefPtr(&d));
    CHECK(!r && d.released == 2);
}

int main()
{
    Run();
    if (g_fail) { std::printf("004ce4e0_TEST FAIL\n"); return 1; }
    std::printf("004ce4e0_TEST PASS\n");
    return 0;
}
