// Behaviour test for 0x00C75F80 CCombFilter::Allocate: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <cstring>
struct Ring { float* data; unsigned size; unsigned pos; void Clear() { std::memset(data, 0, size * sizeof(float)); } };
struct Comb { Ring ring; unsigned mode; };
static void Allocate(Comb& c, unsigned length, float** pMemory, unsigned* pBytes, unsigned index)
{
    if (*pMemory) { float* block = *pMemory; *pMemory += length; c.ring.data = block; c.ring.size = length; c.ring.pos = 0; c.ring.Clear(); c.mode = index; }
    *pBytes += length * sizeof(float);
}
int main() {
    float pool[8]; for (int i = 0; i < 8; ++i) pool[i] = 1.0f;
    float* mem = pool; unsigned bytes = 4; Comb c = { { 0, 0, 7 }, 0 };
    Allocate(c, 3, &mem, &bytes, 5);
    int ok = mem == pool + 3 && bytes == 16 && c.ring.data == pool && c.ring.size == 3 && c.ring.pos == 0 && c.mode == 5 && pool[0] == 0.0f && pool[2] == 0.0f && pool[3] == 1.0f;
    float* none = 0; unsigned bytes2 = 0; Comb d = { { 0, 0, 0 }, 9 };
    Allocate(d, 2, &none, &bytes2, 1);
    ok = ok && bytes2 == 8 && d.mode == 9 && d.ring.data == 0;
    std::printf(ok ? "00c75f80_TEST PASS\n" : "00c75f80_TEST FAIL\n");
    return ok ? 0 : 1;
}
