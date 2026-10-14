// CRetailTree_009aa9e0::BuyNode(const SValue_009aa9e0&) -- allocate one 40-byte red-black tree node and copy-construct the value at +0x10 (the links are left for the caller)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AA9E0, length 76 bytes.
// Callees: malloc @ 0x00BFEA0E. The chunk label ("CCreatureAction_NavToPos::Clone" / "...::Clone") is a propagated STL label: the body is the Dinkumware tree's
// node factory for one value type (value at +0x10 after the four link/colour words), `this` unused. The value type is reconstructed from the copy:
// a CCharString (copy constructor out of line) and five words.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (landed-trusted)
inline void* operator new(unsigned int, void* p) { return p; }   // placement form: the null test on its result is what retail emits
struct CCharString {                       // modelled locally: the shared header inlines the copy constructor, retail calls it out of line
    void* m_pStringData;
    CCharString(const CCharString& other);   // 0x0099EC30
};
struct SRecord_009aa9e0 { long a, b, c, d, e; };   // plain 20-byte record: copied as a block (five moves through re-based pointers at +4)
struct SValue_009aa9e0 {                   // a pair of a name and a record: the string copy constructor @ 0x0099EC30 runs out of line, the record follows
    CCharString m_name;
    SRecord_009aa9e0 m_record;
    SValue_009aa9e0(const SValue_009aa9e0& o) : m_name(o.m_name), m_record(o.m_record) { }
};
struct SNode_009aa9e0 {                     // 40 bytes: colour / parent / left / right, then the value
    unsigned char m_color; unsigned char _pad[3];
    SNode_009aa9e0* m_pParent;
    SNode_009aa9e0* m_pLeft;
    SNode_009aa9e0* m_pRight;
    SValue_009aa9e0 m_value;                 // +0x10
};

struct CRetailTree_009aa9e0 {
    SNode_009aa9e0* BuyNode(const SValue_009aa9e0& value);
};

SNode_009aa9e0* CRetailTree_009aa9e0::BuyNode(const SValue_009aa9e0& value)
{
    SNode_009aa9e0* node = (SNode_009aa9e0*)malloc(sizeof(SNode_009aa9e0));
    new (&node->m_value) SValue_009aa9e0(value);
    return node;
}
