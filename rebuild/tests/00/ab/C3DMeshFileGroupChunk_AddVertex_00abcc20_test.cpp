#include <cstdio>

struct V { long b; float w; };
struct Arr { V* beg; V* end; V* cap; V store[4];
    Arr() { beg = store; end = store; cap = store + 4; }
    void Add(const V& v) { if (end == cap) return; *end = v; ++end; } };
struct Chunk { Arr list;
    void AddVertex(const V& v) {
        if (list.beg != list.end && list.end[-1].b == v.b) list.end[-1].w += v.w; else list.Add(v); } };
int main()
{
    Chunk c; V a = { 1, 0.5f }, b = { 1, 0.25f }, d = { 2, 1.0f };
    c.AddVertex(a); c.AddVertex(b); c.AddVertex(d);
    if (c.list.end - c.list.beg != 2 || c.list.store[0].w != 0.75f || c.list.store[1].b != 2) { std::printf("00abcc20_TEST FAIL\n"); return 1; }
    std::printf("00abcc20_TEST PASS\n");
    return 0;
}