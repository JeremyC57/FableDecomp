// Behaviour test for 0x00E403D0 CBordelloLady::GetLHTSTag (NF388 literal audit): links the row object; the three CCharString
// operator+ overloads are captured. Asserts the prefix literal is "TEXT_QST_B13_" (retail operand 0x012E4430; the HEAD row passed
// "LHTS_"), the separator is "_", and the concatenation order: prefix + Name, then + "_", then + suffix.
#include <cstdio>
#include <cstring>
class CCharString { public:
    void* m_pStringData;
    ~CCharString();
};
CCharString __fastcall operator+(const char* left, const CCharString& right);
CCharString __fastcall operator+(const CCharString& left, const char* right);
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);
struct CBordelloLady {
    unsigned char _pad[0x28];
    CCharString   Name;
    CCharString GetLHTSTag(const CCharString& suffix);
};
static int g_step = 0; static const char* g_prefix = 0; static const char* g_sep = 0;
static const void* g_name = 0; static const void* g_suffix = 0; static bool g_order = true;
static CCharString Make(int tag) { CCharString r; r.m_pStringData = (void*)(long)tag; return r; }
CCharString::~CCharString() {}
CCharString __fastcall operator+(const char* left, const CCharString& right)
{ if (g_step++ != 0 || &right != g_name) g_order = false; g_prefix = left; return Make(1); }
CCharString __fastcall operator+(const CCharString& left, const char* right)
{ if (g_step++ != 1 || left.m_pStringData != (void*)1) g_order = false; g_sep = right; return Make(2); }
CCharString __fastcall operator+(const CCharString& left, const CCharString& right)
{ if (g_step++ != 2 || left.m_pStringData != (void*)2 || &right != g_suffix) g_order = false; return Make(3); }
int main()
{
    static CBordelloLady lady; lady.Name.m_pStringData = 0;
    CCharString suffix; suffix.m_pStringData = 0;
    g_name = &lady.Name; g_suffix = &suffix;
    {
        CCharString r = lady.GetLHTSTag(suffix);
        if (!g_order || g_step != 3 || r.m_pStringData != (void*)3 || !g_prefix || std::strcmp(g_prefix, "TEXT_QST_B13_") != 0
            || !g_sep || std::strcmp(g_sep, "_") != 0) {
            std::printf("FAIL_00e403d0 step=%d prefix=%s\n", g_step, g_prefix ? g_prefix : "(null)"); return 1;
        }
    }
    std::printf("00e403d0_TEST PASS\n");
    return 0;
}
