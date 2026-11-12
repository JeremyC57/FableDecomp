// Behaviour test for 0x00AF5430 NParticleEngine::CParticleSystem::ClearDecal: a copy of the model (counted primitive, handle with
// Clear/assign, CDecal accessors GetParentHandle / GetPrimID / ClearParentHandle / ClearAll) driven through the same loop.
#include <cstdio>
#include <vector>
struct Prim {
    unsigned long refs; int notified; int deleted;
    Prim() : refs(1), notified(0), deleted(0) {}
    void AddRef() { ++refs; }
    long Release() { return (long)--refs; }
};
struct Handle {
    Prim* Data;
    Handle() : Data(0) {}
    Handle(const Handle& o) : Data(o.Data) { if (Data) Data->AddRef(); }
    ~Handle() { Clear(); }
    bool IsNull() const { return Data == 0; }
    void Clear() {
        if (Data && Data->refs == 1) Data->notified++;
        if (Data) { Prim* p = Data; if (p->Release() == 0) p->deleted++; Data = 0; }
    }
    Handle& operator=(const Handle& o) { Clear(); Data = o.Data; if (Data) Data->AddRef(); return *this; }
};
struct Decal {
    std::vector<std::vector<long> > PrimIDList;
    std::vector<Handle> ParentHandleList;
    bool GetParentHandle(long group, Handle& h) const {
        if (ParentHandleList.size() > (unsigned long)group) { h = ParentHandleList[group]; return !h.IsNull(); }
        return false;
    }
    bool GetPrimID(long group, long index, long& id) const {
        if (PrimIDList.size() > (unsigned long)group && PrimIDList[group].size() > (unsigned long)index) { id = PrimIDList[group][index]; return id >= 0; }
        return false;
    }
    void ClearParentHandle(long group) {
        if (ParentHandleList.size() > (unsigned long)group) { ParentHandleList[group].Clear(); PrimIDList[group].clear(); }
    }
    void ClearAll() { ParentHandleList.clear(); PrimIDList.clear(); }
};
struct Engine { std::vector<long> cleared; void ClearChildPrimitive(Handle&, unsigned long id) { cleared.push_back((long)id); } };
static void ClearDecal(Engine* PEngine, Decal* decal)
{
    Handle handle;
    for (long group = 0; decal->GetParentHandle(group, handle); ++group) {
        if (PEngine && !handle.IsNull()) {
            long id;
            for (long index = 0; decal->GetPrimID(group, index, id); ++index)
                PEngine->ClearChildPrimitive(handle, id);
        }
        decal->ClearParentHandle(group);
    }
    decal->ClearAll();
}
int main()
{
    Prim a, b, c;
    {
        Decal d;
        Handle h; h.Data = &a; a.refs = 1;
        d.ParentHandleList.push_back(h); h.Clear();       // a: refs 1 (held by list)
        a.notified = 0;
        Handle h2; h2.Data = &b; b.refs = 1; d.ParentHandleList.push_back(h2); h2.Clear(); b.notified = 0;
        Handle empty; d.ParentHandleList.push_back(empty);
        Handle h3; h3.Data = &c; c.refs = 1; d.ParentHandleList.push_back(h3); h3.Clear(); c.notified = 0;
        long g0[] = { 4, 7, -1, 9 }; long g1[] = { 11 }; long g3[] = { 99 };
        d.PrimIDList.push_back(std::vector<long>(g0, g0 + 4));
        d.PrimIDList.push_back(std::vector<long>(g1, g1 + 1));
        d.PrimIDList.push_back(std::vector<long>());
        d.PrimIDList.push_back(std::vector<long>(g3, g3 + 1));
        Engine e;
        ClearDecal(&e, &d);
        // group 0: ids 4,7 then stop at -1; group 1: id 11; group 2 empty handle stops the walk (group 3 never visited)
        bool ok = e.cleared.size() == 3 && e.cleared[0] == 4 && e.cleared[1] == 7 && e.cleared[2] == 11;
        ok = ok && d.ParentHandleList.empty() && d.PrimIDList.empty();
        ok = ok && a.deleted == 1 && b.deleted == 1 && c.deleted == 1;
        // null engine: a non-empty decal is still emptied and its handle released, with no engine call
        Prim p2; Decal d2;
        { Handle h4; h4.Data = &p2; p2.refs = 1; d2.ParentHandleList.push_back(h4); h4.Clear(); p2.notified = 0; }
        long g4[] = { 5, 6 }; d2.PrimIDList.push_back(std::vector<long>(g4, g4 + 2));
        unsigned long before = e.cleared.size();
        ClearDecal(0, &d2);
        ok = ok && e.cleared.size() == before && d2.ParentHandleList.empty() && d2.PrimIDList.empty() && p2.deleted == 1;
        if (!ok) { std::printf("00af5430_TEST FAIL\n"); return 1; }
    }
    std::printf("00af5430_TEST PASS\n");
    return 0;
}