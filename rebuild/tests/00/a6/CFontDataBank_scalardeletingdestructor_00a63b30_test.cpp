#include <cstdio>
#include <cstdlib>
// Behaviour test for CFontDataBank scalar deleting destructor (0x00A63B30): CloseFonts, then the string, the
// array block (freed when set), the state-block member, then the CBankFile base.
static int g_order[8]; static int g_n = 0; static int g_freed = 0;
class CCharString { public: void* m_pData; ~CCharString() { g_order[g_n++] = 4; } };
class CBankFile { public: virtual ~CBankFile() { g_order[g_n++] = 1; } };
class CBankStateBlock { public: virtual void v00() { } ~CBankStateBlock() { g_order[g_n++] = 2; } };
struct CFontArray {
    void* m_pData; long m_size; long m_capacity;
    ~CFontArray() { g_order[g_n++] = 3; if (m_pData) { std::free(m_pData); ++g_freed; } }
};
class CFontDataBank : public CBankFile { public:
    CBankStateBlock m_stateBlock; CFontArray m_fonts; CCharString m_name;
    void CloseFonts() { g_order[g_n++] = 5; }
    virtual ~CFontDataBank() { CloseFonts(); }
};
int main()
{
    CFontDataBank* b = new CFontDataBank;
    b->m_fonts.m_pData = std::malloc(8);
    CBankFile* p = b;
    delete p;
    int want[5] = { 5, 4, 3, 2, 1 };
    if (g_n != 5 || g_freed != 1) { std::printf("00a63b30_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (g_order[i] != want[i]) { std::printf("00a63b30_TEST FAIL\n"); return 1; }
    std::printf("00a63b30_TEST PASS\n");
    return 0;
}