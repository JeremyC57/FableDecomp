#include <cstdio>
// Behaviour model of the handler-table registration at 0x008B08B0: three int -> handler maps; the first gets
// keys 0..8 (key 0 with a null handler), the second keys 0..1, the third key 0; inserting an existing key keeps the
// first handler (map::insert semantics), so running it twice changes nothing.
typedef bool (*THandler)(int);
static bool H1(int) { return false; }
static bool H2(int v) { return v == 2; }
static bool H3(int v) { return v == 3; }
static bool H4(int v) { return v == 4; }
static bool H5(int v) { return v == 5; }
static bool H6(int v) { return v == 6; }
static bool H7(int v) { return v == 7; }
static bool H8(int v) { return v == 8; }
static bool D1(int v) { return v == 11; }
struct HandlerPair { int first; THandler second; HandlerPair(int k, THandler f) : first(k), second(f) {} };
struct InsertResult { int index; bool inserted; };
struct HandlerMap {
    int keys[16]; THandler fns[16]; int count;
    HandlerMap() : count(0) {}
    InsertResult insert(const HandlerPair& v)
    {
        InsertResult r;
        for (int i = 0; i < count; ++i)
            if (keys[i] == v.first) { r.index = i; r.inserted = false; return r; }
        keys[count] = v.first; fns[count] = v.second; r.index = count++; r.inserted = true;
        return r;
    }
    THandler find(int k) const { for (int i = 0; i < count; ++i) if (keys[i] == k) return fns[i]; return 0; }
};
static HandlerMap g_A, g_B, g_C;
static void Register()
{
    g_A.insert(HandlerPair(0, 0));
    g_A.insert(HandlerPair(1, H1));
    g_A.insert(HandlerPair(2, H2));
    g_A.insert(HandlerPair(3, H3));
    g_A.insert(HandlerPair(4, H4));
    g_A.insert(HandlerPair(5, H5));
    g_A.insert(HandlerPair(6, H6));
    g_A.insert(HandlerPair(7, H7));
    g_A.insert(HandlerPair(8, H8));
    g_B.insert(HandlerPair(0, 0));
    g_B.insert(HandlerPair(1, D1));
    g_C.insert(HandlerPair(0, 0));
}
int main()
{
    Register();
    bool ok = g_A.count == 9 && g_B.count == 2 && g_C.count == 1
        && g_A.find(0) == 0 && g_A.find(1) == H1 && g_A.find(8) == H8 && g_A.find(5)(5)
        && g_B.find(1) == D1 && g_B.find(0) == 0 && g_C.find(0) == 0 && g_C.find(1) == 0;
    Register();
    ok = ok && g_A.count == 9 && g_B.count == 2 && g_C.count == 1 && g_A.find(3) == H3;
    if (!ok) { std::printf("008b08b0_TEST FAIL\n"); return 1; }
    std::printf("008b08b0_TEST PASS\n");
    return 0;
}