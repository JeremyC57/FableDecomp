// Behaviour test for 0x00A80EE0 std::vector<CANavQuadTreeNode*>::operator=
#include <cstdio>
#include <cstdlib>
#include <cstring>

class CANavQuadTreeNode { public: int id; };

struct Vec {
    CANavQuadTreeNode** start; CANavQuadTreeNode** finish; CANavQuadTreeNode** eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    unsigned int capacity() const { return (unsigned int)(eos - start); }
    Vec& operator=(const Vec& x)
    {
        if (&x != this) {
            unsigned int xlen = x.size();
            if (xlen > capacity()) {
                CANavQuadTreeNode** tmp = xlen ? (CANavQuadTreeNode**)std::malloc(xlen * sizeof(void*)) : 0;
                if (x.finish != x.start) std::memmove(tmp, x.start, (x.finish - x.start) * sizeof(void*));
                if (start) std::free(start);
                start = tmp; eos = start + xlen;
            } else if (size() >= xlen) {
                if (x.finish != x.start) std::memmove(start, x.start, (x.finish - x.start) * sizeof(void*));
            } else {
                unsigned int n = size();
                if (n) std::memmove(start, x.start, n * sizeof(void*));
                std::memmove(finish, x.start + n, (xlen - n) * sizeof(void*));
            }
            finish = start + xlen;
        }
        return *this;
    }
};

static Vec make(CANavQuadTreeNode* nodes, int n, int cap)
{
    Vec v; v.start = (CANavQuadTreeNode**)std::malloc(cap * sizeof(void*));
    for (int i = 0; i < n; ++i) v.start[i] = &nodes[i];
    v.finish = v.start + n; v.eos = v.start + cap; return v;
}

static bool same(const Vec& a, const Vec& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i) if (a.start[i] != b.start[i]) return false;
    return true;
}

int main()
{
    CANavQuadTreeNode nodes[10];
    Vec big = make(nodes, 8, 8), small = make(nodes + 5, 2, 3), mid = make(nodes + 1, 4, 6);
    Vec a = make(nodes, 1, 1);
    a = big;                                   // reallocation path
    if (!same(a, big) || a.capacity() != 8) { std::printf("00a80ee0_TEST FAIL\n"); return 1; }
    a = small;                                 // shrink: copy over, finish moves back
    if (!same(a, small) || a.capacity() != 8) { std::printf("00a80ee0_TEST FAIL\n"); return 1; }
    a = mid;                                   // grow within capacity: copy + append
    if (!same(a, mid) || a.capacity() != 8) { std::printf("00a80ee0_TEST FAIL\n"); return 1; }
    Vec& r = (a = a);
    if (&r != &a || !same(a, mid)) { std::printf("00a80ee0_TEST FAIL\n"); return 1; }
    std::printf("00a80ee0_TEST PASS\n");
    return 0;
}