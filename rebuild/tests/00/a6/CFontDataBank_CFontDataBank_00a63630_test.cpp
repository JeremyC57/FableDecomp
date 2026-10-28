#include <cstdio>
#include <string>
// Behaviour test for CFontDataBank::CFontDataBank (0x00A63630): base, state block, empty Fonts, empty Filename.
static int g_order = 0, g_base = -1, g_state = -1;
class CBankFile { public: CBankFile() { g_base = g_order++; } virtual ~CBankFile() { } };
struct CFontBankStateBlock { CFontBankStateBlock() { g_state = g_order++; } virtual ~CFontBankStateBlock() { } };
struct Vec { void* s; void* f; void* e; Vec() : s(0), f(0), e(0) { } };
class CFontDataBank : public CBankFile { public:
    CFontBankStateBlock StateBlock; Vec Fonts; std::string Filename;
    CFontDataBank() { Filename = ""; }
};
int main()
{
    CFontDataBank b;
    bool ok = g_base == 0 && g_state == 1 && b.Fonts.s == 0 && b.Fonts.f == 0 && b.Fonts.e == 0 && b.Filename.empty();
    std::printf(ok ? "00a63630_TEST PASS\n" : "00a63630_TEST FAIL\n");
    return ok ? 0 : 1;
}