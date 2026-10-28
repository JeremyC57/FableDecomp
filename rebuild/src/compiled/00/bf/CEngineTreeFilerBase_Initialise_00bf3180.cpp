// CEngineTreeFilerBase::Initialise(long keySize) -- drop any existing tree, remember the key size, start again from an empty branch root (one child slot)
// Leaf: ?Initialise@CEngineTreeFilerBase@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF3180, 76 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?Initialise@CEngineTreeFilerBase@@QAEXJ@Z (catalog row already carries it); callers are the
//   CEngineTreeFiler<...> users 0x00BCA0F3 / 0x00BCA4F3 / 0x00BCA7BC / 0x00BCC63C.
// Callees: CEngineTreeFilerBase::DeleteTree(CNodeHeader*) 0x00BF29E0 (Ego_r.pdb private, thiscall), operator new[] 0x00BFEB22.
// Layout (Ego_r.pdb): BaseNode +0, KeySize +4; CNodeHeader 0x1C (IsLeaf, KeyOffset, KeyBitStart, KeyBitCount, Entry,
//   PrevSibling, NextSibling, ChildListHead, ChildListTail, Parent), the key words or 1 << KeyBitCount child slots after it.
// Shape: the private AllocNode(isLeaf, keyOffset, keyBitStart, keyBitCount, key) inlined for a branch with a zero-bit
//   window: new[] of 0x1C + 4 bytes, the header stores in the order of the landed AddNewKey 0x00BF3030 (which inlines
//   the leaf arm), then the single child slot cleared.
void* __cdecl operator new[](unsigned int size);             // 0x00BFEB22

class CEngineTreeFilerBase {
public:
    void Initialise(long keySize);
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
    };
    void DeleteTree(CNodeHeader* node);              // 0x00BF29E0
    CNodeHeader* AllocNode(bool isLeaf, long keyOffset, long keyBitStart, long keyBitCount, unsigned long* key)
    {
        long size = isLeaf ? KeySize * 4 : (1 << keyBitCount) * 4;
        CNodeHeader* node = reinterpret_cast<CNodeHeader*>(new unsigned char[sizeof(CNodeHeader) + size]);
        node->IsLeaf = isLeaf;
        node->KeyBitCount = (unsigned char)keyBitCount;
        node->KeyBitStart = (unsigned char)keyBitStart;
        node->KeyOffset = (unsigned char)keyOffset;
        node->ChildListHead = 0;
        node->ChildListTail = 0;
        node->PrevSibling = 0;
        node->NextSibling = 0;
        node->Parent = 0;
        node->Entry = 0;
        if (isLeaf) {
            for (long i = 0; i < KeySize; ++i)
                node->Key()[i] = key[i];
        } else {
            for (long i = 0; i < (1 << keyBitCount); ++i)
                node->Children()[i] = 0;
        }
        return node;
    }
    CNodeHeader* BaseNode;                           // +0x00
    long KeySize;                                    // +0x04
};

void CEngineTreeFilerBase::Initialise(long keySize)
{
    if (BaseNode) {
        DeleteTree(BaseNode);
        BaseNode = 0;
    }
    KeySize = keySize;
    BaseNode = AllocNode(false, 0, 0, 0, 0);
}