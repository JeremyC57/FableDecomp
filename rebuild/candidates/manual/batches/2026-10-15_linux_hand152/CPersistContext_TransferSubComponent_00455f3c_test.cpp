#include <cstdio>
static int g_last = -1;
struct Ctx;
struct Comp { int binary; Comp() : binary(0) {} void TransferBinary(Ctx*) { ++binary; } };
struct Item { Comp* value; Item(Comp& c) : value(&c) {} };
struct Ctx {
    long Mode; int crc;
    void CheckCRC(const char*) { ++crc; }
    void LoadText(const char*, Item& i) { g_last = 0; i.value->binary += 100; }
    void SaveText(const char*, Item& i) { g_last = 1; i.value->binary += 1000; }
    void TransferSubComponent(const char* name, Comp& value)
    {
        CheckCRC(name);
        long mode = Mode;
        if (mode != 0 && mode != 1) value.TransferBinary(this);
        else if (mode == 1) { Item item(value); SaveText(name, item); }
        else { Item item(value); LoadText(name, item); }
    }
};
int main()
{
    Ctx c; c.crc = 0; Comp v;
    c.Mode = 0; c.TransferSubComponent("a", v); if (g_last != 0 || v.binary != 100) { std::printf("00455f3c_TEST FAIL\n"); return 1; }
    c.Mode = 1; c.TransferSubComponent("a", v); if (g_last != 1 || v.binary != 1100) { std::printf("00455f3c_TEST FAIL\n"); return 1; }
    c.Mode = 3; c.TransferSubComponent("a", v); if (v.binary != 1101 || c.crc != 3) { std::printf("00455f3c_TEST FAIL\n"); return 1; }
    std::printf("00455f3c_TEST PASS\n");
    return 0;
}
