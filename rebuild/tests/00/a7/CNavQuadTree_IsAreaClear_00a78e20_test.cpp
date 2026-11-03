// Behaviour test for CNavQuadTree::IsAreaClear (0x00A78E20): a standalone model of the three checks -- a 255 topology weight under a
// layer-0 node's square fails at once; the line-list cells the square covers (floor of edge / 8, at least one cell each way) are
// collected and any blocking list fails; each switchable list that blocks marks `switchable`, records its id and fails.
#include <cstdio>
#include <cmath>
#include <vector>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct Box { float TLX, TLY, BRX, BRY; };
struct Line { float x1, y1, x2, y2; };
static const float g_sizes[7] = { 32, 16, 8, 4, 2, 1, 0.5f };
struct Node { float x, y; int level, layer; };
struct Switchable { u64 id; std::vector<Line> lines; };
struct Tree {
    float mapX, mapY, mapWidth;
    std::vector<unsigned char> weights;
    std::vector<std::vector<Line> > lineLists;                // one layer
    std::vector<Switchable> switchables;
    static bool Blocks(const Box& a, const std::vector<const std::vector<Line>*>& lists)
    {
        for (size_t i = 0; i < lists.size(); ++i)
            for (size_t j = 0; j < lists[i]->size(); ++j) {
                const Line& l = (*lists[i])[j];
                if (l.x1 >= a.TLX && l.x1 < a.BRX && l.y1 >= a.TLY && l.y1 < a.BRY) return true;
            }
        return false;
    }
    bool IsAreaClear(const Node& n, bool& sw, std::vector<u64>& ids) const
    {
        sw = false;
        float half = g_sizes[n.level] * 0.5f;
        Box area = { n.x - half, n.y - half, half + n.x, half + n.y };
        if (n.layer == 0 && !weights.empty())
            for (long x = (long)(area.TLX - mapX); x < area.BRX - mapX; ++x)
                for (long y = (long)(area.TLY - mapY); y < area.BRY - mapY; ++y)
                    if (weights[(long)(y * mapWidth + x)] == 255) return false;
        std::vector<const std::vector<Line>*> lines;
        if (!lineLists.empty()) {
            float rx = n.x - mapX, ry = n.y - mapY;
            long minX = (long)std::floor((rx - half) / 8), minY = (long)std::floor((ry - half) / 8);
            long maxX = (long)std::floor((rx + half) / 8), maxY = (long)std::floor((ry + half) / 8);
            if (maxX == minX) ++maxX;
            if (maxY == minY) ++maxY;
            for (unsigned long x = minX; x < (unsigned long)maxX; ++x)
                for (unsigned long y = minY; y < (unsigned long)maxY; ++y)
                    lines.push_back(&lineLists[(long)(x + y * mapWidth / 8.0f)]);
        }
        if (Blocks(area, lines)) return false;
        for (size_t i = 0; i < switchables.size(); ++i) {
            std::vector<const std::vector<Line>*> one(1, &switchables[i].lines);
            if (Blocks(area, one)) { sw = true; ids.push_back(switchables[i].id); }
        }
        return !sw;
    }
};
int main()
{
    Tree t; t.mapX = 0; t.mapY = 0; t.mapWidth = 32;
    t.weights.assign(32 * 32, 0);
    t.lineLists.resize(4 * 4);
    Node n = { 12.0f, 12.0f, 2, 0 };                           // square 8..16 (one cell: 1,1)
    bool sw; std::vector<u64> ids;
    if (!t.IsAreaClear(n, sw, ids) || sw) { std::printf("00a78e20_TEST FAIL\n"); return 1; }
    t.weights[10 * 32 + 9] = 255;                              // under the square
    if (t.IsAreaClear(n, sw, ids)) { std::printf("00a78e20_TEST FAIL\n"); return 1; }
    t.weights[10 * 32 + 9] = 0;
    Line l = { 10, 10, 11, 11 };
    t.lineLists[1 * 4 + 1].push_back(l);                       // the covered cell
    if (t.IsAreaClear(n, sw, ids)) { std::printf("00a78e20_TEST FAIL\n"); return 1; }
    t.lineLists[1 * 4 + 1].clear();
    Switchable s; s.id = ((((u64)1) << 32) | 2); s.lines.push_back(l);
    t.switchables.push_back(s);
    if (t.IsAreaClear(n, sw, ids) || !sw || ids.size() != 1 || ids[0] != ((((u64)1) << 32) | 2)) { std::printf("00a78e20_TEST FAIL\n"); return 1; }
    std::printf("00a78e20_TEST PASS\n");
    return 0;
}