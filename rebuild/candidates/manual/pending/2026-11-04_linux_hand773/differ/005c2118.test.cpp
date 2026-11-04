// Behaviour test for CPersistTraits<NInventory::CItem>::TransferIn 0x005C2118: a token-list model of the text item format
// "( hasData , [ ( count , name... , kills , wielded , trophy [ , questPersistent , refCount ] ) , ] defName , number , price )".
// Integer augmentation tokens are skipped, "" / "NULL" names leave the slot 0; any missing separator leaves the item unchanged.
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct Item { long Type, Number, Price; bool HasData; long Aug[4]; long AugCount, Kills, Wielded, Trophy, RefCount; bool Quest; };
struct Parser {
    const char* const* T; int Pos;
    const char* Next() { return T[Pos++]; }
    const char* Peek() const { return T[Pos]; }
    bool Sym(const char* s) { return std::strcmp(Next(), s) == 0; }
    long Long() { return std::atol(Next()); }
};
static long DefIndex(const char* name) { return (long)std::strlen(name) * 10; }
static void TransferIn(Parser& p, Item& value)
{
    Item data; std::memset(&data, 0, sizeof(data)); data.Quest = true; data.RefCount = -1;
    bool has = false;
    if (!p.Sym("(")) return;
    has = std::strcmp(p.Next(), "TRUE") == 0;
    if (!p.Sym(",")) return;
    if (has) {
        if (!p.Sym("(")) return;
        long count = p.Long();
        if (!p.Sym(",")) return;
        data.AugCount = count;
        for (long i = 0; i < count; ++i) {
            const char* t = p.Peek();
            if (t[0] >= '0' && t[0] <= '9') p.Next();
            else { const char* n = p.Next(); if (std::strcmp(n, "") && std::strcmp(n, "NULL")) data.Aug[i] = DefIndex(n); }
            if (!p.Sym(",")) return;
        }
        data.Kills = p.Long(); if (!p.Sym(",")) return;
        data.Wielded = p.Long(); if (!p.Sym(",")) return;
        data.Trophy = p.Long();
        if (std::strcmp(p.Next(), ",") == 0) {
            data.Quest = std::strcmp(p.Next(), "TRUE") == 0;
            if (!p.Sym(",")) return;
            data.RefCount = p.Long();
            if (!p.Sym(")")) return;
        }
        if (!p.Sym(",")) return;
    }
    long type = DefIndex(p.Next());
    if (p.Sym(",")) {
        long number = p.Long();
        if (p.Sym(",")) {
            long price = p.Long();
            if (p.Sym(")")) { data.Type = type; data.Number = number; data.Price = price; data.HasData = has; value = data; }
        }
    }
}
int main()
{
    static const char* const a[] = { "(", "TRUE", ",", "(", "2", ",", "ABC", ",", "7", ",", "5", ",", "6", ",", "1", ",",
                                     "FALSE", ",", "3", ")", ",", "SWORD", ",", "1", ",", "40", ")" };
    static const char* const b[] = { "(", "FALSE", ",", "AXE", ",", "2", ",", "9", ")" };
    static const char* const c[] = { "(", "FALSE", ";", "AXE" };
    Item x; std::memset(&x, 0, sizeof(x));
    Parser pa = { a, 0 }; TransferIn(pa, x);
    int ok = x.Type == 50 && x.Number == 1 && x.Price == 40 && x.HasData && x.AugCount == 2 && x.Aug[0] == 30 && x.Aug[1] == 0
          && x.Kills == 5 && x.Wielded == 6 && x.Trophy == 1 && !x.Quest && x.RefCount == 3;
    Parser pb = { b, 0 }; TransferIn(pb, x);
    ok = ok && x.Type == 30 && x.Number == 2 && x.Price == 9 && !x.HasData && x.Quest && x.RefCount == -1;
    Parser pc = { c, 0 }; TransferIn(pc, x);
    ok = ok && x.Type == 30 && x.Price == 9;
    std::printf(ok ? "005c2118_TEST PASS\n" : "005c2118_TEST FAIL\n");
    return ok ? 0 : 1;
}
