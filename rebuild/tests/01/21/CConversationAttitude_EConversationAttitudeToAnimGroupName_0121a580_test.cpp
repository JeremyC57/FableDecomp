// Test for CConversationAttitude::$E_ConversationAttitudeToAnimGroupName (0x0121A580): the dynamic initializer builds the static
// std::vector<CCharString> ConversationAttitudeToAnimGroupName as vector(12, CCharString("")): one allocation of exactly twelve slots,
// each copy-constructed from the empty temporary, then the temporary is destroyed and the atexit destructor registered.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
static int g_live = 0, g_copies = 0, g_atexit = 0;
class CCharString { public:
    char* p;
    CCharString(const char* s, long len = -1) { if (len < 0) len = (long)std::strlen(s); p = (char*)std::malloc(len + 1); std::memcpy(p, s, len); p[len] = 0; ++g_live; }
    CCharString(const CCharString& o) { long n = (long)std::strlen(o.p); p = (char*)std::malloc(n + 1); std::memcpy(p, o.p, n + 1); ++g_live; ++g_copies; }
    ~CCharString() { std::free(p); --g_live; }
    bool IsEmpty() const { return p[0] == 0; }
};
struct VectorCCharString {
    CCharString* m_start; CCharString* m_finish; CCharString* m_end_of_storage;
    VectorCCharString(unsigned n, const CCharString& val)
    {
        m_start = (CCharString*)std::malloc(n * sizeof(CCharString)); m_finish = m_start; m_end_of_storage = m_start + n;
        for (CCharString* q = m_start; n != 0; --n, ++q) new (q) CCharString(val);
        m_finish = m_end_of_storage;
    }
    ~VectorCCharString() { for (CCharString* q = m_start; q != m_finish; ++q) q->~CCharString(); std::free(m_start); }
};
static VectorCCharString* g_vec = 0;
static void Destroy() { ++g_atexit; delete g_vec; g_vec = 0; }
static void Initialize() { g_vec = new VectorCCharString(12, CCharString("")); }
int main()
{
    Initialize();
    if (g_vec->m_finish - g_vec->m_start != 12 || g_vec->m_end_of_storage != g_vec->m_finish) { std::printf("0121a580_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 12; ++i) if (!g_vec->m_start[i].IsEmpty()) { std::printf("0121a580_TEST FAIL\n"); return 1; }
    if (g_live != 12 || g_copies != 12) { std::printf("0121a580_TEST FAIL\n"); return 1; }
    Destroy();
    if (g_live != 0 || g_atexit != 1) { std::printf("0121a580_TEST FAIL\n"); return 1; }
    std::printf("0121a580_TEST PASS\n");
    return 0;
}