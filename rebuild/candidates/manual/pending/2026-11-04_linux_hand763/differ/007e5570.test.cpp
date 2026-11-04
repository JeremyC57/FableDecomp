// Behaviour test for 0x007E5570 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <string>
#include <sstream>
static std::string PlayersMoney(long money, bool placed, long stake, bool input, bool haveDef) {
    if (placed) money -= stake;
    if (input && haveDef) { std::ostringstream s; s << "Money: " << money; return s.str(); }
    return "";
}
int main() {
    bool ok = PlayersMoney(100, false, 30, true, true) == "Money: 100" && PlayersMoney(100, true, 30, true, true) == "Money: 70";
    ok = ok && PlayersMoney(100, true, 30, false, true).empty() && PlayersMoney(100, false, 0, true, false).empty();
    std::printf(ok ? "007e5570_TEST PASS\n" : "007e5570_TEST FAIL\n");
    return ok ? 0 : 1;
}
