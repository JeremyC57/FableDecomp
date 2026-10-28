// Behaviour test for CEngineTreeFilerBase::Initialise (0x00BF3180): an existing tree is deleted, the key size stored, and a fresh empty branch root allocated.
#include <cstdio>
struct Node { unsigned char IsLeaf, KeyOffset, KeyBitStart, KeyBitCount; void* Entry; Node* Prev; Node* Next; Node* Head; Node* Tail; Node* Parent; void* children[1]; };
static int g_deleted;
struct Filer { Node* BaseNode; int KeySize;
    void DeleteTree(Node* n) { ++g_deleted; delete n; }
    Node* AllocBranchRoot() { Node* n = new Node; n->IsLeaf = 0; n->KeyBitCount = 0; n->KeyBitStart = 0; n->KeyOffset = 0;
        n->Head = n->Tail = n->Prev = n->Next = n->Parent = 0; n->Entry = 0; n->children[0] = 0; return n; }
    void Initialise(int keySize) { if (BaseNode) { DeleteTree(BaseNode); BaseNode = 0; } KeySize = keySize; BaseNode = AllocBranchRoot(); } };
int main()
{
    Filer f; f.BaseNode = 0; g_deleted = 0;
    f.Initialise(2);
    if (g_deleted != 0 || f.KeySize != 2 || f.BaseNode == 0 || f.BaseNode->IsLeaf != 0 || f.BaseNode->KeyBitCount != 0 || f.BaseNode->children[0] != 0) {
        std::printf("00bf3180_TEST FAIL\n"); return 1; }
    f.Initialise(4);
    if (g_deleted != 1 || f.KeySize != 4 || f.BaseNode == 0) { std::printf("00bf3180_TEST FAIL\n"); return 1; }
    f.DeleteTree(f.BaseNode);
    std::printf("00bf3180_TEST PASS\n");
    return 0;
}