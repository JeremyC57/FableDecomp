#include <cstdio>
// Behaviour test for CNavQuadTree::GetMemoryUsage (0x00A77E80): tallies are zeroed, layer-0 nodes add straight into the
// caller's tallies, layer-1 nodes add through locals, and the total is the sum of every node's return value.
class CANavQuadTreeNode { public:
    long bytes;
    virtual long GetMemoryUsage(long& a, long& b, long& c, long& d) { a += 1; b += 2; c += 3; d += 4; return bytes; }
};
template<class T> struct Vec { T* _M_start; T* _M_finish; unsigned long size() const { return _M_finish - _M_start; } T& operator[](unsigned long i) { return _M_start[i]; } };
struct Layer { Vec<CANavQuadTreeNode*> Nodes; };
class CNavQuadTree { public:
    Vec<Layer> NavigationLayers;
    long GetMemoryUsage(long& nodeBytes, long& lineBytes, long& switchableBytes, long& otherBytes)
    {
        nodeBytes = 0; lineBytes = 0; switchableBytes = 0; otherBytes = 0;
        long total = 0;
        for (CANavQuadTreeNode** it = NavigationLayers[0].Nodes._M_start; it != NavigationLayers[0].Nodes._M_finish; ++it)
            total += (*it)->GetMemoryUsage(nodeBytes, lineBytes, switchableBytes, otherBytes);
        if (NavigationLayers.size() > 1) {
            long n = 0, l = 0, s = 0, o = 0, layerTotal = 0;
            for (CANavQuadTreeNode** it = NavigationLayers[1].Nodes._M_start; it != NavigationLayers[1].Nodes._M_finish; ++it)
                layerTotal += (*it)->GetMemoryUsage(n, l, s, o);
            nodeBytes += n; lineBytes += l; switchableBytes += s; otherBytes += o;
            total += layerTotal;
        }
        return total;
    }
};
int main()
{
    CANavQuadTreeNode a, b, c; a.bytes = 10; b.bytes = 20; c.bytes = 300;
    CANavQuadTreeNode* l0[2] = { &a, &b };
    CANavQuadTreeNode* l1[1] = { &c };
    Layer layers[3];
    layers[0].Nodes._M_start = l0; layers[0].Nodes._M_finish = l0 + 2;
    layers[1].Nodes._M_start = l1; layers[1].Nodes._M_finish = l1 + 1;
    layers[2].Nodes._M_start = l1; layers[2].Nodes._M_finish = l1 + 1;
    CNavQuadTree t; t.NavigationLayers._M_start = layers; t.NavigationLayers._M_finish = layers + 2;
    long n = 99, l = 99, s = 99, o = 99;
    long total = t.GetMemoryUsage(n, l, s, o);
    int bad = total != 330 || n != 3 || l != 6 || s != 9 || o != 12;
    t.NavigationLayers._M_finish = layers + 1;
    total = t.GetMemoryUsage(n, l, s, o);
    bad |= total != 30 || n != 2 || o != 8;
    if (bad) { std::printf("00a77e80_TEST FAIL\n"); return 1; }
    std::printf("00a77e80_TEST PASS\n");
    return 0;
}