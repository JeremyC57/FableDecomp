// CArray<CUnprovenElement16_00bca080>::operator[](unsigned int i) -- address of element i of a CArray (STLport vector) of 16-byte elements
// Leaf: ??A?$CArray@UCUnprovenElement16_00bca080@@@@QAEAAUCUnprovenElement16_00bca080@@I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCA080, 14 bytes, thiscall, `ret 4`: `_M_start + i*16`.
// Identity (descriptive): an out-of-line CArray<T>::operator[] COMDAT for a 16-byte element type; no direct call reaches it
//   (the xref hits are bytes inside other instructions), so the element type is not recoverable from a caller. The class is
//   named by its address; the 0x00BCA090 neighbour is the same accessor shape for another instantiation.
struct CUnprovenElement16_00bca080 {           // 16 bytes (the `shl eax,4` stride)
    unsigned long Word[4];
};

template <class T> class CArray {               // retail CArray: the STLport 4.6 vector words
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int i) { return _M_start[i]; }
};

template CUnprovenElement16_00bca080& CArray<CUnprovenElement16_00bca080>::operator[](unsigned int);