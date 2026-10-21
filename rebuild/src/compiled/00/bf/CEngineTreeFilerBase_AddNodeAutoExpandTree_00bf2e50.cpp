// CEngineTreeFilerBase::AddNodeAutoExpandTree(CNodeHeader*, CNodeHeader*) -- insert a leaf below a radix-tree branch: find the closest existing leaf, locate the first differing key bit, and either recurse into the matching child (relinking a replacement child into the sibling list), add the node directly, or rebuild the subtree from its leaf list when the branch's key window has to grow
// Leaf: AddNodeAutoExpandTree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF2E50, length 480 bytes, `ret 8`.
// Callees: CEngineTreeFilerBase::FindNode @ 0x00BC9B30 (COMDAT-folded body), AddNode @ 0x00BF2A10, ConvertTreeToList @ 0x00BF2AD0,
//   ConvertListToTree @ 0x00BF2B50, itself (recursion).
// Layout: PDB CNodeHeader (28 bytes): IsLeaf +0, KeyOffset +1, KeyBitStart +2, KeyBitCount +3, Entry +4, PrevSibling +8,
//   NextSibling +0xc, ChildListHead +0x10, ChildListTail +0x14, Parent +0x18; key words (leaf) or child pointers (branch)
//   follow at +0x1c. PDB CEngineTreeFilerBase: BaseNode +0, KeySize +4 (same model as the landed AddNode 0x00BF2A10).
// Shape: the window bounds use MIN/MAX macros (the clamped count re-evaluates its MAX); the bit scan reads leafKey[i] and
//   key[i] inside the loop (hoisted by the compiler), which frees the right registers.
#define TREE_MIN(a, b) ((a) < (b) ? (a) : (b))
#define TREE_MAX(a, b) ((a) > (b) ? (a) : (b))

struct CNodeHeader {
    unsigned char IsLeaf, KeyOffset, KeyBitStart, KeyBitCount;
    void* Entry;
    CNodeHeader* PrevSibling;
    CNodeHeader* NextSibling;
    CNodeHeader* ChildListHead;
    CNodeHeader* ChildListTail;
    CNodeHeader* Parent;
    unsigned long* Key() { return reinterpret_cast<unsigned long*>(this + 1); }
    CNodeHeader** Children() { return reinterpret_cast<CNodeHeader**>(this + 1); }
    unsigned long GetIndex(const unsigned long* key) { return (key[KeyOffset] >> KeyBitStart) & ((1UL << KeyBitCount) - 1); }
};

struct CEngineTreeFilerBase {
    CNodeHeader* BaseNode;
    long KeySize;
    void AddNode(CNodeHeader* branch, CNodeHeader* node);
    CNodeHeader* AddNodeAutoExpandTree(CNodeHeader* branch, CNodeHeader* node);
    CNodeHeader* ConvertListToTree(CNodeHeader* list);
    CNodeHeader* ConvertTreeToList(CNodeHeader* tree);
    CNodeHeader* FindNode(CNodeHeader* tree, unsigned long* key);
};

CNodeHeader* CEngineTreeFilerBase::AddNodeAutoExpandTree(CNodeHeader* branch, CNodeHeader* node)
{
    unsigned long* key = node->Key();
    CNodeHeader* leaf;
    if (branch->IsLeaf) {
        leaf = branch;
    } else {
        CNodeHeader* child = branch->Children()[branch->GetIndex(key)];
        leaf = child ? FindNode(child, key) : 0;
    }
    if (!leaf) {
        AddNode(branch, node);
        return 0;
    }
    unsigned long* leafKey = leaf->Key();
    long diffBit = -1;
    long diffWord = -1;
    for (long i = 0; i < KeySize; ++i) {
        if (leafKey[i] != key[i]) {
            diffWord = i;
            for (unsigned long bit = 0; bit < 32; ++bit) {
                if (((1UL << bit) & leafKey[i]) != ((1UL << bit) & key[i])) {
                    diffBit = bit;
                    break;
                }
            }
            break;
        }
    }
    long newStart;
    long newCount;
    if (diffWord == branch->KeyOffset && branch->KeyBitCount) {
        newStart = TREE_MIN(diffBit, (long)branch->KeyBitStart);
        newCount = TREE_MIN(TREE_MAX(diffBit, (long)branch->KeyBitStart + branch->KeyBitCount - 1) - newStart + 1, 10);
    } else {
        newStart = diffBit;
        newCount = 1;
    }
    if (diffWord == branch->KeyOffset && newStart == branch->KeyBitStart && newCount == branch->KeyBitCount) {
        unsigned long index = branch->GetIndex(key);
        CNodeHeader* child = branch->Children()[index];
        CNodeHeader* prev = child->PrevSibling;
        CNodeHeader* next = child->NextSibling;
        CNodeHeader* replacement = AddNodeAutoExpandTree(child, node);
        if (replacement) {
            branch->Children()[index] = replacement;
            replacement->NextSibling = next;
            replacement->PrevSibling = prev;
            replacement->Parent = branch;
            if (next)
                next->PrevSibling = replacement;
            if (prev)
                prev->NextSibling = replacement;
            if (branch->ChildListHead == child)
                branch->ChildListHead = replacement;
            if (branch->ChildListTail == child)
                branch->ChildListTail = replacement;
        }
        return 0;
    }
    node->NextSibling = ConvertTreeToList(branch);
    return ConvertListToTree(node);
}