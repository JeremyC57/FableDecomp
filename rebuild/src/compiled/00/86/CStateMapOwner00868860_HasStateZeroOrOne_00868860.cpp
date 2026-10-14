// HasStateZeroOrOne_00868860() -- true when the object's map at +0x30 (an STLport red-black tree keyed by
// a long: header +0, node {colour, parent, left, right, key +0x10}) holds key 0 or key 1. The find is the
// out-of-line tree find @ 0x006DAB90 (hidden-pointer iterator result, `const long&` key: both keys are
// stack temporaries); end() is the header pointer, read once after the first find and kept in edi.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00868860, length 87 bytes.
// Callees: the tree find @ 0x006DAB90 (x2; manifest label BinaryTree_LowerBound). Called from eleven sites
// (0x667EE7 .. 0x8F5E4A) on an object that is not identified here; the manifest label
// (CChangingStateComponent::GetCurrentState) is propagated garbage, so the row is named by what it does.
struct SStateNode;
struct SStateIterator {
    SStateNode* Node;
    SStateIterator(SStateNode* node) : Node(node) { }
};
struct CRetailStateMap {                        // STLport _Rb_tree<long, ...>: {header, node count, compare}
    SStateNode*   Header;                       // +0
    long          NodeCount;                    // +4
    unsigned char Compare[4];                   // +8
    SStateIterator Find(const long& key);       // 0x006DAB90
};
struct CStateMapOwner_00868860 {
    unsigned char   _pad_0x00[0x30];
    CRetailStateMap m_states;                   // +0x30
    bool HasStateZeroOrOne();
};

bool CStateMapOwner_00868860::HasStateZeroOrOne()
{
    SStateNode* end;
    {
        SStateIterator first = m_states.Find(0);
        end = m_states.Header;
        if (first.Node != end)
            return true;
    }
    {
        SStateIterator second = m_states.Find(1);
        return second.Node != end ? true : false;
    }
}