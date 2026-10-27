#include <cstdio>
#include <cstring>
// Behaviour model: create the component by definition name, give it the bank's text for the symbol, return it.
struct Comp { char def[48]; char text[32]; int texts; };
static Comp g_comp; static int g_creates;
static Comp* CreateComponent(const char* def) { ++g_creates; std::strcpy(g_comp.def, def); g_comp.texts = 0; return &g_comp; }
static const char* GetTextBySymbol(const char* sym) { return std::strcmp(sym, "SYM_A") == 0 ? "Text A" : "?"; }
static Comp* CreateMenuDesc(const char* sym)
{
    Comp* description = CreateComponent("PC_DESCRIPTION");
    std::strcpy(description->text, GetTextBySymbol(sym)); ++description->texts;
    return description;
}
int main()
{
    Comp* c = CreateMenuDesc("SYM_A");
    if (c != &g_comp || g_creates != 1 || std::strcmp(c->def, "PC_DESCRIPTION") != 0 || std::strcmp(c->text, "Text A") != 0 || c->texts != 1)
        { std::printf("005b3613_TEST FAIL\n"); return 1; }
    std::printf("005b3613_TEST PASS\n");
    return 0;
}
