// CEngineTreeFilerBase::AddNode -- insert a node into a radix branch and its ordered child list.
// Retail 00BF2A10,188 bytes; no callees, globals, literals or vtable relocations.
// PDB CNodeHeader fields agree with native offsets; trailing words are branch children or leaf key.
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
    unsigned long GetIndex(CNodeHeader* n) { unsigned long key = n->Key()[KeyOffset]; key >>= KeyBitStart; return key & ((1UL << KeyBitCount) - 1); }
};
struct CEngineTreeFilerBase {
    CNodeHeader* BaseNode;
    long KeySize;
    void AddNode(CNodeHeader* branch, CNodeHeader* node);
};
void CEngineTreeFilerBase::AddNode(CNodeHeader* branch, CNodeHeader* node)
{
    long i = branch->GetIndex(node);
    CNodeHeader** children = branch->Children();
    while (children[i]) {
        branch = children[i];
        i = branch->GetIndex(node);
        children = branch->Children();
    }
    node->Parent = branch;
    children[i] = node;
    --i;
    for (; i >= 0; --i) {
        if (children[i]) {
            node->PrevSibling = children[i];
            node->NextSibling = children[i]->NextSibling;
            children[i]->NextSibling = node;
            if (node->NextSibling) node->NextSibling->PrevSibling = node;
            if (branch->ChildListTail->NextSibling) branch->ChildListTail = branch->ChildListTail->NextSibling;
            return;
        }
    }
    if (!branch->ChildListTail) branch->ChildListTail = node;
    node->NextSibling = branch->ChildListHead;
    branch->ChildListHead = node;
    if (node->NextSibling) node->NextSibling->PrevSibling = node;
}