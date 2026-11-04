// Behaviour test for 0x00BE2540 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <cstring>
struct SQuad { float V[4]; };
struct Marker { SQuad Quad; void* First; void* Second; bool FlagA, FlagB;
    Marker* Init() { SQuad q; std::memset(&q, 0, sizeof q); Quad = q; First = 0; Second = 0; FlagA = false; FlagB = false; return this; } };
int main() {
    Marker m; std::memset(&m, 0x5a, sizeof m);
    bool ok = m.Init() == &m && m.Quad.V[0] == 0.0f && m.Quad.V[3] == 0.0f && !m.First && !m.Second && !m.FlagA && !m.FlagB;
    std::printf(ok ? "00be2540_TEST PASS\n" : "00be2540_TEST FAIL\n");
    return ok ? 0 : 1;
}
