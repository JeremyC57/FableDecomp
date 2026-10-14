// CRetailTree_00824fb0::BuyNode(const SValue_00824fb0&) -- allocate one 48-byte red-black tree node and copy-construct the value at +0x10 (the links are left for the caller)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00824FB0, length 76 bytes.
// Callees: malloc @ 0x00BFEA0E. The chunk label ("CCreatureAction_NavToPos::Clone" / "...::Clone") is a propagated STL label: the body is the Dinkumware tree's
// node factory for one value type (value at +0x10 after the four link/colour words), `this` unused. The value type is reconstructed from the copy:
// five words and a three-word struct member (copied from a +0x14 shifted base).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (landed-trusted)
inline void* operator new(unsigned int, void* p) { return p; }   // placement form: the null test on its result is what retail emits
struct C3DVector_00824fb0 { float x, y, z; };   // plain record: copied as a block (three moves through re-based pointers at +0x14)
struct SValue_00824fb0 {                   // five words then the vector, member-wise copy
    long a, b, c, d, e;
    C3DVector_00824fb0 m_vec;
    SValue_00824fb0(const SValue_00824fb0& o) : a(o.a), b(o.b), c(o.c), d(o.d), e(o.e), m_vec(o.m_vec) { }
};
struct SNode_00824fb0 {                     // 48 bytes: colour / parent / left / right, then the value
    unsigned char m_color; unsigned char _pad[3];
    SNode_00824fb0* m_pParent;
    SNode_00824fb0* m_pLeft;
    SNode_00824fb0* m_pRight;
    SValue_00824fb0 m_value;                 // +0x10
};

struct CRetailTree_00824fb0 {
    SNode_00824fb0* BuyNode(const SValue_00824fb0& value);
};

SNode_00824fb0* CRetailTree_00824fb0::BuyNode(const SValue_00824fb0& value)
{
    SNode_00824fb0* node = (SNode_00824fb0*)malloc(sizeof(SNode_00824fb0));
    new (&node->m_value) SValue_00824fb0(value);
    return node;
}