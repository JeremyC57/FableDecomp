// CNavigatorOwner_00711520::ResetBuffers() -- empty two STLport vectors (a trivially copied one at +4, one of 8-byte records at +0x10) and zero the two counters at +0x2C / +0x30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711520, length 96 bytes. Callee: memmove @ 0x00BFEAE6.
// The manifest label (CNearestBuildingWithProperty::CNearestBuildingWithProperty) is propagated and wrong: the body is a
// reset of two vectors, called first thing by the navigator-queue owner's update @ 0x0070EC30 (which then clears its
// navigator-queue lists at +0x54 / +0x58 / +0x60 through the landed 0x0070B8A0 / 0x0070BA00); named by effect.
// Shape: STLport `vector::clear()` = `erase(begin(), end())`: for the trivial vector `__copy_trivial(finish, finish,
// start)` (`cmp eax,eax` on the twice-read finish pointer, memmove + byte count otherwise), for the record vector
// the SGI random-access `__copy` loop (`n = (last - first) >> 3 ; n > 0`, one word pair per pass through ebx); the
// finish pointer is stored back, then the counters take the loop's zero register (edi).
// Leaf: ResetBuffers
#include <stddef.h>
extern "C" void* __cdecl memmove(void* destination, const void* source, size_t size);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

struct SWordPair {                                 // 8-byte element with a user assignment
    long m_a;
    long m_b;
    SWordPair& operator=(const SWordPair& other) { m_a = other.m_a; m_b = other.m_b; return *this; }
};

template <class T> inline T* CopyAssign(T* first, T* last, T* result)
{
    for (ptrdiff_t n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

struct CTrivialVector {                            // STLport vector of a trivially assignable type
    long* m_pStart;
    long* m_pFinish;
    long* m_pEndOfStorage;
    void clear()
    {
        long* i = (long*)CopyTrivial(m_pFinish, m_pFinish, m_pStart);
        m_pFinish = i;
    }
};

struct CWordPairVector {                           // STLport vector<SWordPair>
    SWordPair* m_pStart;
    SWordPair* m_pFinish;
    SWordPair* m_pEndOfStorage;
    void clear()
    {
        SWordPair* i = CopyAssign(m_pFinish, m_pFinish, m_pStart);
        m_pFinish = i;
    }
};

class CNavigatorOwner_00711520 {
public:
    unsigned long   m_unknown0;                    // +0x00
    CTrivialVector  m_first;                       // +0x04
    CWordPairVector m_second;                      // +0x10
    unsigned char   _pad1c[0x10];
    long            m_countA;                      // +0x2C
    long            m_countB;                      // +0x30
    void ResetBuffers();
};

void CNavigatorOwner_00711520::ResetBuffers()
{
    m_first.clear();
    m_second.clear();
    m_countA = 0;
    m_countB = 0;
}
