// Behaviour test for CNavNavigableLeafNode::AddToIndexTable (0x00A787D0): this node goes into the table under the current index, then the index advances.
#include <cstdio>
struct Node;
struct Pair { const Node* first; int second; };
struct Table { Pair entries[8]; int count;
    void insert(const Pair& p) { for (int i = 0; i < count; ++i) if (entries[i].first == p.first) return; entries[count++] = p; } };
struct Node { void AddToIndexTable(int& index, Table& table) const { Pair p; p.first = this; p.second = index; table.insert(p); ++index; } };
int main()
{
    Table t; t.count = 0; int index = 5; Node a, b;
    a.AddToIndexTable(index, t); b.AddToIndexTable(index, t);
    if (index != 7 || t.count != 2 || t.entries[0].first != &a || t.entries[0].second != 5 || t.entries[1].second != 6) {
        std::printf("00a787d0_TEST FAIL\n"); return 1; }
    a.AddToIndexTable(index, t);                       // insert_unique: an existing key keeps its index, the counter still moves
    if (index != 8 || t.count != 2 || t.entries[0].second != 5) { std::printf("00a787d0_TEST FAIL\n"); return 1; }
    std::printf("00a787d0_TEST PASS\n");
    return 0;
}