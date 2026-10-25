// behaviour test for 0x00AE7880 std::vector<std::set<long> >::vector(unsigned int n): n empty sets, each with its own
// header (colour red, no root, leftmost = rightmost = header); the temporary's header is freed again
#include <cstdio>
#include <cstdlib>
#include <new>
static int gLive = 0;
static void* Alloc(unsigned int n) { if (n == 0) return 0; ++gLive; return std::malloc(n); }
static void Free(void* p) { if (p) { --gLive; std::free(p); } }
struct SNode { bool Color; SNode* Parent; SNode* Left; SNode* Right; };
struct SSet {
    SNode* Header; unsigned int Count;
    SSet() { Header = (SNode*)Alloc(sizeof(SNode)); Header->Color = false; Header->Parent = 0; Header->Left = Header->Right = Header; Count = 0; }
    SSet(const SSet& o) { Header = (SNode*)Alloc(sizeof(SNode)); Header->Color = false; Header->Parent = 0; Header->Left = Header->Right = Header; Count = o.Count; }
    ~SSet() { Free(Header); }
};
struct SVector {
    SSet* Start; SSet* Finish; SSet* EndOfStorage;
    explicit SVector(unsigned int n)
    {
        Start = (SSet*)Alloc(n * sizeof(SSet));
        Finish = Start;
        EndOfStorage = Start + n;
        SSet tmp;
        SSet* cur = Start;
        for (unsigned int k = n; k > 0; --k, ++cur) new (cur) SSet(tmp);
        Finish = cur;
    }
};
int main()
{
    SVector v(4);
    if (v.Finish - v.Start != 4 || v.EndOfStorage != v.Finish) { std::printf("00ae7880_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) {
        SNode* h = v.Start[i].Header;
        if (h->Color || h->Parent != 0 || h->Left != h || h->Right != h || v.Start[i].Count != 0) { std::printf("00ae7880_TEST FAIL\n"); return 1; }
        if (i && h == v.Start[i - 1].Header) { std::printf("00ae7880_TEST FAIL\n"); return 1; }
    }
    if (gLive != 5) { std::printf("00ae7880_TEST FAIL\n"); return 1; }   // storage + 4 headers (temporary freed)
    SVector e(0);
    if (e.Start != 0 || e.Finish != 0) { std::printf("00ae7880_TEST FAIL\n"); return 1; }
    std::printf("00ae7880_TEST PASS\n");
    return 0;
}