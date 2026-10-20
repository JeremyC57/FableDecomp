// Behaviour test for CTCExpression::OnSerialise (0x007EF070): save writes the def name (empty without a def), load
// looks the def up again when a name was read.
#include <cstdio>
#include <cstring>

struct Ctx { long Mode; char stored[32]; };
struct Expr {
    const char* defName; int lookups; char lookedUp[32];
    void OnSerialise(Ctx& c)
    {
        if (c.Mode == 1 || c.Mode == 3) {
            char name[32] = "";
            if (defName) std::strcpy(name, defName);
            std::strcpy(c.stored, name);
        } else {
            char name[32];
            std::strcpy(name, c.stored);
            if (std::strcmp(name, "") != 0) { ++lookups; std::strcpy(lookedUp, name); }
        }
    }
};

int main()
{
    Ctx c = { 1, "" };
    Expr e = { "EXPRESSION_SMILE", 0, "" };
    e.OnSerialise(c);
    if (std::strcmp(c.stored, "EXPRESSION_SMILE") != 0) { std::printf("007ef070_TEST FAIL\n"); return 1; }
    Expr l = { 0, 0, "" };
    c.Mode = 2; l.OnSerialise(c);
    if (l.lookups != 1 || std::strcmp(l.lookedUp, "EXPRESSION_SMILE") != 0) { std::printf("007ef070_TEST FAIL\n"); return 1; }
    c.Mode = 3; Expr none = { 0, 0, "" }; none.OnSerialise(c);
    if (c.stored[0] != 0) { std::printf("007ef070_TEST FAIL\n"); return 1; }
    c.Mode = 0; l.OnSerialise(c);
    if (l.lookups != 1) { std::printf("007ef070_TEST FAIL\n"); return 1; }
    std::printf("007ef070_TEST PASS\n");
    return 0;
}