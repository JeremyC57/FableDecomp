// Behaviour test for 0x009E5C90 CSymbolMap::LoadFromFile: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <string>
static std::string Strip(std::string s, const std::string& open, const std::string& close)
{
    size_t p;
    while ((p = s.find(open)) != std::string::npos) { size_t e = s.find(close, p + open.size()); if (e == std::string::npos) { s.erase(p); break; } s.erase(p, e - p + (close == "\n" || close == "\r" ? 0 : close.size())); }
    return s;
}
int main() {
    std::string buf = "A=1 // x\nB=2 /* y */C=3";
    buf = Strip(buf, "//", "\n"); buf = Strip(buf, "//", "\r"); buf = Strip(buf, "/*", "*/");
    int ok = buf == "A=1 \nB=2 C=3";
    std::printf(ok ? "009e5c90_TEST PASS\n" : "009e5c90_TEST FAIL\n");
    return ok ? 0 : 1;
}
