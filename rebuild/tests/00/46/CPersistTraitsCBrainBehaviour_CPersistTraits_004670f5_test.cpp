// Behaviour test for 0x004670F5 CPersistTraits<CBrainBehaviour>::CPersistTraits(): the default behaviour's Name is set
// from an empty string (SetString sees ""), Priority and ScriptAIPriority are zero, and the "" temporary is destroyed.
#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#endif

static int g_live = 0, g_sets = 0; static const char* g_setText = 0;
struct CCharString {
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { ++g_live; }
    ~CCharString() { --g_live; }
};

class CDefString {
public:
    long Index;
    __forceinline CDefString(const CCharString& name) { SetString(name); }
    void SetString(const CCharString& name) { ++g_sets; g_setText = name.Text; Index = 77; }
};

struct CBrainBehaviour {
    CDefString Name;
    long       Priority;
    long       ScriptAIPriority;
    __forceinline CBrainBehaviour(const CCharString& name) : Name(name), Priority(0), ScriptAIPriority(0) { }
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CBrainBehaviour> {
    CBrainBehaviour DefaultVal;
    CPersistTraits();
};

CPersistTraits<CBrainBehaviour>::CPersistTraits() : DefaultVal("")
{
}

int main()
{
    CPersistTraits<CBrainBehaviour> traits;
    if (g_sets != 1 || g_setText == 0 || g_setText[0] != 0 || g_live != 0 || traits.DefaultVal.Name.Index != 77 ||
        traits.DefaultVal.Priority != 0 || traits.DefaultVal.ScriptAIPriority != 0) {
        std::printf("004670f5_TEST FAIL\n"); return 1;
    }
    std::printf("004670f5_TEST PASS\n");
    return 0;
}