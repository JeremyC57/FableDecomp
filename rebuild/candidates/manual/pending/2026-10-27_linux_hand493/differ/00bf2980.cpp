// CEngineTreeFilerBase::GetTreeMemSize(CNodeHeader* node) -- bytes used by a key tree: a leaf is its header plus KeySize key words, an inner node its child-pointer table plus every child's subtree
// Grade: DIFFER(113v96) -- VC7.1 strength-reduces the child-table walk (lea ebp,[node+0x1c] ; add ebp,4) and spills this; retail indexes [edi+esi*4+0x1c] with this kept in ebp
// Leaf: ?GetTreeMemSize@CEngineTreeFilerBase@@AAEJPAUCNodeHeader@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF2980, 96 bytes, thiscall, `ret 4`, recursive.
// Identity (proven): Ego_r.pdb private ?GetTreeMemSize@CEngineTreeFilerBase@@AAEJPAUCNodeHeader@1@@Z -- the only
//   self-recursive long-returning (node) member of the family, sitting between the landed CEngineTreeFilerBase members
//   (DeleteTree 0x00BF29E0 follows it; FindID 0x00BC9B80, AddNode 0x00BF2A10 are landed).
// Layout: Ego_r.pdb CEngineTreeFilerBase {BaseNode +0, KeySize +4} and the 28-byte nested struct CNodeHeader {IsLeaf,
//   KeyOffset, KeyBitStart, KeyBitCount, Entry, PrevSibling, NextSibling +0xc, ChildListHead +0x10, ChildListTail, Parent};
//   an inner node's child-pointer table (1 << KeyBitCount slots) follows the header at +0x1c.
// Shape: the slot count is recomputed from KeyBitCount on every pass (`mov cl,[edi+3] ; shl edx,cl`), the first one before
//   the total's `4 << KeyBitCount` seed.
class CEngineTreeFilerBase {
public:
    struct CNodeHeader {
        unsigned char IsLeaf;                   // +0x00
        unsigned char KeyOffset;                // +0x01
        unsigned char KeyBitStart;              // +0x02
        unsigned char KeyBitCount;              // +0x03
        void*         Entry;                    // +0x04
        CNodeHeader*  PrevSibling;              // +0x08
        CNodeHeader*  NextSibling;              // +0x0c
        CNodeHeader*  ChildListHead;            // +0x10
        CNodeHeader*  ChildListTail;            // +0x14
        CNodeHeader*  Parent;                   // +0x18
        CNodeHeader*  Children[1];              // +0x1c, inner nodes only: 1 << KeyBitCount slots
    };
    CNodeHeader* BaseNode;                      // +0x00
    long         KeySize;                       // +0x04
private:
    long GetTreeMemSize(CNodeHeader* node);
};

long CEngineTreeFilerBase::GetTreeMemSize(CNodeHeader* node)
{
    if (node->IsLeaf)
        return KeySize * 4 + 0x1c;
    long size = sizeof(CNodeHeader*) << node->KeyBitCount;
    for (long i = 0; i < (1 << node->KeyBitCount); i++) {
        if (node->Children[i])
            size += GetTreeMemSize(node->Children[i]);
    }
    return size;
}
