// CRetailTree_005288f0::BuyNode(const SValue_005288f0&) -- allocate one 40-byte red-black tree node and copy-construct the value at +0x10 (the links are left for the caller)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005288F0, length 64 bytes.
// Callees: malloc @ 0x00BFEA0E. The chunk label ("CCreatureAction_NavToPos::Clone" / "...::Clone") is a propagated STL label: the body is the Dinkumware tree's
// node factory for one value type (value at +0x10 after the four link/colour words), `this` unused. The value type is reconstructed from the copy:
// two words and a four-word struct member (member-wise copy from a shifted base).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (landed-trusted)
inline void* operator new(unsigned int, void* p) { return p; }   // placement form: the null test on its result is what retail emits
struct SInner_005288f0 { long a, b, c, d; };   // plain 16-byte record: the pair's explicit copy copies it as a block (four moves through re-based pointers)
struct SValue_005288f0 {                   // a pair of a key and a record
    long m_key;
    long m_kind;
    SInner_005288f0 m_data;
    SValue_005288f0(const SValue_005288f0& o) : m_key(o.m_key), m_kind(o.m_kind), m_data(o.m_data) { }
};
struct SNode_005288f0 {                     // 40 bytes: colour / parent / left / right, then the value
    unsigned char m_color; unsigned char _pad[3];
    SNode_005288f0* m_pParent;
    SNode_005288f0* m_pLeft;
    SNode_005288f0* m_pRight;
    SValue_005288f0 m_value;                 // +0x10
};

struct CRetailTree_005288f0 {
    SNode_005288f0* BuyNode(const SValue_005288f0& value);
};

SNode_005288f0* CRetailTree_005288f0::BuyNode(const SValue_005288f0& value)
{
    SNode_005288f0* node = (SNode_005288f0*)malloc(sizeof(SNode_005288f0));
    new (&node->m_value) SValue_005288f0(value);
    return node;
}