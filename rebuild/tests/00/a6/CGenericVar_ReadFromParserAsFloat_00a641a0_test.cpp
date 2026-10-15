#include <cstdio>
// Host model of ReadFromParserAsFloat: an integer item (type 3) is converted, a float item (type 4) stored, else false.
struct Item { long type; long i; float f; long GetAsLong() const { return i; } float GetAsFloat() const { return f; } };
struct Parser { Item items[4]; int n, pos; bool ReadNextItem(Item& out) { if (pos >= n) return false; out = items[pos++]; return true; } };
struct Var {
    float storage; float* target; bool ref;
    Var(bool r) : storage(-1.0f), target(&storage), ref(r) {}
    bool ReadFromParserAsFloat(Parser& p)
    {
        Item item;
        if (p.ReadNextItem(item)) {
            if (item.type == 3) { *(ref ? target : &storage) = (float)item.GetAsLong(); return true; }
            if (item.type == 4) { *(ref ? target : &storage) = item.GetAsFloat(); return true; }
        }
        return false;
    }
};
int main()
{
    int fail = 0;
    Parser p; p.n = 3; p.pos = 0;
    p.items[0].type = 3; p.items[0].i = -7;
    p.items[1].type = 4; p.items[1].f = 2.5f;
    p.items[2].type = 1; p.items[2].i = 9;
    Var v(0 != 0);
    if (!v.ReadFromParserAsFloat(p) || v.storage != -7.0f) fail = 1;
    if (!v.ReadFromParserAsFloat(p) || v.storage != 2.5f) fail = 1;
    if (v.ReadFromParserAsFloat(p) || v.storage != 2.5f) fail = 1;
    if (v.ReadFromParserAsFloat(p)) fail = 1;
    if (fail) { std::printf("00a641a0_TEST FAIL\n"); return 1; }
    std::printf("00a641a0_TEST PASS\n");
    return 0;
}