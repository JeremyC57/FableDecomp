// Behaviour test for 0x00AC7B00 CVertexPairContraction::CleanUp: duplicates (same Index1 and Index2) removed, first
// occurrence and order kept, adjacent duplicates handled by the j step-back.
#include <cstdio>
template <class T> class CArray { public:
    T* m_first; T* m_last; T* m_end;
    T* begin() const { return m_first; }
    T* end() const { return m_last; }
    unsigned long size() const { return (unsigned long)(m_last - m_first); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
    static T* copy(T* first, T* last, T* result)
    { for (long n = (long)(last - first); n > 0; --n) { *result = *first; ++first; ++result; } return result; }
    T* erase(T* position) { if (position + 1 != end()) copy(position + 1, m_last, position); --m_last; return position; }
};
struct CSubContraction { unsigned long Index1, Index2; };
struct CVertexPairContraction {
    CArray<CSubContraction> SubContractions;
    void CleanUp()
    {
        for (unsigned long i = 0; i < SubContractions.size(); ++i) {
            CSubContraction& a = SubContractions[i];
            for (unsigned long j = i + 1; j < SubContractions.size(); ++j) {
                CSubContraction& b = SubContractions[j];
                if (a.Index1 == b.Index1 && a.Index2 == b.Index2) { SubContractions.erase(SubContractions.begin() + j); --j; }
            }
        }
    }
};
int main()
{
    CSubContraction d[8] = { {1,2}, {3,4}, {1,2}, {1,2}, {2,1}, {3,4}, {5,6}, {1,2} };
    CVertexPairContraction c; c.SubContractions.m_first = d; c.SubContractions.m_last = d + 8; c.SubContractions.m_end = d + 8;
    c.CleanUp();
    CSubContraction want[4] = { {1,2}, {3,4}, {2,1}, {5,6} };
    bool ok = c.SubContractions.size() == 4;
    for (int i = 0; ok && i < 4; ++i) ok = d[i].Index1 == want[i].Index1 && d[i].Index2 == want[i].Index2;
    CVertexPairContraction e; e.SubContractions.m_first = e.SubContractions.m_last = d; e.CleanUp();
    ok = ok && e.SubContractions.size() == 0;
    if (!ok) { std::printf("00ac7b00_TEST FAIL\n"); return 1; }
    std::printf("00ac7b00_TEST PASS\n");
    return 0;
}