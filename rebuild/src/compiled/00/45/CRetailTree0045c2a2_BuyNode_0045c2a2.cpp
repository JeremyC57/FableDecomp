// CRetailTree_0045c2a2::BuyNode(const SValue_0045c2a2&) -- allocate one 36-byte red-black tree node and copy-construct the value at +0x10 (the links are left for the caller)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C2A2, length 40 bytes.
// Callees: malloc @ 0x00BFEA0E. The chunk label ("CCreatureAction_NavToPos::Clone" / "...::Clone") is a propagated STL label: the body is the Dinkumware tree's
// node factory for one value type (value at +0x10 after the four link/colour words), `this` unused. The value type is reconstructed from the copy:
// a key word and a 16-byte block (array member).
#pragma optimize("s", on)   // the 0x0045 module is size-optimised: `pop ecx` after malloc
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (landed-trusted)
inline void* operator new(unsigned int, void* p) { return p; }   // placement form: the null test on its result is what retail emits
struct SBlock16_0045c2a2 { long v[4]; };
struct SValue_0045c2a2 {                   // a pair: explicit member-wise copy, the 16-byte block member copied as a block (movsd x4)
    long m_key;
    SBlock16_0045c2a2 m_data;
    SValue_0045c2a2(const SValue_0045c2a2& o) : m_key(o.m_key), m_data(o.m_data) { }
};
struct SNode_0045c2a2 {                     // 36 bytes: colour / parent / left / right, then the value
    unsigned char m_color; unsigned char _pad[3];
    SNode_0045c2a2* m_pParent;
    SNode_0045c2a2* m_pLeft;
    SNode_0045c2a2* m_pRight;
    SValue_0045c2a2 m_value;                 // +0x10
};

struct CRetailTree_0045c2a2 {
    SNode_0045c2a2* BuyNode(const SValue_0045c2a2& value);
};

SNode_0045c2a2* CRetailTree_0045c2a2::BuyNode(const SValue_0045c2a2& value)
{
    SNode_0045c2a2* node = (SNode_0045c2a2*)malloc(sizeof(SNode_0045c2a2));
    new (&node->m_value) SValue_0045c2a2(value);
    return node;
}