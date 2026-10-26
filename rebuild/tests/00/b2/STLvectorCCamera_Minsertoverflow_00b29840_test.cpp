#include <cstdio>
#include <cstdlib>
#include <new>

// Model of STLport vector<CCamera>::_M_insert_overflow with an out-of-line CCamera copy constructor.
static int g_copies = 0;
class CCamera { public:
    float pos[3];
    float rh[6];
    bool heightLocked, zLocked;
    unsigned int fovFlags;
    float hfov, vfov, zoom;
    CCamera() { }
    CCamera(const CCamera& o);
};
CCamera::CCamera(const CCamera& o)
{
    ++g_copies;
    for (int i = 0; i < 3; ++i) pos[i] = o.pos[i];
    for (int i = 0; i < 6; ++i) rh[i] = o.rh[i];
    heightLocked = o.heightLocked; zLocked = o.zLocked; fovFlags = o.fovFlags; hfov = o.hfov; vfov = o.vfov; zoom = o.zoom;
}
struct Vec {
    CCamera* start; CCamera* finish; CCamera* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void InsertOverflow(CCamera* pos, const CCamera& x, unsigned int fillLen, bool atEnd)
    {
        unsigned int oldSize = size();
        unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
        CCamera* ns = len ? (CCamera*)std::malloc(len * sizeof(CCamera)) : 0;
        CCamera* nf = ns;
        for (CCamera* p = start; p != pos; ++p, ++nf) new (nf) CCamera(*p);
        if (fillLen == 1) { new (nf) CCamera(x); ++nf; }
        else for (unsigned int i = 0; i < fillLen; ++i, ++nf) new (nf) CCamera(x);
        if (!atEnd) for (CCamera* p = pos; p != finish; ++p, ++nf) new (nf) CCamera(*p);
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    CCamera c; c.pos[0] = 1.0f; c.zoom = 2.0f; c.heightLocked = true;
    v.InsertOverflow(v.finish, c, 1, true);
    v.InsertOverflow(v.finish, c, 1, true);
    if (v.size() != 2 || v.eos - v.start != 2 || g_copies != 3 || v.start[1].zoom != 2.0f || !v.start[1].heightLocked) { std::printf("00b29840_TEST FAIL\n"); return 1; }
    CCamera d; d.pos[0] = 5.0f; d.zoom = 6.0f; d.heightLocked = false;
    v.InsertOverflow(v.start + 1, d, 2, false);
    if (v.size() != 4 || v.eos - v.start != 4 || v.start[1].pos[0] != 5.0f || v.start[3].zoom != 2.0f) { std::printf("00b29840_TEST FAIL\n"); return 1; }
    std::free(v.start);
    std::printf("00b29840_TEST PASS\n");
    return 0;
}