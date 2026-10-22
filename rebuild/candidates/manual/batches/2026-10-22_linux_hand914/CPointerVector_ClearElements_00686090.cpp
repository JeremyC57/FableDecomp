// CPointerVector::ClearElements() -- sets every element of an STLport vector of pointers to null, re-reading size() each iteration
// Leaf: ClearElements
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686090, 41 bytes. No callees. Unreferenced in the image (recovered
// from a mis-cut catalog row); the element type is not identified (4-byte pointer or long).
// Shape: `for (unsigned i = 0; i < size(); ++i) (*this)[i] = 0;` -- unsigned compare (`jbe`/`jb`), size()
// as `(finish - start) >> 2` recomputed after each store, operator[] as `*(begin() + i)`.
struct CPointerVector {
    void** _M_start;                  // +0
    void** _M_finish;                 // +4
    void** _M_end_of_storage;         // +8
    unsigned int size() const { return _M_finish - _M_start; }
    void*& operator[](unsigned int n) { return *(_M_start + n); }
    void ClearElements();
};

void CPointerVector::ClearElements()
{
    for (unsigned int i = 0; i < size(); ++i)
        (*this)[i] = 0;
}
