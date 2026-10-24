// std::_Sort_heap(SNamedVector* first, SNamedVector* last, CCompareNamedVector pred) -- Dinkumware _Sort_heap over 16-byte (CCharString, C3DVector) pairs: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F190, length 209 bytes (+ int3 padding), `ret 4`.
// Identity: catalog `_Sort_heap<pair<CCharString,C3DVector>*,CKeyPairCompareLess<CCharString,C3DVector>>`, consistent
// with the bytes: the callee is the landed `_Adjust_heap` @ 0x0077EA60 over the same 16-byte (CCharString, 3-dword
// payload) record, and the element copy runs the CCharString copy constructor / assignment.
// Callees: _Adjust_heap @ 0x0077EA60 (out of line, __fastcall: first in ecx, hole 0 in edx), CCharString
// copy constructor @ 0x0099EC30 (x2), CCharString::operator= @ 0x0099EFB0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: /Gr Dinkumware TU with _Pop_heap_0 / _Pop_heap inline and the loop calling _Pop_heap_0 directly
// (16-byte stride: `and -16 ; cmp 16`). `_Ty(*(_Last - 1))` is a frame temporary built by the pair's memberwise
// user copy constructor (string copy, then the payload's three dwords), `*_Dest = *_First` is the pair's
// implicit assignment (CCharString::operator=, then three dword moves), the value argument is copied from the
// temporary, and the temporary's string is destroyed after the _Adjust_heap call (no EH frame: no /EHsc).
// Element model from the landed 0x0077EA60 source (the vector payload as three dwords: the copies never touch
// the x87 stack).
class CCharString {
public:
    void* data;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct C3DVector {                          // 12 bytes, copied as three dwords
    int data[3];
    C3DVector(const C3DVector& o) { data[0] = o.data[0]; data[1] = o.data[1]; data[2] = o.data[2]; }
};
struct SNamedVector {                       // std::pair<CCharString, C3DVector> (16 bytes)
    CCharString first;                      // +0x00
    C3DVector   second;                     // +0x04
    SNamedVector(const SNamedVector& p) : first(p.first), second(p.second) { }
};
struct CCompareNamedVector {                // CKeyPairCompareLess<CCharString, C3DVector>, one dword by value
    int state;
};

void __fastcall _Adjust_heap(SNamedVector* first, int hole, int bottom, SNamedVector value,
                             CCompareNamedVector pred);   // 0x0077EA60

inline void _Pop_heap(SNamedVector* first, SNamedVector* last, SNamedVector* dest, SNamedVector value,
                      CCompareNamedVector pred, int*)
{
    *dest = *first;
    _Adjust_heap(first, 0, int(last - first), value, pred);
}

inline void _Pop_heap_0(SNamedVector* first, SNamedVector* last, CCompareNamedVector pred, SNamedVector*)
{
    _Pop_heap(first, last - 1, last - 1, SNamedVector(*(last - 1)), pred, (int*)0);
}

void __fastcall _Sort_heap(SNamedVector* first, SNamedVector* last, CCompareNamedVector pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (SNamedVector*)0);
}