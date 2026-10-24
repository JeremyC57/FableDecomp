// Behaviour test for 0x004B83E0 vector<CCharString>::_M_allocate_and_copy: allocate n slots, copy-construct
// [first, last) into the front of the block, return the block (0 when n == 0).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) {}
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_copies; }
};

struct CCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString* _M_allocate_and_copy(unsigned int n, const CCharString* first, const CCharString* last)
    {
        CCharString* result = n != 0 ? static_cast<CCharString*>(std::malloc(n * sizeof(CCharString))) : 0;
        CCharString* cur = result;
        for (; first != last; ++first, ++cur)
            new (cur) CCharString(*first);
        return result;
    }
};

int main()
{
    CCharString src[3] = { CCharString("a"), CCharString("bb"), CCharString("ccc") };
    CCharStringVector v = { 0, 0, 0 };
    g_copies = 0;
    CCharString* p = v._M_allocate_and_copy(5, src, src + 3);
    if (p == 0 || g_copies != 3 || p[0].m_pData != src[0].m_pData || p[2].m_pData != src[2].m_pData) {
        std::printf("004b83e0_TEST FAIL\n"); return 1;
    }
    std::free(p);
    g_copies = 0;
    CCharString* q = v._M_allocate_and_copy(0, src, src);
    if (q != 0 || g_copies != 0) { std::printf("004b83e0_TEST FAIL\n"); return 1; }
    std::printf("004b83e0_TEST PASS\n");
    return 0;
}