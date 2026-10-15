#include <cstdio>
#include <cstring>
// CPersistContext::Transfer(name, CCharString&): the default is a copy of a default-constructed CCharString (helper with a named
// return); the temporary dies before the call, the default after it
static long g_ctor, g_copy, g_dtor, g_calls; static bool g_defaultEmpty;
struct CCharString {
    int m_len;
    CCharString() : m_len(0) { ++g_ctor; }
    CCharString(const CCharString& other) : m_len(other.m_len) { ++g_copy; }
    ~CCharString() { ++g_dtor; }
};
struct CPersistContext {
    long pad[11];
    void Transfer(const char* name, CCharString& value, const CCharString& def) { ++g_calls; g_defaultEmpty = def.m_len == 0 && &def != &value && std::strcmp(name, "Name") == 0 && g_ctor + g_copy - g_dtor == 2; }
    static CCharString DefaultValue() { CCharString value; return value; }
    void Transfer(const char* name, CCharString& value) { CCharString defaultValue = DefaultValue(); Transfer(name, value, defaultValue); }
};
int main()
{
    long ctor0, copy0, dtor0;
    {
        CPersistContext c; CCharString v; v.m_len = 4;
        ctor0 = g_ctor; copy0 = g_copy; dtor0 = g_dtor;
        c.Transfer("Name", v);
        if (v.m_len != 4) { std::printf("00411010_TEST FAIL\n"); return 1; }
    }
    // one default constructed and one copied (or the copy elided by the host compiler), all destroyed again
    if (g_calls != 1 || !g_defaultEmpty || g_ctor + g_copy != g_dtor || g_ctor - ctor0 != 1 || dtor0 != 0) { std::printf("00411010_TEST FAIL\n"); return 1; }
    std::printf("00411010_TEST PASS\n");
    return 0;
}