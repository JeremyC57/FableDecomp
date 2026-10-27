// behaviour test for 0x00670010: the getter returns the table's text for the def field's table position
#include <cstdio>
#include <string>
static std::string GetString(long index) { return index == 7 ? "seven" : "other"; }
struct Def { long TablePos; };
static std::string Get(const Def* def) { std::string name = GetString(def->TablePos); return name; }
int main()
{
    Def d = { 7 }; Def e = { 3 };
    if (Get(&d) != "seven" || Get(&e) != "other") { std::printf("00670010_TEST FAIL\n"); return 1; }
    std::printf("00670010_TEST PASS\n");
    return 0;
}