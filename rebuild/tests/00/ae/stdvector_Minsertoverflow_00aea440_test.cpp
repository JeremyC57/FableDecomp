// behaviour test for 0x00aea440 std::vector<std::pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> > >::_M_insert_overflow: the new block holds
// size + max(size, fill_len) elements; head, fill_len copies of x and (unless atend) the tail are copy-constructed in
// order (each copy of a shared pointer bumps its count); the old elements are destroyed (count dropped) and freed
#include <cstdio>
#include <cstdlib>
struct Info { unsigned int RefCount; };
struct Elem { int key; void* data; Info* info; };
struct Vec { Elem* start; Elem* finish; Elem* eos; };
static int gAllocs = 0, gFrees = 0;
static void CopyElem(Elem* p, const Elem& x) { if (p) { p->key = x.key; p->data = x.data; p->info = x.info; if (p->info) ++p->info->RefCount; } }
static void DestroyElem(Elem* p) { if (p->info) --p->info->RefCount; p->data = 0; p->info = 0; }
static void InsertOverflow(Vec& v, Elem* pos, const Elem& x, unsigned int fillLen, bool atend)
{
    unsigned int oldSize = (unsigned int)(v.finish - v.start);
    unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
    Elem* ns = len ? (++gAllocs, (Elem*)std::malloc(len * sizeof(Elem))) : 0;
    Elem* nf = ns;
    for (Elem* p = v.start; p != pos; ++p, ++nf) CopyElem(nf, *p);
    if (fillLen == 1) { CopyElem(nf, x); ++nf; }
    else for (unsigned int n = fillLen; n > 0; --n, ++nf) CopyElem(nf, x);
    if (!atend) for (Elem* p = pos; p != v.finish; ++p, ++nf) CopyElem(nf, *p);
    for (Elem* p = v.start; p != v.finish; ++p) DestroyElem(p);
    if (v.start) { ++gFrees; std::free(v.start); }
    v.start = ns; v.finish = nf; v.eos = ns + len;
}
int main()
{
    int fails = 0;
    Info shared = { 1 }, other = { 1 };
    Vec v; v.start = v.finish = v.eos = 0;
    Elem x = { 7, &shared, &shared };
    InsertOverflow(v, v.finish, x, 1, true);
    if (gAllocs != 1 || v.finish - v.start != 1 || v.eos != v.finish || shared.RefCount != 2 || v.start[0].key != 7) fails |= 1;
    Elem y = { 3, &other, &other };
    InsertOverflow(v, v.start, y, 3, false);
    if (gAllocs != 2 || gFrees != 1 || v.finish - v.start != 4 || v.eos - v.start != 4) fails |= 2;
    if (v.start[0].key != 3 || v.start[2].key != 3 || v.start[3].key != 7) fails |= 4;
    if (other.RefCount != 4 || shared.RefCount != 2) fails |= 8;
    Elem z = { 9, 0, 0 };
    InsertOverflow(v, v.start + 1, z, 1, false);
    if (v.finish - v.start != 5 || v.eos - v.start != 8 || v.start[1].key != 9 || v.start[1].info || v.start[4].key != 7) fails |= 16;
    if (other.RefCount != 4 || shared.RefCount != 2) fails |= 32;
    std::free(v.start);
    if (fails) { std::printf("00aea440_TEST FAIL %d\n", fails); return 1; }
    std::printf("00aea440_TEST PASS\n");
    return 0;
}