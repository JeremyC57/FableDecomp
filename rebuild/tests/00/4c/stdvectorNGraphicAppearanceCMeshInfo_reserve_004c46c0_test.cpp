// Behaviour test for 0x004C46C0 vector<CMeshInfo>::reserve(n): nothing when capacity >= n; otherwise a non-empty
// vector copies its 12-byte entries into a fresh n-slot block and frees the old one; an empty one just allocates.
#include <cstdio>
#include <cstdlib>

struct CMeshInfo { int Index; float SizeX; unsigned char PrimaryMesh : 1; };
static int g_copies = 0, g_frees = 0;

static CMeshInfo* UninitCopy(const CMeshInfo* f, const CMeshInfo* l, CMeshInfo* r)
{
    for (; f != l; ++f, ++r) { if (r) { *r = *f; ++g_copies; } }
    return r;
}

struct Vec {
    CMeshInfo* _M_start; CMeshInfo* _M_finish; CMeshInfo* _M_end;
    unsigned int size() const { return unsigned(_M_finish - _M_start); }
    unsigned int capacity() const { return unsigned(_M_end - _M_start); }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old = size();
            CMeshInfo* tmp;
            if (_M_start) {
                tmp = n ? static_cast<CMeshInfo*>(std::malloc(n * sizeof(CMeshInfo))) : 0;
                UninitCopy(_M_start, _M_finish, tmp);
                ++g_frees; std::free(_M_start);
            }
            else tmp = n ? static_cast<CMeshInfo*>(std::malloc(n * sizeof(CMeshInfo))) : 0;
            _M_start = tmp; _M_finish = tmp + old; _M_end = tmp + n;
        }
    }
};

int main()
{
    if (sizeof(CMeshInfo) != 12) { std::printf("004c46c0_TEST FAIL\n"); return 1; }
    Vec e = { 0, 0, 0 };
    e.reserve(2);
    if (e._M_start == 0 || e.size() != 0 || e.capacity() != 2 || g_frees != 0) { std::printf("004c46c0_TEST FAIL\n"); return 1; }
    e._M_start[0].Index = 7; e._M_start[0].SizeX = 1.5f; e._M_start[0].PrimaryMesh = 1;
    e._M_start[1].Index = 9; e._M_start[1].SizeX = 2.5f; e._M_start[1].PrimaryMesh = 0;
    e._M_finish = e._M_start + 2;
    e.reserve(2);                                      // capacity 2 >= 2: no change
    if (e.capacity() != 2 || g_frees != 0 || g_copies != 0) { std::printf("004c46c0_TEST FAIL\n"); return 1; }
    e.reserve(5);
    if (e.capacity() != 5 || e.size() != 2 || e._M_start[0].Index != 7 || e._M_start[1].SizeX != 2.5f ||
        e._M_start[0].PrimaryMesh != 1 || e._M_start[1].PrimaryMesh != 0 || g_frees != 1 || g_copies != 2) {
        std::printf("004c46c0_TEST FAIL\n"); return 1;
    }
    std::free(e._M_start);
    std::printf("004c46c0_TEST PASS\n");
    return 0;
}