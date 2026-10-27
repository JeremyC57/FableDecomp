// CArray<CUnprovenElement4_00be8b00>::size() const -- element count of a CArray (STLport vector) of 4-byte elements
// Leaf: ?size@?$CArray@UCUnprovenElement4_00be8b00@@@@QBEIXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE8B00, 9 bytes, thiscall: `(_M_finish - _M_start) >> 2` (signed `sar 2`).
// Identity (descriptive): an out-of-line CArray<T>::size() COMDAT for a 4-byte element type (a pointer or handle vector);
//   no direct call reaches it (the xref hits are bytes inside other instructions), so the element type is unrecovered.
struct CUnprovenElement4_00be8b00 {            // 4 bytes (the `sar 2` stride)
    unsigned long Word;
};

template <class T> class CArray {               // retail CArray: the STLport 4.6 vector words
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
};

template unsigned int CArray<CUnprovenElement4_00be8b00>::size() const;