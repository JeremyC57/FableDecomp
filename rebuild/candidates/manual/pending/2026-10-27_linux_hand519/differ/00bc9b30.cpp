// CEngineTreeFilerBase::FindNode(CNodeHeader* node, unsigned long* key) -- walk the radix tree from node by the key bits each branch selects, down to a leaf; 0 at an empty child slot
// Grade: DIFFER(66v66) -- register choice only: retail keeps key in esi and the child index in edx (`mov edx,[esi+ecx*4] ; shr edx,cl ; and edx,edi ; mov eax,[eax+edx*4+1Ch]`); ours swaps them (key in edx, index in esi). while/do-while/for(;;), named index/mask/word locals and inline GetIndex forms are inert
// Leaf: ?FindNode@CEngineTreeFilerBase@@AAEPAUCNodeHeader@1@PAU21@PAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC9B30, 66 bytes, thiscall (`this` unused), `ret 8`.
// Identity (proven): Ego_r.pdb ?FindNode@CEngineTreeFilerBase@@AAEPAUCNodeHeader@1@PAU21@PAK@Z; the landed
//   AddNodeAutoExpandTree 0x00BF2E50 calls it (`FindNode(child, key)`), as do nine CEngineTreeFiler<...> users.
// Layout: CNodeHeader as Initialise 0x00BF3180 (IsLeaf +0, KeyOffset +1, KeyBitStart +2, KeyBitCount +3, child slots at +0x1C).
// Shape: `while (!node->IsLeaf) { node = child[index]; if (!node) return 0; } return node;` with the index
//   (key[KeyOffset] >> KeyBitStart) & ((1 << KeyBitCount) - 1).
class CEngineTreeFilerBase {
public:
private:
    struct CNodeHeader {
        unsigned char IsLeaf;
        unsigned char KeyOffset;
        unsigned char KeyBitStart;
        unsigned char KeyBitCount;
        void* Entry;
        CNodeHeader* PrevSibling;
        CNodeHeader* NextSibling;
        CNodeHeader* ChildListHead;
        CNodeHeader* ChildListTail;
        CNodeHeader* Parent;
        unsigned long* Key() { return reinterpret_cast<unsigned long*>(this + 1); }
        CNodeHeader** Children() { return reinterpret_cast<CNodeHeader**>(this + 1); }
        unsigned long GetIndex(const unsigned long* key) const { return (key[KeyOffset] >> KeyBitStart) & ((1 << KeyBitCount) - 1); }
    };
    CNodeHeader* FindNode(CNodeHeader* node, unsigned long* key);
    CNodeHeader* BaseNode;                           // +0x00
    long KeySize;                                    // +0x04
};


CEngineTreeFilerBase::CNodeHeader* CEngineTreeFilerBase::FindNode(CNodeHeader* node, unsigned long* key)
{
    while (!node->IsLeaf) {
        node = node->Children()[node->GetIndex(key)];
        if (node == 0)
            return 0;
    }
    return node;
}
