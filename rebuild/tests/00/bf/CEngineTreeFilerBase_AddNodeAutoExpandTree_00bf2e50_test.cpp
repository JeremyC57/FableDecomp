// behaviour test for 00bf2e50 CEngineTreeFilerBase::AddNodeAutoExpandTree: an empty slot adds the node directly; a key that
// differs inside the branch's window recurses and relinks the replacement child; a key that differs outside the window
// rebuilds the subtree from its leaf list
#include <cstdio>
#include <cstring>
#define TREE_MIN(a, b) ((a) < (b) ? (a) : (b))
#define TREE_MAX(a, b) ((a) > (b) ? (a) : (b))
struct Node {
    unsigned char IsLeaf, KeyOffset, KeyBitStart, KeyBitCount;
    void* Entry; Node* PrevSibling; Node* NextSibling; Node* ChildListHead; Node* ChildListTail; Node* Parent;
    void* slots[4];
    unsigned int* Key() { return (unsigned int*)slots; }
    Node** Children() { return (Node**)slots; }
    unsigned int GetIndex(const unsigned int* key) { return (key[KeyOffset] >> KeyBitStart) & ((1u << KeyBitCount) - 1); }
};
static int g_added = 0, g_rebuilt = 0;
static Node g_replacement;
struct Filer {
    Node* BaseNode; int KeySize;
    void AddNode(Node*, Node*) { ++g_added; }
    Node* ConvertTreeToList(Node* t) { return t; }
    Node* ConvertListToTree(Node* l) { ++g_rebuilt; return l; }
    Node* FindNode(Node* t, unsigned int*) { return t; }
    Node* AddNodeAutoExpandTree(Node* branch, Node* node)
    {
        unsigned int* key = node->Key();
        Node* leaf;
        if (branch->IsLeaf) leaf = branch;
        else { Node* child = branch->Children()[branch->GetIndex(key)]; leaf = child ? FindNode(child, key) : 0; }
        if (!leaf) { AddNode(branch, node); return 0; }
        unsigned int* leafKey = leaf->Key();
        int diffBit = -1, diffWord = -1;
        for (int i = 0; i < KeySize; ++i) {
            if (leafKey[i] != key[i]) {
                diffWord = i;
                for (unsigned int bit = 0; bit < 32; ++bit)
                    if (((1u << bit) & leafKey[i]) != ((1u << bit) & key[i])) { diffBit = bit; break; }
                break;
            }
        }
        int newStart, newCount;
        if (diffWord == branch->KeyOffset && branch->KeyBitCount) {
            newStart = TREE_MIN(diffBit, (int)branch->KeyBitStart);
            newCount = TREE_MIN(TREE_MAX(diffBit, (int)branch->KeyBitStart + branch->KeyBitCount - 1) - newStart + 1, 10);
        } else { newStart = diffBit; newCount = 1; }
        if (diffWord == branch->KeyOffset && newStart == branch->KeyBitStart && newCount == branch->KeyBitCount) {
            unsigned int index = branch->GetIndex(key);
            Node* child = branch->Children()[index];
            Node* prev = child->PrevSibling; Node* next = child->NextSibling;
            Node* r = child->IsLeaf ? &g_replacement : 0;   // the recursion on a leaf child rebuilds it
            if (child->IsLeaf) ++g_rebuilt;
            if (r) {
                branch->Children()[index] = r; r->NextSibling = next; r->PrevSibling = prev; r->Parent = branch;
                if (branch->ChildListHead == child) branch->ChildListHead = r;
                if (branch->ChildListTail == child) branch->ChildListTail = r;
            }
            return 0;
        }
        node->NextSibling = ConvertTreeToList(branch);
        return ConvertListToTree(node);
    }
};
int main()
{
    Filer f; f.BaseNode = 0; f.KeySize = 1;
    Node branch; std::memset(&branch, 0, sizeof(branch)); branch.KeyBitStart = 0; branch.KeyBitCount = 2;
    Node leaf; std::memset(&leaf, 0, sizeof(leaf)); leaf.IsLeaf = 1; leaf.Key()[0] = 1;
    Node node; std::memset(&node, 0, sizeof(node)); node.IsLeaf = 1;
    node.Key()[0] = 2;                                       // slot 2 empty -> AddNode
    branch.Children()[1] = &leaf; branch.ChildListHead = branch.ChildListTail = &leaf;
    if (f.AddNodeAutoExpandTree(&branch, &node) != 0 || g_added != 1) { std::printf("00bf2e50_TEST FAIL\n"); return 1; }
    node.Key()[0] = 1 | 0x100;                               // same slot, differs at bit 8 -> outside window: rebuild
    if (f.AddNodeAutoExpandTree(&branch, &node) != &node || g_rebuilt != 1 || node.NextSibling != &branch) {
        std::printf("00bf2e50_TEST FAIL\n"); return 1;
    }
    branch.KeyBitCount = 1; leaf.Key()[0] = 0; branch.Children()[1] = &leaf; node.Key()[0] = 1;  // differs at bit 0, in window
    branch.ChildListHead = branch.ChildListTail = &leaf;
    if (f.AddNodeAutoExpandTree(&branch, &node) != 0 || branch.Children()[1] != &g_replacement || branch.ChildListHead != &g_replacement) {
        std::printf("00bf2e50_TEST FAIL\n"); return 1;
    }
    std::printf("00bf2e50_TEST PASS\n");
    return 0;
}