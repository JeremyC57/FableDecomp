// _global::_Adjust_heap(SKeyedVector*, long, long, SKeyedVector, SKeyLess) -- Dinkumware heap sift-down for 16-byte (unsigned long key, C3DVector) pairs ordered by an unsigned key less, then _Push_heap @ 0x00769470
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769d50, 169 bytes.
// Callee: 0x00769470 _Push_heap (same instantiation).  first in ecx, hole in edx (link-time
// __fastcall); bottom, the 16-byte pair by value (built in its argument slot: `sub esp,10h ;
// mov eax,esp` then four dword stores = user copy constructor) and the empty comparator follow
// (`ret 18h`).
// Shape: VC7.1 <algorithm> `_Adjust_heap` with a 16-byte stride (`shl esi,4`); the element
// assignment is four dword moves (implicit operator=), the compare an UNSIGNED `jae` on the key.
// Label: the propagated pair<unsigned long, C3DHelperPoint> is 20 bytes (Ego_r.pdb C3DHelperPoint =
// C3DVector Position + long BoneIndex = 16), which is the 20-stride twin 0x00A32440; this 16-stride
// body carries a 12-byte payload, modelled as a C3DVector.  The argument-slot copy stores four
// dwords straight off eax, so C3DVector has a memberwise user copy constructor (an implicit one
// copies the vector as a block through `add eax,4`).
namespace FableHeap_00769d50 {

struct C3DVector {
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

void __fastcall _Push_heap(SKeyedVector* first, long hole, long top, SKeyedVector value, SKeyLess pred);   // 0x00769470

void __fastcall _Adjust_heap(SKeyedVector* first, long hole, long bottom, SKeyedVector value, SKeyLess pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}

}