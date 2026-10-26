// _global::UninitCopy_PairC2DVectorByte_008002f0(const SPairC2DVectorByte* first, const SPairC2DVectorByte* last, SPairC2DVectorByte* result, const __false_type&) -- STLport __uninitialized_copy over 9-byte pair<C2DVector, unsigned char>: placement copy-construct [first, last), return the advanced result
// Leaf: UninitCopy_PairC2DVectorByte_008002f0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008002F0, length 50 bytes, `ret 8`.
// Callers: one site, 0x00800E77 in the vector allocate-and-insert helper 0x00800DA0.
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. The 9-byte
//   stride needs an alignment-1 record: Fable's C2DVector is byte-packed (as C3DVector in CProng, 0x006F14A0), and
//   Ego_r.pdb has exactly such a 9-byte `pair<C2DVector, unsigned char>` (first +0, second +8); CNavNodeInfo
//   {C2DVector Position; bool Navigable} is the other 9-byte candidate. Taken as the pair.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack). STLport's pair has a memberwise user copy constructor, so the two
//   vector words and the byte are moved through edx (`last` lives in esi to free a byte register) instead of a 9-byte
//   block copy; placement-new null test on the destination; the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

#pragma pack(push, 1)
class C2DVector { public:                              // byte-packed 2D vector (alignment 1)
    float X, Y;
};
#pragma pack(pop)

struct SPairC2DVectorByte {                     // std::pair<C2DVector, unsigned char>, 9 bytes
    C2DVector     first;                        // +0x00
    unsigned char second;                       // +0x08
    SPairC2DVectorByte(const SPairC2DVectorByte& o) : first(o.first), second(o.second) { }
};

SPairC2DVectorByte* __fastcall UninitCopy_PairC2DVectorByte_008002f0(const SPairC2DVectorByte* first, const SPairC2DVectorByte* last, SPairC2DVectorByte* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SPairC2DVectorByte(*first);
    return result;
}