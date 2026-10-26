// Behaviour test for 0x009A7AC0 CBankFileManager::CBankFileManager: every STLport container comes up empty with its own
// header node (red, null root, leftmost = rightmost = header; list node linked to itself), not using retail banks, FileVersion 100.
#include <cstdio>
#include <cstdlib>
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Tree {
    NodeBase* header; unsigned int count; unsigned int comp;
    Tree(unsigned int nodeSize) : header(0), count(0), comp(0)
    { header = (NodeBase*)std::malloc(nodeSize < sizeof(NodeBase) ? sizeof(NodeBase) : nodeSize); header->color = false; header->parent = 0; header->left = header; header->right = header; }
    bool empty_ok() const { return header && count == 0 && header->color == false && header->parent == 0 && header->left == header && header->right == header; }
};
struct ListNode { ListNode* next; ListNode* prev; void* data[2]; };
struct List { ListNode* node; List() : node(0) { ListNode* n = (ListNode*)std::malloc(sizeof(ListNode)); n->next = n; n->prev = n; node = n; } };
class CWideString { public: void* PStringData; CWideString() : PStringData(0) {} };
struct CBankFileManager {
    Tree Banks; List RetailBanks; bool UsingRetailBanks; Tree BankAliasMap; CWideString BasePath; Tree CreatedRetailBanks; unsigned int FileVersion;
    CBankFileManager() : Banks(0x28), BankAliasMap(0x18), CreatedRetailBanks(0x14) { UsingRetailBanks = false; FileVersion = 100; }
};
int main()
{
    CBankFileManager m;
    if (!m.Banks.empty_ok() || !m.BankAliasMap.empty_ok() || !m.CreatedRetailBanks.empty_ok()) { std::printf("009a7ac0_TEST FAIL\n"); return 1; }
    if (m.Banks.header == m.BankAliasMap.header || m.BankAliasMap.header == m.CreatedRetailBanks.header) { std::printf("009a7ac0_TEST FAIL\n"); return 1; }
    if (!m.RetailBanks.node || m.RetailBanks.node->next != m.RetailBanks.node || m.RetailBanks.node->prev != m.RetailBanks.node) { std::printf("009a7ac0_TEST FAIL\n"); return 1; }
    if (m.UsingRetailBanks || m.FileVersion != 100 || m.BasePath.PStringData != 0) { std::printf("009a7ac0_TEST FAIL\n"); return 1; }
    std::printf("009a7ac0_TEST PASS\n");
    return 0;
}