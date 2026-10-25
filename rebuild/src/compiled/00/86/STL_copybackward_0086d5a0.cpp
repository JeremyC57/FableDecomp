// std::__copy_backward(SVectorMap* first, SVectorMap* last, SVectorMap* result, const random_access_iterator_tag&, int*) -- SGI counted backward copy of 16-byte CVectorMap records (vector of 12-byte entries, empty comparator, dirty flag): returns the moved-back result
// Leaf: __copy_backward
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086D5A0, 61 bytes, `ret 0Ch` (__fastcall: first in ecx, last in edx, result,
// the iterator-tag reference and the Distance* on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: the 12-byte vector's operator= @ 0x006741E0 (catalog "Vector_Assign"; also called by the landed 0x0086D5E0).
// Callers: the copy_backward front end @ 0x0086D960 and vector<SVectorMap>::_M_fill_insert @ 0x0086E4B0 (which copies
// the fill value through the vector copy constructor 0x00673E10 plus the +0x0D flag byte -- the same 16-byte record).
// Shape: `for (n = last - first; n > 0; --n) *--result = *--last; return result;` -- the element's implicit operator= is
// the vector's out-of-line assignment, nothing for the empty comparator byte at +0x0C, and the Dirty byte at +0x0D
// (scheduled after the `dec ebx`). Same record as the landed 0x0086D5E0 (SPairWithVectorMap18 +8) and 0x006250B0.
// Label correction: the catalog's `_Move<CHeroLogBook::CQuestObjectiveDesc*, ...>` is propagated (CQuestObjectiveDesc is a
// 12-byte record in Ego_r.pdb; this element is 16 bytes); member names are descriptive.
struct SVector_006741e0 {                        // 12 bytes
    void* m_first;
    void* m_last;
    void* m_end;
    SVector_006741e0& operator=(const SVector_006741e0& other);   // 0x006741E0
};

struct CKeyPairCompareLess {
};

struct SVectorMap {                              // CVectorMap<...>: vector, comparator, dirty flag (16 bytes)
    SVector_006741e0    Vector;                  // +0x00
    CKeyPairCompareLess Compare;                 // +0x0c
    bool                Dirty;                   // +0x0d
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SVectorMap* __fastcall __copy_backward(SVectorMap* first, SVectorMap* last, SVectorMap* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}