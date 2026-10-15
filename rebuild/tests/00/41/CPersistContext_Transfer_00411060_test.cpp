#include <cstdio>
#include <cstring>
// CPersistContext::Transfer(name, CWideString&): the default is a copy of a default-constructed CWideString (helper with a named
// return); the temporary dies before the call, the default after it
static long g_ctor, g_copy, g_dtor, g_calls; static bool g_defaultEmpty;
struct CWideString {
    int m_len;
    CWideString() : m_len(0) { ++g_ctor; }
    CWideString(const CWideString& other) : m_len(other.m_len) { ++g_copy; }
    ~CWideString() { ++g_dtor; }
};
struct CPersistContext {
    long pad[11];
    void Transfer(const char* name, CWideString& value, const CWideString& def) { ++g_calls; g_defaultEmpty = def.m_len == 0 && &def != &value && std::strcmp(name, "Name") == 0 && g_ctor + g_copy - g_dtor == 2; }
    static CWideString DefaultValue() { CWideString value; return value; }
    void Transfer(const char* name, CWideString& value) { CWideString defaultValue = DefaultValue(); Transfer(name, value, defaultValue); }
};
int main()
{
    long ctor0, copy0, dtor0;
    {
        CPersistContext c; CWideString v; v.m_len = 4;
        ctor0 = g_ctor; copy0 = g_copy; dtor0 = g_dtor;
        c.Transfer("Name", v);
        if (v.m_len != 4) { std::printf("00411060_TEST FAIL\n"); return 1; }
    }
    // one default constructed and one copied (or the copy elided by the host compiler), all destroyed again
    if (g_calls != 1 || !g_defaultEmpty || g_ctor + g_copy != g_dtor || g_ctor - ctor0 != 1 || dtor0 != 0) { std::printf("00411060_TEST FAIL\n"); return 1; }
    std::printf("00411060_TEST PASS\n");
    return 0;
}