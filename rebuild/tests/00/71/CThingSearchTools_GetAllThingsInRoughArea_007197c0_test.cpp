// behaviour test for 0x007197C0 CThingSearchTools::GetAllThingsInRoughArea: each thing linked into the overlapped
// mapwho cells is reported once per search (search stamp), only when it has the requested type bits
#include <cstdio>
#include <vector>
struct Entry { unsigned long stamp; unsigned long typeMask; int id; };
struct Node { Entry* e; Node* next; };
static unsigned long g_stamp = 0;
static void Search(Node** cells, int w, int x0, int y0, int x1, int y1, unsigned long mask, std::vector<int>& out) {
    ++g_stamp;
    for (int y = y0; y < y1; ++y) {
        Node** cell = cells + y * w + x0;                 // offset-based cell pointer, as GetCellPtr
        for (int x = x0; x < x1; ++x, ++cell)
            for (Node* n = *cell; n; n = n->next)
                if (n->e->stamp < g_stamp) { n->e->stamp = g_stamp; if ((n->e->typeMask & mask) == mask) out.push_back(n->e->id); }
    }
}
int main() {
    Entry a = {0, 0xF, 1}, b = {0, 0x3, 2};
    Node na1 = {&a, 0}, na2 = {&a, 0}, nb = {&b, &na2};
    Node* cells[4] = {&na1, &nb, 0, 0};
    std::vector<int> out; Search(cells, 2, 0, 0, 2, 2, 0xF, out);
    if (out.size() != 1 || out[0] != 1) { std::printf("007197c0_TEST FAIL\n"); return 1; }
    out.clear(); Search(cells, 2, 0, 0, 2, 2, 0x3, out);
    if (out.size() != 2) { std::printf("007197c0_TEST FAIL\n"); return 1; }
    std::printf("007197c0_TEST PASS\n"); return 0;
}