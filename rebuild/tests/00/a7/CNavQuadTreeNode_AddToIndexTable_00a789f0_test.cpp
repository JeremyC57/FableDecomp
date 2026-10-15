#include <cstdio>
struct Node;
struct Table { const Node* keys[16]; long vals[16]; int n; void insert(const Node* k, long v) { keys[n] = k; vals[n] = v; ++n; } };
struct Node {
    Node* TL; Node* TR; Node* BL; Node* BR; long type;
    long GetNodeType() const { return type; }
    void AddToIndexTable(long& index, Table& table) const
    {
        if (TL) {
            if (TL->GetNodeType() != 2) TL->AddToIndexTable(index, table);
            if (TR->GetNodeType() != 2) TR->AddToIndexTable(index, table);
            if (BL->GetNodeType() != 2) BL->AddToIndexTable(index, table);
            if (BR->GetNodeType() != 2) BR->AddToIndexTable(index, table);
        }
        table.insert(this, index);
        ++index;
    }
};
int main()
{
    Node c[4]; for (int i = 0; i < 4; ++i) { c[i].TL = c[i].TR = c[i].BL = c[i].BR = 0; c[i].type = 1; }
    c[2].type = 2;
    Node root; root.TL = &c[0]; root.TR = &c[1]; root.BL = &c[2]; root.BR = &c[3]; root.type = 0;
    Table t; t.n = 0; long index = 5;
    root.AddToIndexTable(index, t);
    if (index != 9 || t.n != 4 || t.keys[0] != &c[0] || t.vals[0] != 5 || t.keys[2] != &c[3] || t.keys[3] != &root || t.vals[3] != 8) { std::printf("00a789f0_TEST FAIL\n"); return 1; }
    std::printf("00a789f0_TEST PASS\n");
    return 0;
}