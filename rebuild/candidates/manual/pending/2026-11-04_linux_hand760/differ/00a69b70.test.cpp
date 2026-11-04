// Behaviour test for 0x00A69B70 (parked row): the row's logic over a flat model.
#include <cstdio>

struct Data { bool Fail; void* Owner; bool Create() { return !Fail; } };
struct Pool { Data* Free; Data* Allocate() { Data* d = Free; Free = 0; return d; } };
struct Texture { Data* D; };
struct Manager {
    Pool Pools[4]; unsigned long Failed[4];
    long GetPoolIndex(unsigned long size) const { return size < 4 ? (long)size : -1; }
    bool AllocateTexture(Texture& t, unsigned long size, void* owner) {
        long index = GetPoolIndex(size); if (index == -1) return false;
        Data* d = Pools[index].Allocate();
        if (d && d->Create()) { d->Owner = owner; t.D = d; return true; }
        ++Failed[index]; return false;
    }
};
int main() {
    Data good = { false, 0 }, bad = { true, 0 }; int owner;
    Manager m = { { { &good }, { &bad }, { 0 }, { 0 } }, { 0, 0, 0, 0 } }; Texture t = { 0 };
    bool ok = m.AllocateTexture(t, 0, &owner) && t.D == &good && good.Owner == &owner;
    ok = ok && !m.AllocateTexture(t, 1, &owner) && m.Failed[1] == 1;
    ok = ok && !m.AllocateTexture(t, 2, &owner) && m.Failed[2] == 1 && !m.AllocateTexture(t, 9, &owner);
    std::printf(ok ? "00a69b70_TEST PASS\n" : "00a69b70_TEST FAIL\n");
    return ok ? 0 : 1;
}
