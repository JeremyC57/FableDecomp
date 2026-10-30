// Behaviour test for 0x00AF2010 std::vector<NParticleEngine::CParticleEmitterInfo>::_M_fill_insert (STLport 4.6 logic over an
// intrusive counted pointer element: every copy add-refs, every overwrite/destroy releases, Delete at zero).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_deleted = 0;
struct Pointee {
    unsigned int IVRefCount;
    int id;
    void Delete() { ++g_deleted; }
};
struct CountedPtr {
    Pointee* Object;
    CountedPtr() : Object(0) { }
    CountedPtr(const CountedPtr& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~CountedPtr() { if (Object) { if (--Object->IVRefCount == 0) Object->Delete(); Object = 0; } }
    CountedPtr& operator=(const CountedPtr& o)
    {
        if (Object != o.Object) {
            if (Object) { if (--Object->IVRefCount == 0) Object->Delete(); Object = 0; }
            Object = o.Object;
            if (Object) ++Object->IVRefCount;
        }
        return *this;
    }
};
struct Info { CountedPtr Data; void* Bank; unsigned int Index; };

struct InfoVector { Info* start; Info* finish; Info* eos; int overflow; };

static Info* UninitCopy(Info* f, Info* l, Info* r) { for (; f != l; ++f, ++r) new (r) Info(*f); return r; }
static Info* UninitFillN(Info* f, unsigned int n, const Info& x) { for (; n > 0; --n, ++f) new (f) Info(x); return f; }
static Info* CopyBackward(Info* f, Info* l, Info* r) { for (int n = (int)(l - f); n > 0; --n) *--r = *--l; return r; }
static void Fill(Info* f, Info* l, const Info& v) { for (; f != l; ++f) *f = v; }

static void FillInsert(InfoVector* v, Info* position, unsigned int n, const Info& x)
{
    if (n != 0) {
        if ((unsigned int)(v->eos - v->finish) >= n) {
            Info x_copy = x;
            const unsigned int elems_after = (unsigned int)(v->finish - position);
            Info* old_finish = v->finish;
            if (elems_after > n) {
                UninitCopy(v->finish - n, v->finish, v->finish);
                v->finish += n;
                CopyBackward(position, old_finish - n, old_finish);
                Fill(position, position + n, x_copy);
            } else {
                UninitFillN(v->finish, n - elems_after, x_copy);
                v->finish += n - elems_after;
                UninitCopy(position, old_finish, v->finish);
                v->finish += elems_after;
                Fill(position, old_finish, x_copy);
            }
        } else
            ++v->overflow;
    }
}

int main()
{
    int fail = 0;
    Pointee p[4];
    for (int i = 0; i < 4; ++i) { p[i].IVRefCount = 1; p[i].id = i; }   // the test holds one reference each
    InfoVector v;
    v.start = (Info*)std::malloc(8 * sizeof(Info)); v.finish = v.start; v.eos = v.start + 8; v.overflow = 0;
    Info proto;
    for (int i = 0; i < 3; ++i) { proto.Data.Object = &p[i]; ++p[i].IVRefCount; proto.Bank = 0; proto.Index = i; new (v.finish++) Info(proto); proto.Data.Object = 0; --p[i].IVRefCount; }
    // vector: p0 p1 p2 (each count 2)
    Info x; x.Data.Object = &p[3]; ++p[3].IVRefCount; x.Bank = 0; x.Index = 99;   // p3 count 2
    FillInsert(&v, v.start + 1, 1, x);                 // elems_after 2 > 1: p0 p3 p1 p2
    if (v.finish - v.start != 4 || v.start[1].Data.Object != &p[3] || v.start[2].Data.Object != &p[1] || v.start[3].Data.Object != &p[2]) fail = 1;
    if (p[3].IVRefCount != 3 || p[1].IVRefCount != 2 || p[2].IVRefCount != 2) fail = 1;
    FillInsert(&v, v.start + 3, 2, x);                 // elems_after 1 <= 2: p0 p3 p1 p3 p3 p2
    if (v.finish - v.start != 6 || v.start[3].Data.Object != &p[3] || v.start[4].Data.Object != &p[3] || v.start[5].Data.Object != &p[2]) fail = 1;
    if (p[3].IVRefCount != 5 || p[2].IVRefCount != 2 || v.start[4].Index != 99) fail = 1;
    FillInsert(&v, v.start, 0, x);
    FillInsert(&v, v.start, 5, x);                     // no room: overflow path
    if (v.overflow != 1 || v.finish - v.start != 6) fail = 1;
    for (Info* i = v.start; i != v.finish; ++i) i->~Info();
    std::free(v.start);
    if (p[3].IVRefCount != 2 || p[0].IVRefCount != 1 || g_deleted != 0) fail = 1;
    if (fail) { std::printf("00af2010_TEST FAIL\n"); return 1; }
    std::printf("00af2010_TEST PASS\n");
    return 0;
}