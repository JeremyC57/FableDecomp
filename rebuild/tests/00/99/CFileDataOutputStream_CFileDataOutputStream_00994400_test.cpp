#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0; static const char* g_text = ""; static long g_len = 0;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct CCharString { void* m_pData; CCharString(const char* text, long length) { g_text = text; g_len = length; m_pData = (void*)text; } };
struct CDataOutputStream : CBase { CCharString m_lineEnding; virtual void v00() { } CDataOutputStream() : CBase(), m_lineEnding("\r\n", -1) { } };
struct CFileDataOutputStream : CDataOutputStream { void* m_pFile; virtual void v00() { } CFileDataOutputStream(void* file) : CDataOutputStream(), m_pFile(file) { } };
int main() {
    int f; CFileDataOutputStream s(&f); if (g_base != 1 || std::strcmp(g_text, "\r\n") || g_len != -1 || s.m_pFile != &f) { std::printf("00994400_TEST FAIL\n"); return 1; }
    std::printf("00994400_TEST PASS\n"); return 0;
}