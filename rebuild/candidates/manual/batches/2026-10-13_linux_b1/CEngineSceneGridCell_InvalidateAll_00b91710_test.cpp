#include <cstdio>

struct Cell { Cell* Parent; unsigned char pad[0x40]; unsigned char Flags;
    void InvalidateAll() { Cell* c = this; do { unsigned char f = c->Flags; if (!(f & 7)) return; c->Flags = (unsigned char)(f & ~7); c = c->Parent; } while (c); } };
int main()
{
    Cell a, b, c; a.Parent = &b; b.Parent = &c; c.Parent = 0;
    a.Flags = 0x1F; b.Flags = 0x0B; c.Flags = 0x08;   // c already invalid -> never touched
    a.InvalidateAll();
    if (a.Flags != 0x18 || b.Flags != 0x08 || c.Flags != 0x08) { std::printf("00b91710_TEST FAIL\n"); return 1; }
    std::printf("00b91710_TEST PASS\n");
    return 0;
}
