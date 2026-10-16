#include <cstdio>
struct V { float x, y, z; };
struct Out { unsigned int words[8]; int nw; const void* blocks[4]; unsigned int sizes[4]; int nb;
    Out() : nw(0), nb(0) {}
    void WriteULong(unsigned int v) { words[nw++] = v; }
    void WriteData(const void* p, unsigned int n) { blocks[nb] = p; sizes[nb++] = n; } };
struct Arr { V* b; V* e; V* c; unsigned int Size() const { return (unsigned int)(e - b); }
    const V* GetData() const { return Size() != 0 ? b : (const V*)this; } };
int main() {
    V u[3]; Arr U = { u, u + 3, u + 3 }; Arr Vt = { 0, 0, 0 }; Out o;
    o.WriteULong(1); o.WriteULong(U.Size()); o.WriteData(U.GetData(), U.Size() * sizeof(V)); o.WriteData(Vt.GetData(), Vt.Size() * sizeof(V));
    if (o.nw != 2 || o.words[1] != 3 || o.sizes[0] != 36 || o.blocks[0] != u || o.blocks[1] != &Vt || o.sizes[1] != 0) { std::printf("00aa57f0_TEST FAIL\n"); return 1; }
    std::printf("00aa57f0_TEST PASS\n"); return 0; }