#include <cstdio>
struct V { long Index; };
static int g_written = -1, g_text = -1, g_bin = 0;
static bool Eq(const V& a, const V& b) { return a.Index == b.Index; }
struct Item { V* value; Item(V& v) : value(&v) {} };
struct Traits { void TransferBinaryIn(int, V& v) { ++g_bin; } };
struct Ctx {
    bool UseDefaults; long Mode; int crc;
    void CheckCRC(const char*) { ++crc; }
    void LoadText(const char*, Item&) { g_text = 0; }
    void SaveText(const char*, Item&) { g_text = 1; }
    void Write(const V& v) { g_written = (int)v.Index; }
    void Transfer(const char* name, V& value, const V& def)
    {
        CheckCRC(name);
        switch (Mode) {
        case 0: { if (UseDefaults) value = def; Item item(value); LoadText(name, item); break; }
        case 1: { Item item(value); SaveText(name, item); break; }
        case 2: { Traits t; t.TransferBinaryIn(0, value); break; }
        case 3: Write(value); break;
        default: value = def; break;
        }
    }
};
int main()
{
    Ctx c; c.crc = 0; V def = { 5 }; V val = { 0 }; V other = { 9 };
    c.Mode = 0; c.UseDefaults = false; c.Transfer("n", val, def); int ok = !Eq(val, def) && g_text == 0;
    c.UseDefaults = true; c.Transfer("n", val, def); ok = ok && Eq(val, def);
    c.Mode = 1; c.Transfer("n", val, other); ok = ok && g_text == 1 && Eq(val, def);
    c.Mode = 2; c.Transfer("n", val, other); ok = ok && g_bin == 1;
    c.Mode = 3; c.Transfer("n", other, def); ok = ok && g_written == 9;
    c.Mode = 7; c.Transfer("n", val, other); ok = ok && Eq(val, other) && c.crc == 6;
    if (!ok) { std::printf("00580371_TEST FAIL\n"); return 1; }
    std::printf("00580371_TEST PASS\n");
    return 0;
}