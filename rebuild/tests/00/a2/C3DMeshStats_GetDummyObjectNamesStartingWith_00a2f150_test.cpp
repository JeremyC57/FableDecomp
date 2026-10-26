// Behaviour test for 0x00A2F150 C3DMeshStats::GetDummyObjectNamesStartingWith: forwards to UnpackNames with dummyObjects = true.
#include <cstdio>

class CCharString { public: const char* m_text; };
struct CCharStringVector { int m_count; };

static CCharStringVector* g_names = 0;
static bool g_dummy = false;
static const CCharString* g_prefix = 0;
static bool g_once = false;
static int g_calls = 0;

class C3DMeshStats { public:
    int m_id;
    void GetDummyObjectNamesStartingWith(CCharStringVector& names, const CCharString& prefix, bool once) const
    {
        UnpackNames(names, true, prefix, once);
    }
    void UnpackNames(CCharStringVector& names, bool dummyObjects, const CCharString& prefix, bool once) const
    {
        ++g_calls;
        g_names = &names;
        g_dummy = dummyObjects;
        g_prefix = &prefix;
        g_once = once;
        names.m_count += once ? 1 : 2;
    }
};

int main()
{
    C3DMeshStats stats;
    stats.m_id = 7;
    CCharStringVector names;
    names.m_count = 0;
    CCharString prefix;
    prefix.m_text = "DO_";
    stats.GetDummyObjectNamesStartingWith(names, prefix, false);
    if (g_calls != 1 || g_names != &names || !g_dummy || g_prefix != &prefix || g_once || names.m_count != 2) {
        std::printf("00a2f150_TEST FAIL\n");
        return 1;
    }
    stats.GetDummyObjectNamesStartingWith(names, prefix, true);
    if (g_calls != 2 || !g_dummy || !g_once || names.m_count != 3) {
        std::printf("00a2f150_TEST FAIL\n");
        return 1;
    }
    std::printf("00a2f150_TEST PASS\n");
    return 0;
}