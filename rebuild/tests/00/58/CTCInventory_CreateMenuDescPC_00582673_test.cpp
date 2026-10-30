// Behaviour test for CTCInventory::CreateMenuDescPC (retail 0x00582673): creates the "PC_DESCRIPTION" component, sets its text to the
// text bank's entry for the symbol (both flags true) and returns it counted (one reference held by the caller).
#include <cstdio>
#include <cstring>
struct CComponent { char def[64]; int textId; bool a, b; void SetText(int t, bool x, bool y) { textId = t; a = x; b = y; } };
struct CInfo { long RefCount; };
struct CCountedPointer {
    CComponent* Data; CInfo* Info;
    CCountedPointer(CComponent* d) : Data(d), Info(d ? new CInfo() : 0) { if (Info) Info->RefCount = 1; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info && --Info->RefCount == 0) { delete Data; delete Info; } }
};
static CComponent* CreateComponent(const char* def) { CComponent* c = new CComponent(); std::strcpy(c->def, def); c->textId = -1; return c; }
struct CDataBank { int GetTextBySymbol(const char* s) const { return (int)std::strlen(s) * 10; } };
static CDataBank g_bank;
static CCountedPointer CreateMenuDescPC(const char* symbol)
{
    CCountedPointer description(CreateComponent("PC_DESCRIPTION"));
    description.Data->SetText(g_bank.GetTextBySymbol(symbol), true, true);
    return description;
}
int main()
{
    CCountedPointer p = CreateMenuDescPC("TEXT_SWORD");
    if (std::strcmp(p.Data->def, "PC_DESCRIPTION") != 0 || p.Data->textId != 100 || !p.Data->a || !p.Data->b || p.Info->RefCount != 1) {
        std::printf("00582673_TEST FAIL\n"); return 1;
    }
    std::printf("00582673_TEST PASS\n");
    return 0;
}