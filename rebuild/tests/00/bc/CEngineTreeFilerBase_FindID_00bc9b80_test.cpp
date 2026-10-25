// Behaviour test for 0x00BC9B80 CEngineTreeFilerBase::FindID: descend the bit-sliced key tree (child slot =
// (key[KeyOffset] >> KeyBitStart) & ((1 << KeyBitCount) - 1)) to a leaf and return the leaf's address as its ID when
// all KeySize stored key words match, else 0 (a null child or a key mismatch).
#include <cstdio>
#include <cstring>

struct CNodeHeader {
    unsigned char IsLeaf, KeyOffset, KeyBitStart, KeyBitCount;
    void* Entry;
    CNodeHeader *PrevSibling, *NextSibling, *ChildListHead, *ChildListTail, *Parent;
};

struct CTree {
    CNodeHeader* BaseNode;
    long KeySize;
    CNodeHeader* FindNode(CNodeHeader* node, unsigned long* key)
    {
        if (node->IsLeaf)
            return node;
        CNodeHeader* child = ((CNodeHeader**)(node + 1))[(key[node->KeyOffset] >> node->KeyBitStart) & ((1 << node->KeyBitCount) - 1)];
        if (child)
            return FindNode(child, key);
        return 0;
    }
    CNodeHeader* FindID(unsigned long* key)
    {
        CNodeHeader* node = FindNode(BaseNode, key);
        if (!node)
            return 0;
        unsigned long* nodeKey = (unsigned long*)(node + 1);
        for (long i = 0; i < KeySize; ++i)
            if (key[i] != nodeKey[i])
                return 0;
        return node;
    }
};

static CNodeHeader* MakeBranch(void* mem, unsigned char offset, unsigned char start, unsigned char count)
{
    std::memset(mem, 0, sizeof(CNodeHeader) + 4 * sizeof(CNodeHeader*));
    CNodeHeader* n = (CNodeHeader*)mem;
    n->IsLeaf = 0; n->KeyOffset = offset; n->KeyBitStart = start; n->KeyBitCount = count;
    return n;
}
static CNodeHeader* MakeLeaf(void* mem, unsigned long k0, unsigned long k1)
{
    std::memset(mem, 0, sizeof(CNodeHeader) + 2 * sizeof(unsigned long));
    CNodeHeader* n = (CNodeHeader*)mem;
    n->IsLeaf = 1;
    ((unsigned long*)(n + 1))[0] = k0;
    ((unsigned long*)(n + 1))[1] = k1;
    return n;
}

int main()
{
    static double rootMem[32], innerMem[32], leafA[32], leafB[32];
    CNodeHeader* root = MakeBranch(rootMem, 0, 4, 2);       // slot = (key[0] >> 4) & 3
    CNodeHeader* inner = MakeBranch(innerMem, 1, 0, 1);     // slot = key[1] & 1
    CNodeHeader* a = MakeLeaf(leafA, 0x10, 0x7);
    CNodeHeader* b = MakeLeaf(leafB, 0x30, 0x2);
    ((CNodeHeader**)(root + 1))[1] = inner;
    ((CNodeHeader**)(root + 1))[3] = b;
    ((CNodeHeader**)(inner + 1))[1] = a;
    CTree tree; tree.BaseNode = root; tree.KeySize = 2;
    int ok = 1;
    unsigned long k1[2] = { 0x10, 0x7 };
    unsigned long k2[2] = { 0x30, 0x2 };
    unsigned long k3[2] = { 0x10, 0x5 };                     // reaches leaf a, second word differs
    unsigned long k4[2] = { 0x10, 0x6 };                     // inner slot 0 is empty
    unsigned long k5[2] = { 0x20, 0x0 };                     // root slot 2 is empty
    if (tree.FindID(k1) != a || tree.FindID(k2) != b) ok = 0;
    if (tree.FindID(k3) != 0 || tree.FindID(k4) != 0 || tree.FindID(k5) != 0) ok = 0;
    tree.KeySize = 1;                                        // only the first word compared
    if (tree.FindID(k3) != a) ok = 0;
    tree.BaseNode = a;                                       // a leaf root
    if (tree.FindID(k1) != a) ok = 0;
    std::printf(ok ? "00bc9b80_TEST PASS\n" : "00bc9b80_TEST FAIL\n");
    return ok ? 0 : 1;
}