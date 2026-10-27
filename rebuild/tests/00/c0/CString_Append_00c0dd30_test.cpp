// behaviour test for 00c0dd30 (standalone copy of the model)
#include <cstdio>
#include <cstring>

static char g_buf[32]; static int g_len = 0;
class CString { public: char* m_pchData;
    void ConcatInPlace(int n, const char* src) { std::memcpy(g_buf + g_len, src, n); g_len += n; g_buf[g_len] = 0; }
    void Append(char ch) { ConcatInPlace(1, &ch); } };

int main()
{
    CString s; s.m_pchData = g_buf; s.Append('h'); s.Append('i');
    if (!(std::strcmp(g_buf, "hi") == 0)) { std::printf("00c0dd30_TEST FAIL\n"); return 1; }
    std::printf("00c0dd30_TEST PASS\n");
    return 0;
}