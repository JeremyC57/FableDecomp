// Behaviour test for CNavQuadTree::SaveToFile (0x00A78B20; file words are 32-bit `int` so the model runs on LP64 hosts): a standalone model of the record layout -- size placeholder, version 8,
// map size, region and action-point counts, the action points relative to the map origin, the layer count, the node index table
// (blocked node 0, nodes numbered from 1), the node count, each node's save, the end marker, and the size patched at the start.
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

struct File {
    unsigned char data[512]; int pos, len;
    File() : pos(0), len(0) { std::memset(data, 0, sizeof(data)); }
    void Write(const void* p, int n) { std::memcpy(data + pos, p, n); pos += n; if (pos > len) len = pos; }
    template <class T> void WriteVar(const T& v) { Write(&v, sizeof(T)); }
    int GetPosition() const { return pos; }
    void SetPosition(int p) { pos = p; }
    int ReadLong(int at) const { int v; std::memcpy(&v, data + at, 4); return v; }
    float ReadFloat(int at) const { float v; std::memcpy(&v, data + at, 4); return v; }
};
struct Node {
    int id;
    void AddToIndexTable(int& index, std::map<const Node*, int>& table) const { table[this] = index++; }
    void SaveToFile(File& file, const std::map<const Node*, int>& table, bool, const float*) const
    {
        int idx = table.find(this)->second; file.WriteVar(idx);
    }
};
static const Node BlockedNode = { 0 };
struct Pos { float X, Y; int Layer; };
struct Tree {
    float MapPos[2]; float MapWidth, MapHeight; int Regions;
    std::vector<Pos> ActionPoints;
    std::vector<std::vector<const Node*> > Layers;
    void SaveToFile(File& file) const
    {
        int startPos = file.GetPosition();
        int size = 0;
        file.WriteVar(size);
        file.WriteVar(8);
        file.WriteVar(MapWidth);
        file.WriteVar(MapHeight);
        file.WriteVar((unsigned int)Regions);
        file.WriteVar((unsigned int)ActionPoints.size());
        for (size_t i = 0; i < ActionPoints.size(); ++i) {
            Pos rel = { ActionPoints[i].X - MapPos[0], ActionPoints[i].Y - MapPos[1], ActionPoints[i].Layer };
            file.WriteVar(rel);
        }
        file.WriteVar((unsigned int)Layers.size());
        std::map<const Node*, int> table;
        table.insert(std::make_pair(&BlockedNode, 0));
        int index = 1;
        for (size_t l = 0; l < Layers.size(); ++l)
            for (size_t n = 0; n < Layers[l].size(); ++n)
                Layers[l][n]->AddToIndexTable(index, table);
        file.WriteVar(index - 1);
        for (size_t l = 0; l < Layers.size(); ++l)
            for (size_t n = 0; n < Layers[l].size(); ++n)
                Layers[l][n]->SaveToFile(file, table, true, MapPos);
        file.WriteVar((int)0xACBDEF12U);
        size = file.GetPosition();
        file.SetPosition(startPos);
        file.WriteVar(size);
        file.SetPosition(size);
    }
};

int main()
{
    Node a = { 1 }, b = { 2 }, c = { 3 };
    Tree t;
    t.MapPos[0] = 100.0f; t.MapPos[1] = 200.0f; t.MapWidth = 64.0f; t.MapHeight = 32.0f; t.Regions = 5;
    Pos p = { 110.0f, 230.0f, 1 }; t.ActionPoints.push_back(p);
    std::vector<const Node*> l0; l0.push_back(&a); l0.push_back(&b);
    std::vector<const Node*> l1; l1.push_back(&c);
    t.Layers.push_back(l0); t.Layers.push_back(l1);
    File f; f.pos = 4; f.len = 4;
    t.SaveToFile(f);
    // 4 + 6 longs + 12 (one action point) + layer count + node count + 3 node indexes + end marker
    int expect_end = 4 + 4 * 6 + 12 + 4 + 4 + 12 + 4;
    if (f.pos != expect_end || f.ReadLong(4) != expect_end || f.ReadLong(8) != 8 || f.ReadFloat(12) != 64.0f || f.ReadFloat(16) != 32.0f
        || f.ReadLong(20) != 5 || f.ReadLong(24) != 1 || f.ReadFloat(28) != 10.0f || f.ReadFloat(32) != 30.0f || f.ReadLong(36) != 1
        || f.ReadLong(40) != 2 || f.ReadLong(44) != 3 || f.ReadLong(48) != 1 || f.ReadLong(52) != 2 || f.ReadLong(56) != 3
        || (unsigned int)f.ReadLong(60) != 0xACBDEF12U) {
        std::printf("00a78b20_TEST FAIL\n"); return 1;
    }
    std::printf("00a78b20_TEST PASS\n");
    return 0;
}