// Behaviour test for the 0x012B4F74-vtable SetupEventNotification (0x00C3C4E0): caller buffers are adopted
// (owned bit cleared, old owned buffer freed), a missing buffer reuses a same-size owned one or allocates; the
// 0x80 flag mirrors the previous descriptor's 0x80000000 mode; allocation failure returns -5.
#include <cstdio>
#include <cstdlib>
#include <cstddef>

static int g_allocs = 0, g_frees = 0; static bool g_fail = false;
void* LibAlloc_00c3c360(unsigned long size) { if (g_fail) return 0; ++g_allocs; return std::malloc(size); }
void LibFree_00c3c370(void* block) { ++g_frees; std::free(block); }
struct SNotificationDesc { unsigned long Mode, Reserved04, Reserved08, Size, Reserved10, Reserved14; void* Buffer; };
class CVtable_012b4f74 {
public:
    static void* operator new(size_t size) { return LibAlloc_00c3c360((unsigned long)size); }
    static void operator delete(void* block) { LibFree_00c3c370(block); }
    SNotificationDesc m_desc; unsigned long m_flags;
    long SetupEventNotification(const SNotificationDesc* desc);
};
long CVtable_012b4f74::SetupEventNotification(const SNotificationDesc* desc)
{
    void* buffer = desc->Buffer;
    if (buffer == 0) {
        if ((m_flags & 2) && desc->Size == m_desc.Size) {
            buffer = m_desc.Buffer;
        } else {
            if (m_flags & 2)
                operator delete(m_desc.Buffer);
            buffer = operator new(desc->Size);
            if (buffer == 0)
                return -5;
        }
        m_flags |= 2;
    } else {
        if (m_flags & 2)
            operator delete(m_desc.Buffer);
        m_flags &= ~2;
    }
    if (m_desc.Mode == 0x80000000)
        m_flags |= 0x80;
    else
        m_flags &= 0x7FFFFFFF;
    m_desc = *desc;
    m_desc.Buffer = buffer;
    return 0;
}
int main()
{
    CVtable_012b4f74 n; n.m_flags = 0; n.m_desc.Mode = 0x80000000; n.m_desc.Size = 0; n.m_desc.Buffer = 0;
    SNotificationDesc d = { 1, 0, 0, 16, 0, 0, 0 };
    if (n.SetupEventNotification(&d) != 0 || g_allocs != 1 || !(n.m_flags & 2) || !(n.m_flags & 0x80) || n.m_desc.Size != 16 || !n.m_desc.Buffer) { std::printf("00c3c4e0_TEST FAIL\n"); return 1; }
    void* owned = n.m_desc.Buffer;
    n.m_flags |= 0x80000000;   // the else arm only clears bit 31 (the 0x80 bit set earlier stays)
    if (n.SetupEventNotification(&d) != 0 || g_allocs != 1 || n.m_desc.Buffer != owned || !(n.m_flags & 0x80) || (n.m_flags & 0x80000000)) { std::printf("00c3c4e0_TEST FAIL\n"); return 1; }
    char external[8]; d.Buffer = external;
    if (n.SetupEventNotification(&d) != 0 || g_frees != 1 || (n.m_flags & 2) || n.m_desc.Buffer != external) { std::printf("00c3c4e0_TEST FAIL\n"); return 1; }
    d.Buffer = 0; g_fail = true;
    if (n.SetupEventNotification(&d) != -5) { std::printf("00c3c4e0_TEST FAIL\n"); return 1; }
    std::printf("00c3c4e0_TEST PASS\n");
    return 0;
}