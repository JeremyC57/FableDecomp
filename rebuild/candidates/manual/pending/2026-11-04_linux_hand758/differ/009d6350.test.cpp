// Behaviour test for CBankFileAsyncData::Abort (0x009D6350), g++ model: finished -> true; a pending restart or a cancelled read -> unregister, free, aborted, true; a read that cannot be cancelled -> false.
#include <cstdio>
#include <set>
struct Data { bool finished, aborted; unsigned long uid; long handle; bool hasPool; void* buffer; void* data; };
static bool Abort(Data& d, std::set<Data*>& restart, std::set<Data*>& instances, bool cancelOk, int& frees)
{
    if (d.finished) return true;
    bool result = false;
    if (restart.erase(&d) == 0) {
        if (!cancelOk) return result;
        d.uid = 0;
    }
    result = true;
    instances.erase(&d);
    if (d.hasPool && d.handle != -1) { ++frees; d.handle = -1; d.buffer = 0; d.data = 0; }
    d.aborted = true;
    return result;
}
int main()
{
    int frees = 0; std::set<Data*> rs, in;
    Data a = { true, false, 5, 3, true, &frees, &frees }; int ok = Abort(a, rs, in, false, frees) && !a.aborted;
    Data b = { false, false, 5, 3, true, &frees, &frees }; in.insert(&b);
    ok = ok && !Abort(b, rs, in, false, frees) && !b.aborted && b.uid == 5 && in.count(&b);
    ok = ok && Abort(b, rs, in, true, frees) && b.aborted && b.uid == 0 && !in.count(&b) && frees == 1 && b.handle == -1 && !b.buffer && !b.data;
    Data c = { false, false, 7, -1, true, 0, 0 }; rs.insert(&c);
    ok = ok && Abort(c, rs, in, false, frees) && c.aborted && c.uid == 7 && frees == 1 && rs.empty();
    std::printf(ok ? "009d6350_TEST PASS\n" : "009d6350_TEST FAIL\n");
    return ok ? 0 : 1;
}
