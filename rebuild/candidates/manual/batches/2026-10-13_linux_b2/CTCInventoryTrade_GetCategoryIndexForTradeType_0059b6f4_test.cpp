#include <cstdio>

struct CTradeTypeCategory { long Ident; long CategoryIndex; };
CTradeTypeCategory g_TradeTypeCategories[2] = { { 11, 0 }, { 12, 1 } };
struct T { unsigned char pad[0x170]; long CategoryIdent;
    long Get() { long id = CategoryIdent; for (long i = 0; i < 2; ++i) if (g_TradeTypeCategories[i].Ident == id) return g_TradeTypeCategories[i].CategoryIndex; return -1; } };
int main()
{
    T t; t.CategoryIdent = 12; if (t.Get() != 1) { std::printf("0059b6f4_TEST FAIL\n"); return 1; }
    t.CategoryIdent = 11; if (t.Get() != 0) { std::printf("0059b6f4_TEST FAIL\n"); return 1; }
    t.CategoryIdent = 3; if (t.Get() != -1) { std::printf("0059b6f4_TEST FAIL\n"); return 1; }
    std::printf("0059b6f4_TEST PASS\n");
    return 0;
}
