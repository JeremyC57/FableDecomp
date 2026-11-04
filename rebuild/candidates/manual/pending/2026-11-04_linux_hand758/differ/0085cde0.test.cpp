// Behaviour test for NThingMap::CThingMap::Validate (0x0085CDE0), g++ model: walks every cell of every level (1x1 for the unbounded level) and each node list, changing nothing.
#include <cstdio>
#include <vector>
struct Node { Node* next; };
static long Validate(const std::vector<long>& sizes, long w, long h, std::vector<std::vector<Node*> >& cells)
{
    long visited = 0;
    for (size_t level = 0; level < sizes.size(); ++level) {
        long width = 1, height = 1;
        if (sizes[level] != -1) { width = w / sizes[level]; height = h / sizes[level]; }
        for (long y = 0; y < height; ++y)
            for (long x = 0; x < width; ++x)
                for (Node* n = cells[level][y * width + x]; n; n = n->next) { for (Node* o = n->next; o; o = o->next) { } ++visited; }
    }
    return visited;
}
int main()
{
    std::vector<long> sizes; sizes.push_back(10); sizes.push_back(-1);
    std::vector<std::vector<Node*> > cells(2);
    cells[0].assign(4, (Node*)0); cells[1].assign(1, (Node*)0);
    Node a = { 0 }, b = { &a }, c = { 0 };
    cells[0][3] = &b; cells[1][0] = &c;
    long v = Validate(sizes, 20, 20, cells);
    int ok = v == 3 && b.next == &a && cells[0][3] == &b;
    std::printf(ok ? "0085cde0_TEST PASS\n" : "0085cde0_TEST FAIL\n");
    return ok ? 0 : 1;
}
