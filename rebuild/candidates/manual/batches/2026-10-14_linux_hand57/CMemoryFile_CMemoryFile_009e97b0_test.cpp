#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_base = 0, g_wide = 0, g_access = 0; static unsigned long g_flags = 0; static const void* g_open = 0; struct CBaseNC { virtual void v00() { } CBaseNC() { ++g_base; } }; struct CWideString { void* p; CWideString() : p(0) { ++g_wide; } }; enum FileAccessType { FILE_ACCESS_READ = 1 };
struct CVec { unsigned char* m_first; unsigned char* m_last; unsigned char* m_end; CVec() : m_first(0), m_last(0), m_end(0) { } };
struct CMemoryFile : CBaseNC { virtual void v00() { } long AccessType; long m_8; long Length; CVec m_buffer; bool Opened; CWideString Pathname; void Open(const CWideString& p, FileAccessType a, unsigned long f) { g_open = &p; g_access = a; g_flags = f; }
    CMemoryFile(const CWideString& p, FileAccessType a, unsigned long f) : CBaseNC(), m_buffer(), Pathname() { Open(p, a, f); } };

int main() {
    CWideString name; CMemoryFile f(name, FILE_ACCESS_READ, 7); if (g_base != 1 || g_wide != 2 || f.m_buffer.m_first || f.m_buffer.m_last || f.m_buffer.m_end || g_open != &name || g_access != 1 || g_flags != 7) { std::printf("009e97b0_TEST FAIL\n"); return 1; }
    std::printf("009e97b0_TEST PASS\n"); return 0;
}
