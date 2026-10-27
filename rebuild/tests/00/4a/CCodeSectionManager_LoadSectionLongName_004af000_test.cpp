// Behaviour test for 004af000 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
#include <cstring>
static int g_Live = 0;
class CCharString { public: char Text[32];
    CCharString(const char* s) { std::strcpy(Text, s); ++g_Live; }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); ++g_Live; }
    ~CCharString() { --g_Live; } };
class CCodeSectionManager { public:
    unsigned int Loaded;
    unsigned int GetHandleFromLongName(CCharString n) { return std::strcmp(n.Text, "Section.Long") == 0 ? 7u : 0u; }
    void LoadSection(unsigned int h) { Loaded = h; }
    void LoadSectionLongName(CCharString n) { LoadSection(GetHandleFromLongName(n)); } };
int main() {
    CCodeSectionManager m; m.Loaded = 0;
    { CCharString name("Section.Long"); m.LoadSectionLongName(name); }
    if (m.Loaded != 7 || g_Live != 0) { std::printf("004af000_TEST FAIL\n"); return 1; }
    { std::printf("004af000_TEST PASS\n"); return 0; }
}