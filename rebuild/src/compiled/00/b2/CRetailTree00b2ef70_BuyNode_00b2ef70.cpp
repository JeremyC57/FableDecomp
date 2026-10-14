// CRetailTree_00b2ef70::BuyNode(const SValue_00b2ef70&) -- allocate one 44-byte red-black tree node and copy-construct the value at +0x10 (the links are left for the caller)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2EF70, length 64 bytes.
// Callees: malloc @ 0x00BFEA0E. The chunk label ("CCreatureAction_NavToPos::Clone" / "...::Clone") is a propagated STL label: the body is the Dinkumware tree's
// node factory for one value type (value at +0x10 after the four link/colour words), `this` unused. The value type is reconstructed from the copy:
// four words and a 12-byte vector whose copy constructor @ 0x005BB346 is called with the +0x10 sub-objects.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (landed-trusted)
inline void* operator new(unsigned int, void* p) { return p; }   // placement form: the null test on its result is what retail emits
struct CRetailVector_00b2ef70 { void* first; void* last; void* end; CRetailVector_00b2ef70(const CRetailVector_00b2ef70& other); };   // 0x005BB346: the vector copy constructor
struct SValue_00b2ef70 { long a, b, c, d; CRetailVector_00b2ef70 m_items; };
struct SNode_00b2ef70 {                     // 44 bytes: colour / parent / left / right, then the value
    unsigned char m_color; unsigned char _pad[3];
    SNode_00b2ef70* m_pParent;
    SNode_00b2ef70* m_pLeft;
    SNode_00b2ef70* m_pRight;
    SValue_00b2ef70 m_value;                 // +0x10
};

struct CRetailTree_00b2ef70 {
    SNode_00b2ef70* BuyNode(const SValue_00b2ef70& value);
};

SNode_00b2ef70* CRetailTree_00b2ef70::BuyNode(const SValue_00b2ef70& value)
{
    SNode_00b2ef70* node = (SNode_00b2ef70*)malloc(sizeof(SNode_00b2ef70));
    new (&node->m_value) SValue_00b2ef70(value);
    return node;
}