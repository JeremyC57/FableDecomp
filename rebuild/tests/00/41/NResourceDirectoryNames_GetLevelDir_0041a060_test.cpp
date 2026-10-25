// Behaviour test for NResourceDirectoryNames::GetLevelDir (0x0041A060): returns a CWideString built from L"Data\\Levels\\".
#include <cstdio>

static const wchar_t* g_text = 0;
static int g_ctor = 0;
class CWideString {
public:
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    const wchar_t* Text;
};
CWideString::CWideString(const wchar_t* text) : Text(text) { ++g_ctor; g_text = text; }
CWideString::~CWideString() { }

namespace NResourceDirectoryNames {
CWideString __fastcall GetLevelDir();
}

int main()
{
    CWideString s = NResourceDirectoryNames::GetLevelDir();
    const wchar_t* want = L"Data\\Levels\\";
    int ok = g_ctor == 1 && s.Text == g_text && s.Text != 0;
    for (int i = 0; ok; ++i) {
        if (s.Text[i] != want[i]) ok = 0;
        if (want[i] == 0) break;
    }
    std::printf(ok ? "0041a060_TEST PASS\n" : "0041a060_TEST FAIL\n");
    return ok ? 0 : 1;
}
