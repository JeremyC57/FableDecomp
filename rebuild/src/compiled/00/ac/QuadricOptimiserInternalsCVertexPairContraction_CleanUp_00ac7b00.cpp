// QuadricOptimiserInternals::CVertexPairContraction::CleanUp() -- removes duplicate sub-contractions: for each pair, every later pair with the same Index1/Index2 is erased from SubContractions
// Leaf: CleanUp
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC7B00, 188 bytes, plain `ret`.
//   PDB name ?CleanUp@CVertexPairContraction@QuadricOptimiserInternals@@QAEXXZ.
// Callees: none (STLport vector::erase and the SGI random-access copy loop are inline).
// Layout (retail; landed COptimisedPrimitive::PerformContraction 0x00ACD840 model): Exists +0, Unsplit +1,
//   SubContractions CArray<CSubContraction> +0x04 (first +4, last +8, end +0xc), 8-byte {Index1, Index2} elements.
// Shape: two index loops re-reading size() every pass; `a` is a reference to element i taken once per outer pass;
//   erase(begin() + j) copies the tail down element by element (STLport __copy_ptrs with __false_type: SGI
//   `for (n = last - first; n > 0; --n)`), decrements finish, and the loop steps j back.
template <class T> class CArray { public:                 // retail CArray (STLport vector): first / last / end
    T* m_first;
    T* m_last;
    T* m_end;
    T* begin() const { return m_first; }
    T* end() const { return m_last; }
    unsigned long size() const { return m_last - m_first; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
    static T* copy(T* first, T* last, T* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    T* erase(T* position)
    {
        if (position + 1 != end())
            copy(position + 1, m_last, position);
        --m_last;
        return position;
    }
};
namespace QuadricOptimiserInternals {
class CSubContraction { public:
    unsigned long Index1;                          // +0x00
    unsigned long Index2;                          // +0x04
};
class CVertexPairContraction { public:
    bool                    Exists;                // +0x00
    bool                    Unsplit;               // +0x01
    CArray<CSubContraction> SubContractions;       // +0x04
    float                   Weight2;               // +0x10
    float                   Cost;                  // +0x14
    float                   ResultingPosition[3];  // +0x18
    void*                   ListIterator;          // +0x24
    void CleanUp();
};
void CVertexPairContraction::CleanUp()
{
    for (unsigned long i = 0; i < SubContractions.size(); ++i) {
        CSubContraction& a = SubContractions[i];
        for (unsigned long j = i + 1; j < SubContractions.size(); ++j) {
            CSubContraction& b = SubContractions[j];
            if (a.Index1 == b.Index1 && a.Index2 == b.Index2) {
                SubContractions.erase(SubContractions.begin() + j);
                --j;
            }
        }
    }
}
}