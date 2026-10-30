#include <cstdio>
#include <cstring>

// Behaviour test for 0x00C4C080 CRACBuffer::CRACBuffer: member stores, the "Blocksize <n> not appropriate" warning
// (only for alignments other than 1 that are 0 or not a multiple of 4), and the head / tail padding arithmetic.
static char g_text[256];
static int g_hooks = 0, g_breaks = 0;
static void Hook(const char* message) { ++g_hooks; std::strcpy(g_text, message); }
typedef void (*PFNASSERTHOOK)(const char* message);
static PFNASSERTHOOK g_pfnAssertHook = Hook;

class CString {
public:
    char m_pchData[256];
    CString() { m_pchData[0] = 0; }
    ~CString() { }
    CString& operator<<(const char* text) { std::strcat(m_pchData, text); return *this; }
    CString& operator<<(long value) { char b[32]; std::sprintf(b, "%ld", value); return *this << (const char*)b; }
    CString& operator<<(unsigned long value) { char b[32]; std::sprintf(b, "%lu", value); return *this << (const char*)b; }
};
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };

struct CRACBuffer {
    unsigned char* m_pBase;
    unsigned int   m_size;
    unsigned int   m_offset;
    unsigned int   m_alignment;
    unsigned int   m_headPadding;
    unsigned int   m_tailPadding;
    CRACBuffer(unsigned char* base, unsigned int size, unsigned int offset, unsigned int alignment)
        : m_pBase(base), m_size(size), m_offset(offset), m_alignment(alignment)
    {
        if (alignment != 1 && !(alignment > 0 && (alignment & 3) == 0)) {
            {
                CString line;
                line << ".\\PKRACBuffer.cpp" << "(" << (long)11 << ") : " << "Blocksize " << (unsigned long)alignment << " not appropriate" << "\n";
                if (g_pfnAssertHook)
                    g_pfnAssertHook(line.m_pchData);
            }
            CDebugBreak();
        }
        m_headPadding = m_offset - m_offset / m_alignment * m_alignment;
        m_tailPadding = (m_size + m_headPadding + m_alignment - 1) / m_alignment * m_alignment - m_size - m_headPadding;
    }
};

int main()
{
    int bad = 0;
    unsigned char block[64];
    CRACBuffer a(block, 10, 6, 4);                 // head 2, 2 + 10 = 12 -> tail 0
    bad |= a.m_pBase != block || a.m_size != 10 || a.m_offset != 6 || a.m_alignment != 4;
    bad |= a.m_headPadding != 2 || a.m_tailPadding != 0 || g_hooks != 0 || g_breaks != 0;
    CRACBuffer b(block, 5, 16, 8);                 // head 0, 5 -> 8: tail 3
    bad |= b.m_headPadding != 0 || b.m_tailPadding != 3 || g_hooks != 0;
    CRACBuffer c(block, 7, 3, 1);                  // alignment 1 is allowed: no padding
    bad |= c.m_headPadding != 0 || c.m_tailPadding != 0 || g_hooks != 0;
    CRACBuffer d(block, 4, 5, 6);                  // 6 is not a multiple of 4: warning; head 5, 5 + 4 = 9 -> 12: tail 3
    bad |= g_hooks != 1 || g_breaks != 1 || std::strcmp(g_text, ".\\PKRACBuffer.cpp(11) : Blocksize 6 not appropriate\n") != 0;
    bad |= d.m_headPadding != 5 || d.m_tailPadding != 3;
    g_pfnAssertHook = 0;
    CRACBuffer e(block, 1, 2, 2);                  // warning without a hook still breaks
    bad |= g_hooks != 1 || g_breaks != 2 || e.m_headPadding != 0 || e.m_tailPadding != 1;
    if (bad) { std::printf("00c4c080_TEST FAIL\n"); return 1; }
    std::printf("00c4c080_TEST PASS\n");
    return 0;
}