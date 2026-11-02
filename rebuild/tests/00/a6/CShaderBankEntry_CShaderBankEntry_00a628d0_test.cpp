// Behaviour test for 0x00A628D0 CShaderBankEntry::~CShaderBankEntry (compiler-generated): VSConstantLayout first, then every
// ShaderConstants string in order and its block, then the ShaderCode block, then the base.
#include <cstdio>
#include <cstdlib>
#include <new>
static char g_log[32];
static int g_n = 0;
static void Log(char c) { if (g_n < 31) g_log[g_n++] = c; g_log[g_n] = 0; }

class CCharString { public: char Tag; ~CCharString() { Log(Tag); } };
template <class T> class CArray { public:
    T* Start; T* Finish; T* End; char Tag;
    ~CArray()
    {
        for (T* p = Start; p != Finish; ++p) p->~T();
        if (Start) { Log(Tag); std::free(Start); }
    }
};
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { Log('B'); } };
class CShaderBankEntry : public CBaseClassNonCopyable { public:
    unsigned long ShaderType;
    CArray<unsigned long> ShaderCode;
    CArray<CCharString> ShaderConstants;
    CCharString VSConstantLayout;
};

int main()
{
    CShaderBankEntry* e = new CShaderBankEntry;
    e->VSConstantLayout.Tag = 'L';
    e->ShaderCode.Start = (unsigned long*)std::malloc(8); e->ShaderCode.Finish = e->ShaderCode.Start + 2; e->ShaderCode.Tag = 'K';
    CCharString* s = (CCharString*)std::malloc(2 * sizeof(CCharString));
    new (&s[0]) CCharString(); s[0].Tag = 'x'; new (&s[1]) CCharString(); s[1].Tag = 'y';
    e->ShaderConstants.Start = s; e->ShaderConstants.Finish = s + 2; e->ShaderConstants.Tag = 'C';
    delete (CBaseClassNonCopyable*)e;
    const char* want = "LxyCKB";
    for (int i = 0; want[i] || g_log[i]; ++i)
        if (want[i] != g_log[i]) { std::printf("00a628d0_TEST FAIL\n"); return 1; }
    std::printf("00a628d0_TEST PASS\n");
    return 0;
}