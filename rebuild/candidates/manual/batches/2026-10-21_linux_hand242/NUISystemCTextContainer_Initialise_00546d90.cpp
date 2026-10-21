// NUISystem::CTextContainer::Initialise() -- stack the children vertically: after the CChangingStateComponent reset, set every later child's state-record Y to the summed heights of the children above it, move it to its state-0 position, and record the total height
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546D90, 331 bytes (336 with padding). Vtable 0x01248064 (RTTI NUISystem::CTextContainer) slot 43;
// the manifest label CComponentContainer::Initialise is propagated from the byte-identical body 0x005461D0.
// Callees: CChangingStateComponent::Initialise 0x0052C730; the STLport map<unsigned long, SComponentState>::operator[]
//          0x0042D5B1 (out of line); child vtable slots 15 (+0x3c, SetPosition: writes +0x34/+0x3c/+0x44),
//          23 (+0x5c, GetSize, C2DVector returned through the hidden pointer) and 82 (+0x148, GetStateMap:
//          `lea eax,[ecx+24h]`).
//
// Shape: the running height lives in a frame float that is re-read for every addition (`fld [esp+10h] ;
// fadd [eax+4]`); the state-record walk reloads the child pointer and calls GetStateMap for begin() and for
// every end() test, and the STLport iterator increment (right child then leftmost, else climb) is inline.
// The loop is the guarded do-while (`if (i < size) { key = 0; do ... while (++i < size); }`): the operator[] key is a
// named local of the guarded block, so its zero store lands in the preheader after the guard as in retail. The trailing height is
// `y + Children[size-1]->GetSize().Y` under a non-empty test (begin != end).
// Layout: local model -- NUISystem::CComponent is not in the PDB type dump; offsets follow the landed
// CComponent / CChangingStateComponent rows (0x0052C730, 0x0052E6B0): StateMap +0x24, Position +0x34,
// child vector +0xb0 (8-byte CCountedPointer<CComponent> elements), CComponent 0x134 bytes. +0xd0 is Size.Y (PDB
// CComponent::Size at retail +0xcc), here the stacked content height.
namespace NUISystem {

struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
};
struct SComponentState {                    // value of the state map (key: the state index)
    unsigned char _pad00[8];
    C2DVector     Position;                 // +0x08
    C2DVector     Size;                     // +0x10
    float         Alpha;                    // +0x18
};
struct SStateMapNode {                      // STLport _Rb_tree_node<pair<const unsigned long, SComponentState>>
    unsigned char   m_color;                // +0x00
    SStateMapNode*  m_pParent;              // +0x04
    SStateMapNode*  m_pLeft;                // +0x08
    SStateMapNode*  m_pRight;               // +0x0c
    unsigned long   m_key;                  // +0x10
    SComponentState m_value;                // +0x14
};
struct CStateMapIterator {
    SStateMapNode* m_pNode;
    CStateMapIterator(SStateMapNode* node) : m_pNode(node) {}
    SComponentState& operator*() const { return m_pNode->m_value; }
    bool operator!=(const CStateMapIterator& other) const { return m_pNode != other.m_pNode; }
    static SStateMapNode* Increment(SStateMapNode* node)   // STLport _Rb_global::_M_increment
    {
        if (node->m_pRight != 0) {
            node = node->m_pRight;
            while (node->m_pLeft != 0)
                node = node->m_pLeft;
        } else {
            SStateMapNode* y = node->m_pParent;
            while (node == y->m_pRight) {
                node = y;
                y = y->m_pParent;
            }
            if (node->m_pRight != y)
                node = y;
        }
        return node;
    }
    void operator++() { m_pNode = Increment(m_pNode); }
};
struct CStateMap {                          // map<unsigned long, SComponentState> (STLport, 12 bytes)
    SStateMapNode* m_pHeader;
    unsigned long  m_nodeCount;
    unsigned char  m_compare;
    CStateMapIterator begin() { return CStateMapIterator(m_pHeader->m_pLeft); }
    CStateMapIterator end() { return CStateMapIterator(m_pHeader); }
    SComponentState& operator[](const unsigned long& key);   // 0x0042D5B1
};
class CComponent;
struct CCountedComponentPointer {           // CCountedPointer<NUISystem::CComponent>
    CComponent* Object;
    void*       Info;
    CComponent* operator->() const { return Object; }
};
struct CComponentPointerVector {            // STLport vector<CCountedPointer<CComponent>>
    CCountedComponentPointer* _M_start;
    CCountedComponentPointer* _M_finish;
    CCountedComponentPointer* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    CCountedComponentPointer& operator[](unsigned int i) { return _M_start[i]; }
};
class CComponent {
public:
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void VSlot2();
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void SetPosition(const C2DVector& position);           // slot 15 (+0x3c)
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void VSlot18();
    virtual void VSlot19();
    virtual void VSlot20();
    virtual void VSlot21();
    virtual void VSlot22();
    virtual C2DVector GetSize();                                    // slot 23 (+0x5c)
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void VSlot32();
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void VSlot36();
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void VSlot39();
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void Initialise();                                      // slot 43 (+0xac)
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void VSlot47();
    virtual void VSlot48();
    virtual void VSlot49();
    virtual void VSlot50();
    virtual void VSlot51();
    virtual void VSlot52();
    virtual void VSlot53();
    virtual void VSlot54();
    virtual void VSlot55();
    virtual void VSlot56();
    virtual void VSlot57();
    virtual void VSlot58();
    virtual void VSlot59();
    virtual void VSlot60();
    virtual void VSlot61();
    virtual void VSlot62();
    virtual void VSlot63();
    virtual void VSlot64();
    virtual void VSlot65();
    virtual void VSlot66();
    virtual void VSlot67();
    virtual void VSlot68();
    virtual void VSlot69();
    virtual void VSlot70();
    virtual void VSlot71();
    virtual void VSlot72();
    virtual void VSlot73();
    virtual void VSlot74();
    virtual void VSlot75();
    virtual void VSlot76();
    virtual void VSlot77();
    virtual void VSlot78();
    virtual void VSlot79();
    virtual void VSlot80();
    virtual void VSlot81();
    virtual CStateMap& GetStateMap();                               // slot 82 (+0x148), returns &StateMap (+0x24)
    unsigned char _pad04[0x20];             // +0x04
    CStateMap     StateMap;                 // +0x24
    unsigned char _pad30[0x80];             // +0x30
    CComponentPointerVector Children;       // +0xb0
    unsigned char _padbc[0x10];             // +0xbc ChildrenToDelete, PParent
    float         SizeX;                    // +0xcc Size.X
    float         SizeY;                    // +0xd0 Size.Y (PDB CComponent::Size, retail +0xcc)
    unsigned char _padd4[0x60];             // +0xd4 .. +0x134
};
class CChangingStateComponent : public CComponent {
public:
    unsigned char _pad134[0x28];            // +0x134 .. +0x15c (state machine)
    void Initialise();                      // 0x0052C730
};
class CTextContainer : public CChangingStateComponent {
public:
    void Initialise();
};

void CTextContainer::Initialise()
{
    CChangingStateComponent::Initialise();
    float y = 0.0f;
    unsigned int i = 1;
    if (i < Children.size()) {
        unsigned long state = 0;
        do {
            y += Children[i - 1]->GetSize().Y;
            for (CStateMapIterator it = Children[i]->GetStateMap().begin(); it != Children[i]->GetStateMap().end(); ++it)
                (*it).Position.Y = y;
            Children[i]->SetPosition(Children[i]->GetStateMap()[state].Position);
        } while (++i < Children.size());
    }
    if (!Children.empty())
        SizeY = y + Children[Children.size() - 1]->GetSize().Y;
}

}
