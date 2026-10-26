// CRetailTree_00783dd0::BuyNode(const SValue_00783dd0&) -- allocate one 32-byte red-black tree node and copy-construct the value at +0x10 (the links are left for the caller)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00783DD0, length 80 bytes.
// Callees: malloc @ 0x00BFEA0E. The chunk label ("CCreatureAction_NavToPos::Clone" / "...::Clone") is a propagated STL label: the body is the Dinkumware tree's
// node factory for one value type (value at +0x10 after the four link/colour words), `this` unused. The value type is reconstructed from the copy:
// two words and a CIntelligentPointer (base @ 0x00A01B10, SetPItem(GetPItem) @ 0x00A01B90 / 0x00A01B50).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (landed-trusted)
inline void* operator new(unsigned int, void* p) { return p; }   // placement form: the null test on its result is what retail emits
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct SValue_00783dd0 { long a, b; CIntelligentPointer m_target; };   // two words then the family-44 intelligent-pointer copy (inlined)
struct SNode_00783dd0 {                     // 32 bytes: colour / parent / left / right, then the value
    unsigned char m_color; unsigned char _pad[3];
    SNode_00783dd0* m_pParent;
    SNode_00783dd0* m_pLeft;
    SNode_00783dd0* m_pRight;
    SValue_00783dd0 m_value;                 // +0x10
};

struct CRetailTree_00783dd0 {
    SNode_00783dd0* BuyNode(const SValue_00783dd0& value);
};

SNode_00783dd0* CRetailTree_00783dd0::BuyNode(const SValue_00783dd0& value)
{
    SNode_00783dd0* node = (SNode_00783dd0*)malloc(sizeof(SNode_00783dd0));
    new (&node->m_value) SValue_00783dd0(value);
    return node;
}