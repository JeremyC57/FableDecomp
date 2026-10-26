// behaviour test for 0x0078915C vector<CThingGameObject*>::push_back: with room the value is written at the end and the end advances; when full the
// overflow is asked for exactly one copy of the value at the end, with atend set
#include <cstdio>
struct Vec {
    int* start; int* finish; int* end;
    int calls; int* pos; int val; unsigned int fill; bool atend;
    void Overflow(int* p, const int& x, unsigned int n, bool a) { ++calls; pos = p; val = x; fill = n; atend = a; }
    void PushBack(const int& x)                                  // the function under test
    {
        if (finish != end) { *finish = x; ++finish; }
        else Overflow(finish, x, 1u, true);
    }
};
int main()
{
    int fail = 0;
    int buf[2] = { 0, 0 };
    Vec v; v.start = buf; v.finish = buf; v.end = buf + 2; v.calls = 0;
    v.PushBack(11); v.PushBack(22);
    if (v.finish != buf + 2 || buf[0] != 11 || buf[1] != 22 || v.calls != 0) fail |= 1;
    v.PushBack(33);
    if (v.calls != 1 || v.pos != buf + 2 || v.val != 33 || v.fill != 1u || !v.atend || v.finish != buf + 2) fail |= 2;
    if (fail) { std::printf("0078915c_TEST FAIL %d\n", fail); return 1; }
    std::printf("0078915c_TEST PASS\n");
    return 0;
}