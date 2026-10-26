// CComponentStateMap::operator[](NUISystem::CComponent* const&) -- STLport map<CComponent*, CUIState>::operator[]: lower_bound, insert a default CUIState when the key is missing, return the mapped value
// Leaf: ??ACComponentStateMap
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042D5B1, 83 bytes. Pragmas: frame pointer kept in a
// size-optimised TU (`optimize("y",off)` + `("s",on)`, the tree-TU settings).
// Callees: 0x0042AD6C _Rb_tree::_M_lower_bound (node pointer in eax), 0x0052E5D0 CUIState::CUIState(),
// 0x0042AA96 pair<CComponent* const, CUIState>::pair(const key&, const CUIState&), 0x0042D4D3
// _Rb_tree::insert_unique(iterator hint, const value_type&) (iterator by hidden pointer).
//
// The manifest label `CPersistTraits::Transfer` is propagated and wrong. This is STLport's
//     T& operator[](const key_type& k) {
//         iterator i = lower_bound(k);
//         if (i == end() || key_comp()(k, (*i).first)) i = insert(i, value_type(k, T()));
//         return (*i).second; }
// STLport tree layout: the map holds the header node pointer at +0 (`cmp esi,[edi]` is end()), nodes
// are {color, parent, left, right} with the value at +0x10: the key (+0x10, a pointer: unsigned
// `jae`) and the 0x24-byte mapped CUIState (+0x14, returned with `lea eax,[esi+14h]`). The mapped
// default (frame -0x24) and the pair (frame -0x4C) have trivial destructors; the iterator is a
// 4-byte class with a user copy constructor (built in a reserved argument slot: `push ecx ; mov
// eax,esp ; mov [eax],esi`), and the insert's hidden result reuses the dead key-argument slot
// (`lea eax,[ebp+8]`). The default CUIState is a NAMED local: retail re-addresses it with
// `lea eax,[ebp-24h]` after its constructor, where a bare `CUIState()` temporary pushes the
// constructor's returned eax (80 v 83 bytes).
namespace NUISystem { class CComponent; }

struct CUIState {
    unsigned char m_body[0x24];
    CUIState();                                          // 0x0052E5D0
    CUIState(const CUIState& other);                     // 0x00429E29
};

struct SComponentStatePair {
    NUISystem::CComponent* first;                        // +0x00
    CUIState               second;                       // +0x04
    SComponentStatePair(NUISystem::CComponent* const& key, const CUIState& value);   // 0x0042AA96
};

struct SComponentStateNode {
    long                 m_color;                        // +0x00
    SComponentStateNode* m_pParent;                      // +0x04
    SComponentStateNode* m_pLeft;                        // +0x08
    SComponentStateNode* m_pRight;                       // +0x0C
    SComponentStatePair  m_value;                        // +0x10
};

struct SComponentStateIterator {
    SComponentStateNode* m_pNode;
    explicit SComponentStateIterator(SComponentStateNode* node) : m_pNode(node) {}
    SComponentStateIterator(const SComponentStateIterator& other) : m_pNode(other.m_pNode) {}
    SComponentStatePair& operator*() const { return m_pNode->m_value; }
    bool operator==(const SComponentStateIterator& other) const { return m_pNode == other.m_pNode; }
};

struct CComponentStateMap {
    SComponentStateNode* m_pHeader;                      // +0x00
    unsigned long        m_count;                        // +0x04

    SComponentStateNode* LowerBoundNode(NUISystem::CComponent* const& key);          // 0x0042AD6C
    SComponentStateIterator InsertUnique(SComponentStateIterator hint, const SComponentStatePair& value);   // 0x0042D4D3
    SComponentStateIterator lower_bound(NUISystem::CComponent* const& key) { return SComponentStateIterator(LowerBoundNode(key)); }
    SComponentStateIterator end() { return SComponentStateIterator(m_pHeader); }
    CUIState& operator[](NUISystem::CComponent* const& key);
};

CUIState& CComponentStateMap::operator[](NUISystem::CComponent* const& key)
{
    SComponentStateIterator it = lower_bound(key);
    if (it == end() || key < (*it).first) {
        CUIState defaultValue;
        it = InsertUnique(it, SComponentStatePair(key, defaultValue));
    }
    return (*it).second;
}