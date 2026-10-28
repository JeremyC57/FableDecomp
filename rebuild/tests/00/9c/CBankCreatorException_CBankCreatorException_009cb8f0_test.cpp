// Behaviour test for 0x009CB8F0 CBankCreatorException::CBankCreatorException(entry_symbol, error_text).
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    static int s_copies;
    static int s_assigns;
    CCharString(const char* s) : PData(s) {}
    CCharString(const CCharString& o) : PData(o.PData) { ++s_copies; }
    const CCharString& operator=(const CCharString& o) { PData = o.PData; ++s_assigns; return *this; }
};
int CCharString::s_copies = 0;
int CCharString::s_assigns = 0;
static bool Same(const CCharString& s, const char* t) { return std::strcmp(s.PData, t) == 0; }

class CBankCreatorException {
public:
    CCharString EntrySymbol;
    CCharString ErrorText;
    CBankCreatorException(const CCharString& entry_symbol, const CCharString& error_text)
        : EntrySymbol(entry_symbol), ErrorText(error_text) {}
};

int main()
{
    CCharString::s_copies = 0;
    CBankCreatorException e(CCharString("MESH_HERO"), CCharString("missing bank entry"));
    bool caught = false;
    try { throw e; } catch (const CBankCreatorException& x) { caught = Same(x.EntrySymbol, "MESH_HERO"); }
    if (!(caught && CCharString::s_copies >= 2 && Same(e.EntrySymbol, "MESH_HERO") && Same(e.ErrorText, "missing bank entry"))) {
        std::printf("009cb8f0_TEST FAIL\n");
        return 1;
    }
    std::printf("009cb8f0_TEST PASS\n");
    return 0;
}