// CLandscapeBackgroundTreeNodePair_00bf7b90::LoadHeader(CDataInputStream*) -- loads the headers of the two tree nodes held at +0x4 and +0xEC from the same stream
// Leaf: LoadHeader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF7B90, 34 bytes (+14 int3 padding). Callee:
// CLandscapeBackgroundTreeNode::LoadHeader(CDataInputStream*) @ 0x00BF7400 (landed), called on the two embedded
// nodes at +0x4 and +0xEC (0xE8 bytes apart, the node size). Callers: 0x00BF8398 and 0x00BF8731 (landscape
// background loaders).
// Label: "CSpellContainerList::AddChild" is a propagated name over byte-identical two-member forwarders. Shape: the
// stream argument is loaded once into edi and pushed for both calls; `this` lives in esi.
class CDataInputStream;

struct CLandscapeBackgroundTreeNode {
    unsigned char _body[0xE8];
    void LoadHeader(CDataInputStream* stream);   // 0x00BF7400
};

struct CLandscapeBackgroundTreeNodePair_00bf7b90 {
    unsigned char                _pad00[0x4];
    CLandscapeBackgroundTreeNode m_first;        // +0x4
    CLandscapeBackgroundTreeNode m_second;       // +0xEC
    void LoadHeader(CDataInputStream* stream);
};

void CLandscapeBackgroundTreeNodePair_00bf7b90::LoadHeader(CDataInputStream* stream)
{
    m_first.LoadHeader(stream);
    m_second.LoadHeader(stream);
}
