// behaviour test for 0x006fb240 std::__find_if<NAnimationEvents::CEvent*, CFilter_Bind2<CIsEventIndexEqualTo, CIsEventTimeEqualTo> >: STLport random-access find -- four tests per pass over (last - first) >> 2
// passes, then the 3/2/1 remainder; returns the first hit or last.
#include <cstdio>
#include <cmath>
struct CEvent { unsigned long Index : 30; unsigned long ExternallyImported : 1; unsigned long StopEvent : 1; float Time; };
struct Bind2 { const unsigned long* index; const float* time;
    bool operator()(const CEvent& e) const { return e.Index == *index && std::fabs(e.Time - *time) < 0.0001f; } };
template <class It, class P> static It find_body(It first, It last, P hit)
{
    int trip = (last - first) >> 2;
    for (; trip > 0; --trip) {
        if (hit(*first)) return first; ++first;
        if (hit(*first)) return first; ++first;
        if (hit(*first)) return first; ++first;
        if (hit(*first)) return first; ++first;
    }
    switch (last - first) {
    case 3: if (hit(*first)) return first; ++first;
    case 2: if (hit(*first)) return first; ++first;
    case 1: if (hit(*first)) return first;
    default: return last;
    }
}
int main()
{
    bool ok = true;
    CEvent ev[9];
    for (int i = 0; i < 9; ++i) { ev[i].Index = i % 4; ev[i].ExternallyImported = 1; ev[i].StopEvent = i & 1; ev[i].Time = 0.25f * i; }
    for (int n = 0; n <= 9; ++n)
        for (int k = 0; k < 9; ++k) {
            unsigned long idx = k % 4; float t = 0.25f * k + 0.00005f; Bind2 b; b.index = &idx; b.time = &t;
            CEvent* r = find_body(ev, ev + n, b);
            if (r != (k < n ? ev + k : ev + n)) ok = false;
        }
    { unsigned long idx = 1; float t = 0.25f + 0.001f; Bind2 b; b.index = &idx; b.time = &t;   // time off by 1e-3: no hit
      if (find_body(ev, ev + 9, b) != ev + 9) ok = false; }
    { unsigned long idx = 2; float t = 0.25f; Bind2 b; b.index = &idx; b.time = &t;            // right time, wrong index
      if (find_body(ev, ev + 9, b) != ev + 9) ok = false; }
    if (!ok) { std::printf("006fb240_TEST FAIL\n"); return 1; }
    std::printf("006fb240_TEST PASS\n");
    return 0;
}