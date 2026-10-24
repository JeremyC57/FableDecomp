// Behaviour test for 0x0070B240 vector<CEngineTextureDecal>::_M_insert_overflow (STLport grow-and-insert).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct CBlendList {
    int tag;
    CBlendList(int t) : tag(t) { ++g_live; }
    CBlendList(const CBlendList& o) : tag(o.tag) { ++g_live; }
    ~CBlendList() { --g_live; }
};
struct CEngineTextureDecal {
    int OriginalTextureIndex; int DecalID; CBlendList OverlaidBlends;
    CEngineTextureDecal(int a, int b) : OriginalTextureIndex(a), DecalID(b), OverlaidBlends(a * 10) {}
};

static CEngineTextureDecal* UCopy(const CEngineTextureDecal* f, const CEngineTextureDecal* l, CEngineTextureDecal* r)
{ for ( ; f != l; ++f, ++r) new (r) CEngineTextureDecal(*f); return r; }
static CEngineTextureDecal* UFill(CEngineTextureDecal* f, unsigned int n, const CEngineTextureDecal& x)
{ for ( ; n > 0; --n, ++f) new (f) CEngineTextureDecal(x); return f; }

struct CVector {
    CEngineTextureDecal* _M_start; CEngineTextureDecal* _M_finish; CEngineTextureDecal* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear() { for (CEngineTextureDecal* p = _M_start; p != _M_finish; ++p) p->~CEngineTextureDecal(); std::free(_M_start); }
    void _M_insert_overflow(CEngineTextureDecal* pos, const CEngineTextureDecal& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CEngineTextureDecal* new_start = len != 0 ? (CEngineTextureDecal*)std::malloc(len * sizeof(CEngineTextureDecal)) : 0;
        CEngineTextureDecal* new_finish = UCopy(_M_start, pos, new_start);
        if (fill_len == 1) { new (new_finish) CEngineTextureDecal(x); ++new_finish; }
        else new_finish = UFill(new_finish, fill_len, x);
        if (!atend) new_finish = UCopy(pos, _M_finish, new_finish);
        _M_clear();
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};

int main()
{
    CVector v; v._M_start = 0; v._M_finish = 0; v._M_end_of_storage = 0;
    CEngineTextureDecal a(1, 100), b(2, 200), c(3, 300);
    v._M_insert_overflow(v._M_finish, a, 1, true);                 // append to empty: capacity 1
    bool ok = v.size() == 1 && v._M_end_of_storage == v._M_start + 1 && v._M_start[0].DecalID == 100;
    v._M_insert_overflow(v._M_finish, b, 1, true);                 // capacity 2
    ok = ok && v.size() == 2 && v._M_end_of_storage == v._M_start + 2 && v._M_start[1].DecalID == 200;
    v._M_insert_overflow(v._M_start + 1, c, 3, false);             // insert 3 copies in the middle: len 2 + 3
    ok = ok && v.size() == 5 && v._M_end_of_storage == v._M_start + 5 && v._M_start[0].DecalID == 100
            && v._M_start[1].DecalID == 300 && v._M_start[3].OverlaidBlends.tag == 30 && v._M_start[4].DecalID == 200;
    v._M_clear();
    ok = ok && g_live == 3;
    if (!ok) { std::printf("0070b240_TEST FAIL\n"); return 1; }
    std::printf("0070b240_TEST PASS\n");
    return 0;
}