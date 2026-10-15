#include <cstdio>
#include <cstring>
#include <cstddef>
// Behaviour test for CNavigatorOwner_00711520::ResetBuffers @ 0x00711520
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)std::memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
struct SWordPair { long m_a, m_b; SWordPair& operator=(const SWordPair& o) { m_a = o.m_a; m_b = o.m_b; return *this; } };
template <class T> inline T* CopyAssign(T* first, T* last, T* result)
{
    for (ptrdiff_t n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
struct CTrivialVector { long* m_pStart; long* m_pFinish; long* m_pEndOfStorage; void clear() { m_pFinish = (long*)CopyTrivial(m_pFinish, m_pFinish, m_pStart); } };
struct CWordPairVector { SWordPair* m_pStart; SWordPair* m_pFinish; SWordPair* m_pEndOfStorage; void clear() { m_pFinish = CopyAssign(m_pFinish, m_pFinish, m_pStart); } };
class CNavigatorOwner_00711520 {
public:
    CTrivialVector m_first; CWordPairVector m_second; long m_countA; long m_countB;
    void ResetBuffers() { m_first.clear(); m_second.clear(); m_countA = 0; m_countB = 0; }
};
int main()
{
    long words[4] = { 1, 2, 3, 4 }; SWordPair pairs[3];
    CNavigatorOwner_00711520 o;
    o.m_first.m_pStart = words; o.m_first.m_pFinish = words + 3; o.m_first.m_pEndOfStorage = words + 4;
    o.m_second.m_pStart = pairs; o.m_second.m_pFinish = pairs + 2; o.m_second.m_pEndOfStorage = pairs + 3;
    o.m_countA = 5; o.m_countB = 6;
    o.ResetBuffers();
    if (o.m_first.m_pFinish != words || o.m_second.m_pFinish != pairs || o.m_countA != 0 || o.m_countB != 0 ||
        o.m_first.m_pEndOfStorage != words + 4 || words[2] != 3) { std::printf("00711520_TEST FAIL\n"); return 1; }
    long a[3] = { 7, 8, 9 }, b[3] = { 0, 0, 0 };
    if ((long*)CopyTrivial(a, a + 3, b) != b + 3 || b[2] != 9) { std::printf("00711520_TEST FAIL\n"); return 1; }
    std::printf("00711520_TEST PASS\n");
    return 0;
}