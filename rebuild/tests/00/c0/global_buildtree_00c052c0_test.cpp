// Test for zlib 1.2.1 trees.c build_tree (0x00C052C0): a heap of the used symbols is combined two at a time into a
// Huffman tree (parent depth = max(child depths) + 1, dad links set), a lone symbol gets a forced partner so at least
// two codes exist; the leaf code lengths follow from the dad chain.
#include <cstdio>
enum { L = 8, NODES = 2 * L };
static int heap[NODES + 1], heap_len, heap_max; static unsigned short freq[NODES], dad[NODES]; static unsigned char depth[NODES];
static bool smaller(int n, int m) { return freq[n] < freq[m] || (freq[n] == freq[m] && depth[n] <= depth[m]); }
static void pqdownheap(int k)
{
    int v = heap[k], j = k << 1;
    while (j <= heap_len) {
        if (j < heap_len && smaller(heap[j + 1], heap[j])) j++;
        if (smaller(v, heap[j])) break;
        heap[k] = heap[j]; k = j; j <<= 1;
    }
    heap[k] = v;
}
static int build_tree(int elems)
{
    int n, m, max_code = -1, node;
    heap_len = 0; heap_max = NODES + 1;
    for (n = 0; n < elems; n++) { if (freq[n] != 0) { heap[++heap_len] = max_code = n; depth[n] = 0; } }
    while (heap_len < 2) { node = heap[++heap_len] = (max_code < 2 ? ++max_code : 0); freq[node] = 1; depth[node] = 0; }
    for (n = heap_len / 2; n >= 1; n--) pqdownheap(n);
    node = elems;
    do {
        n = heap[1]; heap[1] = heap[heap_len--]; pqdownheap(1);
        m = heap[1];
        heap[--heap_max] = n; heap[--heap_max] = m;
        freq[node] = freq[n] + freq[m];
        depth[node] = (unsigned char)((depth[n] >= depth[m] ? (int)depth[n] : (int)depth[m]) + 1);
        dad[n] = dad[m] = (unsigned short)node;
        heap[1] = node++;
        pqdownheap(1);
    } while (heap_len >= 2);
    return max_code;
}
static int length_of(int leaf, int root) { int l = 0; while (leaf != root) { leaf = dad[leaf]; ++l; } return l; }
int main()
{
    unsigned short f[4] = { 1, 1, 2, 4 };
    for (int i = 0; i < 4; ++i) freq[i] = f[i];
    int max_code = build_tree(4);
    int root = 4 + 2;   // three merges: nodes 4, 5, 6
    bool ok = max_code == 3 && length_of(0, root) == 3 && length_of(1, root) == 3 && length_of(2, root) == 2
           && length_of(3, root) == 1 && depth[root] == 3 && freq[root] == 8;
    for (int i = 0; i < NODES; ++i) { freq[i] = 0; depth[i] = 0; dad[i] = 0; }
    freq[2] = 5;                                    // one symbol: a partner (symbol 0) is forced in
    max_code = build_tree(4);
    ok = ok && max_code == 2 && freq[0] == 1 && dad[0] == 4 && dad[2] == 4;
    std::printf(ok ? "00c052c0_TEST PASS\n" : "00c052c0_TEST FAIL\n");
    return ok ? 0 : 1;
}