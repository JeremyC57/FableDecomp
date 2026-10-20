#include <cstdio>
struct Event { unsigned long Index : 30; unsigned long Imported : 1; unsigned long Stop : 1; float Time; };
struct Stats { float Duration; const float* Times; const unsigned long* Ids; int Count; };
static bool GetEvents(const Stats& s, Event* out, int& size, float start, float end)
{
    bool found = false;
    for (int i = 0; i < s.Count; ++i) {
        float time = s.Times[i] / s.Duration;
        if (time >= start && time <= end) {
            Event e; e.Index = s.Ids[i]; e.Imported = 1; e.Stop = 0; e.Time = time;
            found = true;
            out[size++] = e;
        }
    }
    return found;
}
int main()
{
    float times[4] = { 0.5f, 1.0f, 1.5f, 2.0f };
    unsigned long ids[4] = { 7, 8, 9, 10 };
    Stats s = { 2.0f, times, ids, 4 };
    Event out[8]; int size = 0;
    bool any = GetEvents(s, out, size, 0.5f, 0.75f);
    bool none = GetEvents(s, out, size, 0.8f, 0.9f);
    bool ok = any && !none && size == 2 && out[0].Index == 8 && out[1].Index == 9 && out[0].Imported == 1 && out[0].Stop == 0 && out[1].Time == 0.75f;
    if (!ok) { std::printf("006f9820_TEST FAIL\n"); return 1; }
    std::printf("006f9820_TEST PASS\n");
    return 0;
}