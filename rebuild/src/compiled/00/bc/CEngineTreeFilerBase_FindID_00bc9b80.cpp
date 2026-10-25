// CEngineTreeFilerBase::FindID(unsigned long* key) -- descend the bit-sliced key tree to the leaf for key and return that leaf's ID (its node address) when its stored key matches all KeySize words, else 0
// Leaf: ?FindID@CEngineTreeFilerBase@@QAEKPAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC9B80, 115 bytes, thiscall, `ret 4`.
// Identity: Ego_d.pdb / FableWin.pdb ?FindID@CEngineTreeFilerBase@@QAEKPAK@Z (absent from the Ego_r name set: inlined
//   there). No direct callers in retail. The out-of-line callee 0x00BC9B30 is ?FindNode@CEngineTreeFilerBase@@AAEPAUCNodeHeader@1@PAU21@PAK@Z
//   (its self-recursion turned into a loop). Catalog "CEngineTreeFilerBase_FindKeyNode" names the class correctly.
// Callees: CEngineTreeFilerBase::FindNode(CNodeHeader* node, unsigned long* key) 0x00BC9B30.
// Layout: Ego_r.pdb CEngineTreeFilerBase {BaseNode +0, KeySize +4}; CNodeHeader (28 bytes: IsLeaf, KeyOffset, KeyBitStart,
//   KeyBitCount bytes, Entry, sibling/child/parent links) is followed by the child table of a branch node or the stored
//   key words of a leaf (+0x1C).
// Shape: FindNode is recursive and visible in this TU, so VC7.1 inlines its first level here (branch test, the child slot
//   `(key[KeyOffset] >> KeyBitStart) & ((1 << KeyBitCount) - 1)`, the null test) and calls itself for the rest; the
//   key compare walks key and the leaf's words with one index register and a fixed difference. The child table and the
//   leaf words are direct `(node + 1)` casts: a helper member (Children()/Key()) is left as a call by the depth limit. The
//   leaf words go through a named `nodeKey` local: indexing the cast in the loop keeps key[i] indexed and walks the leaf.
class CEngineTreeFilerBase {
public:
    struct CNodeHeader {                           // Ego_r.pdb, 28 bytes
        unsigned char IsLeaf;                      // +0x00
        unsigned char KeyOffset;                   // +0x01
        unsigned char KeyBitStart;                 // +0x02
        unsigned char KeyBitCount;                 // +0x03
        void*         Entry;                       // +0x04
        CNodeHeader*  PrevSibling;                 // +0x08
        CNodeHeader*  NextSibling;                 // +0x0c
        CNodeHeader*  ChildListHead;               // +0x10
        CNodeHeader*  ChildListTail;               // +0x14
        CNodeHeader*  Parent;                      // +0x18
    };

    CNodeHeader* BaseNode;                         // +0x00
    long         KeySize;                          // +0x04

    unsigned long FindID(unsigned long* key);
private:
    CNodeHeader* FindNode(CNodeHeader* node, unsigned long* key);
};

#pragma inline_recursion(on)
#pragma inline_depth(1)
__forceinline CEngineTreeFilerBase::CNodeHeader* CEngineTreeFilerBase::FindNode(CNodeHeader* node, unsigned long* key)
{
    if (node->IsLeaf)
        return node;
    CNodeHeader* child = ((CNodeHeader**)(node + 1))[(key[node->KeyOffset] >> node->KeyBitStart) & ((1 << node->KeyBitCount) - 1)];
    if (child)
        return FindNode(child, key);
    return 0;
}

unsigned long CEngineTreeFilerBase::FindID(unsigned long* key)
{
    CNodeHeader* node = FindNode(BaseNode, key);
    if (!node)
        return 0;
    unsigned long* nodeKey = (unsigned long*)(node + 1);
    for (long i = 0; i < KeySize; ++i)
        if (key[i] != nodeKey[i])
            return 0;
    return (unsigned long)node;
}