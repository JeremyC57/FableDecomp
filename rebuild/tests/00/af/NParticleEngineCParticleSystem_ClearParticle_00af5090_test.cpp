// Behaviour test for 0x00AF5090 CParticleSystem::ClearParticle: a copy of the model (groups of handles + id lists).
#include <stdio.h>
#include <vector>
struct Prim { int count; int notified; int deleted; };
struct Handle { Prim* p; };
struct Particle {
    std::vector<Handle> ParentHandleList; std::vector<std::vector<long> > PrimIDList;
    bool GetParentHandle(long g, Handle& h) const { if ((long)ParentHandleList.size() > g) { h = ParentHandleList[g]; return h.p != 0; } return false; }
    bool GetPrimID(long g, long i, long& id) const { if ((long)PrimIDList.size() > g && (long)PrimIDList[g].size() > i) { id = PrimIDList[g][i]; return id >= 0; } return false; }
    void ClearParentHandle(long g) { if ((long)ParentHandleList.size() > g) { Prim* p = ParentHandleList[g].p; if (p) { if (p->count == 1) p->notified++; if (--p->count == 0) p->deleted++; } ParentHandleList[g].p = 0; PrimIDList[g].clear(); } }
    void ClearAll() { ParentHandleList.clear(); PrimIDList.clear(); }
};
struct Engine { std::vector<long> cleared; void ClearChildPrimitive(Handle&, long id) { cleared.push_back(id); } };
static void ClearParticle(Engine* PEngine, Particle* particle) {
    Handle handle;
    for (long group = 0; particle->GetParentHandle(group, handle); ++group) {
        if (PEngine && handle.p) {
            long id;
            for (long index = 0; particle->GetPrimID(group, index, id); ++index) PEngine->ClearChildPrimitive(handle, id);
            particle->ClearParentHandle(group);
        }
    }
    particle->ClearAll();
}
int main() {
    Prim a = {1, 0, 0}, b = {2, 0, 0};
    Particle p; Handle h; h.p = &a; p.ParentHandleList.push_back(h); h.p = &b; p.ParentHandleList.push_back(h); h.p = 0; p.ParentHandleList.push_back(h);
    std::vector<long> v; v.push_back(5); v.push_back(7); v.push_back(-1); v.push_back(9); p.PrimIDList.push_back(v);
    v.clear(); v.push_back(3); p.PrimIDList.push_back(v); v.clear(); v.push_back(11); p.PrimIDList.push_back(v);
    Engine e; ClearParticle(&e, &p);
    bool ok = e.cleared.size() == 3 && e.cleared[0] == 5 && e.cleared[1] == 7 && e.cleared[2] == 3
        && a.notified == 1 && a.deleted == 1 && b.notified == 0 && b.deleted == 0 && b.count == 1
        && p.ParentHandleList.empty() && p.PrimIDList.empty();
    Prim c = {1, 0, 0}; Particle q; h.p = &c; q.ParentHandleList.push_back(h); v.clear(); v.push_back(4); q.PrimIDList.push_back(v);
    ClearParticle(0, &q);   // no engine: the group handle is left alone, arrays still emptied
    ok = ok && c.notified == 0 && c.count == 1 && q.ParentHandleList.empty();
    if (!ok) { printf("00af5090_TEST FAIL\n"); return 1; }
    printf("00af5090_TEST PASS\n"); return 0;
}