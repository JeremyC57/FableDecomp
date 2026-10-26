// std::_Sort_heap(SKeyedVector* first, SKeyedVector* last, SKeyLess pred) -- Dinkumware _Sort_heap over 16-byte (unsigned long key, C3DVector) pairs: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076AF10, length 144 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned __int64,CANavQuadTreeNode*>*,...>` is propagated over the
// byte-identical skeleton. The only callee is the landed Dinkumware `_Adjust_heap` @ 0x00769D50 for 16-byte
// (unsigned long key, C3DVector) pairs with an unsigned key less; the element model is copied from it.
// Callees: _Adjust_heap @ 0x00769D50 (out of line, __fastcall: first in ecx, hole 0 in edx, `ret 18h`).
// Shape: /Gr Dinkumware TU with _Pop_heap_0 / _Pop_heap inline and the loop calling _Pop_heap_0 directly
// (16-byte stride: the count test is `and eax,-16 ; cmp eax,16`). The `_Ty(*(_Last - 1))` temporary stays in
// registers (key in ecx, X/Y on the x87 stack, Z spilled to the frame) because both copy constructors are
// memberwise user constructors; `*_Dest = *_First` is the implicit assignment (four dword moves) and the value
// argument is stored field by field from the temporary.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};

struct SKeyedVector {
    unsigned long m_key;
    C3DVector m_value;
    SKeyedVector(const SKeyedVector& other) : m_key(other.m_key), m_value(other.m_value) { }
};

struct SKeyLess {
    char m_tag;
    bool operator()(const SKeyedVector& a, const SKeyedVector& b) const { return a.m_key < b.m_key; }
};

void __fastcall _Adjust_heap(SKeyedVector* first, long hole, long bottom, SKeyedVector value, SKeyLess pred);   // 0x00769D50

inline void _Pop_heap(SKeyedVector* first, SKeyedVector* last, SKeyedVector* dest, SKeyedVector value,
                      SKeyLess pred, long*)
{
    *dest = *first;
    _Adjust_heap(first, 0, long(last - first), value, pred);
}

inline void _Pop_heap_0(SKeyedVector* first, SKeyedVector* last, SKeyLess pred, SKeyedVector*)
{
    _Pop_heap(first, last - 1, last - 1, SKeyedVector(*(last - 1)), pred, (long*)0);
}

void __fastcall _Sort_heap(SKeyedVector* first, SKeyedVector* last, SKeyLess pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (SKeyedVector*)0);
}