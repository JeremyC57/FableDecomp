// Behaviour test for 0x00BF2980 CEngineTreeFilerBase::GetTreeMemSize (and its same-TU GetNodeMemSize): the row's defines, includes and
// bodies verbatim over the shared header, run on hand-built trees (header + trailing key words / child table). Cases: a leaf root
// (0x1C + KeySize*4, KeySize read from `this`: 1 and 3); a 4-slot branch whose children sit in the FIRST and LAST slots with nulls
// between (loop start and end, null skip); a nested branch (recursion on the child, not the parent); a 1-slot branch (KeyBitCount 0:
// the loop runs exactly once) and an all-null branch (only the table); GetNodeMemSize on a leaf and a branch (header included there);
// (audit) a leaf whose IsLeaf byte is 2 (non-zero test, not == 1) in both functions.
#include <stdio.h>
#include <string.h>
#define private public
#define FABLE_RETAIL_CENGINETREEFILER_INLINES
#define FABLE_RETAIL_CENGINETREEFILER_NODEHEADER_STRUCT
#define FABLE_RETAIL_CENGINETREEFILER_MEMSIZE
#include "engine/CEngineTreeFiler.h"
#undef private

long CEngineTreeFilerBase::GetNodeMemSize(CNodeHeader* node)
{
    if (node->IsLeaf)
        return sizeof(CNodeHeader) + KeySize * sizeof(unsigned long);
    return sizeof(CNodeHeader) + (sizeof(CNodeHeader*) << node->KeyBitCount);
}

long CEngineTreeFilerBase::GetTreeMemSize(CNodeHeader* node)
{
    if (node->IsLeaf)
        return GetNodeMemSize(node);
    CNodeHeader** table = (CNodeHeader**)(node + 1);
    long memcount = sizeof(CNodeHeader*) << node->KeyBitCount;
    for (long sb = 0; sb < (1 << node->KeyBitCount); sb++) {
        if (table[sb])
            memcount += GetTreeMemSize(table[sb]);
    }
    return memcount;
}

// The class's data and nested node type are default-private: the test builds nodes in a mirror of the 28-byte PDB header and reaches
// the members through deduced template parameters and the object image (BaseNode +0, KeySize +4).
struct Node { unsigned char IsLeaf, KeyOffset, KeyBitStart, KeyBitCount; void* Entry; Node* p[5]; };
struct Block { Node h; Node* slots[8]; };                    // header + up to 8 trailing words
static Block g_blocks[16];
static Node* leaf(int i) { Block& b = g_blocks[i]; memset(&b, 0, sizeof b); b.h.IsLeaf = 1; b.h.KeyBitCount = 5; return &b.h; }
static Node* branch(int i, unsigned char bits) { Block& b = g_blocks[i]; memset(&b, 0, sizeof b); b.h.IsLeaf = 0; b.h.KeyBitCount = bits; return &b.h; }
static void set(Node* n, int slot, Node* child) { ((Node**)(n + 1))[slot] = child; }
template <class C, class R, class A> R call(C& c, R (C::*f)(A), Node* n) { return (c.*f)((A)(void*)n); }
static long Tree(CEngineTreeFilerBase& t, Node* n) { return call(t, &CEngineTreeFilerBase::GetTreeMemSize, n); }
static long NodeSize(CEngineTreeFilerBase& t, Node* n) { return call(t, &CEngineTreeFilerBase::GetNodeMemSize, n); }
static void SetKeySize(CEngineTreeFilerBase& t, long k) { memset(&t, 0, sizeof t); memcpy((char*)&t + 4, &k, 4); }
static int fail(int n) { printf("00bf2980_TEST FAIL %d\n", n); return 1; }

int main()
{
    if (sizeof(Node) != 0x1C) return fail(0);
    CEngineTreeFilerBase t; SetKeySize(t, 3);
    Node* a = leaf(0);
    if (Tree(t, a) != 0x1C + 12) return fail(1);
    SetKeySize(t, 1);
    if (Tree(t, a) != 0x1C + 4) return fail(2);
    Node* b = branch(1, 2);                                   // 4 slots: first and last used
    set(b, 0, leaf(2)); set(b, 3, leaf(3)); set(b, 4, leaf(10));   // slot 4 lies past the table: never counted
    if (Tree(t, b) != 16 + 2 * (0x1C + 4)) return fail(3);
    Node* c = branch(4, 1);                                   // nested: 2 slots, slot 1 = leaf, slot 0 = b
    set(c, 0, b); set(c, 1, leaf(5));
    if (Tree(t, c) != 8 + (16 + 2 * 0x20) + 0x20) return fail(4);
    Node* d = branch(6, 0);                                   // 1 slot
    set(d, 0, leaf(7)); set(d, 1, leaf(11));                      // slot 1 lies past the 1-slot table
    if (Tree(t, d) != 4 + 0x20) return fail(5);
    Node* e = branch(8, 3);                                   // 8 slots, all null
    if (Tree(t, e) != 32) return fail(6);
    set(e, 7, leaf(9));                                       // only the very last slot
    if (Tree(t, e) != 32 + 0x20) return fail(7);
    if (NodeSize(t, a) != 0x20 || NodeSize(t, e) != 0x1C + 32) return fail(8);
    // (audit fix) IsLeaf is an unsigned char tested for non-zero (retail `cmp byte ptr [node], 0`): a leaf flag of 2 is a leaf in both
    // functions (kills `IsLeaf == 1`); KeyBitCount 0 and KeySize 3 make the branch reading differ (4 / 0x20 instead of 0x28).
    SetKeySize(t, 3);
    Node* f = leaf(12); f->IsLeaf = 2; f->KeyBitCount = 0;
    if (Tree(t, f) != 0x1C + 12) return fail(9);
    if (NodeSize(t, f) != 0x1C + 12) return fail(10);
    printf("00bf2980_TEST PASS\n");
    return 0;
}