// Behaviour test for CBankFileAsyncData::Abort (0x009D6350): a model of the abort decision.
#include <cstdio>
#include <set>
struct Bank { std::set<void*> instances, restarts; bool removeOk; int removes; };
struct Data {
    Bank* bank; bool Finished, Aborted; unsigned long uid; int pool; long handle; void* buffer; void* data; int frees;
    bool Abort()
    {
        if (Finished)
            return true;
        bool result = false;
        if (bank->restarts.erase(this) != 0)
            result = true;
        else if ((++bank->removes, bank->removeOk)) {
            uid = 0;
            result = true;
        }
        if (result) {
            bank->instances.erase(this);
            if (pool && handle != -1) { ++frees; handle = -1; buffer = 0; data = 0; }
            Aborted = true;
        }
        return result;
    }
};
int main()
{
    Bank b; b.removeOk = false; b.removes = 0;
    Data d = { &b, false, false, 7, 1, 5, &b, &b, 0 };
    b.instances.insert(&d);
    if (d.Abort() || d.Aborted || d.uid != 7 || b.removes != 1) { std::printf("009d6350_TEST FAIL\n"); return 1; }
    b.removeOk = true;
    if (!d.Abort() || !d.Aborted || d.uid != 0 || d.handle != -1 || d.frees != 1 || b.instances.size()) { std::printf("009d6350_TEST FAIL\n"); return 1; }
    Data e = { &b, false, false, 3, 0, 5, 0, 0, 0 };
    b.restarts.insert(&e); b.removeOk = false;
    if (!e.Abort() || e.uid != 3 || e.frees || b.restarts.size()) { std::printf("009d6350_TEST FAIL\n"); return 1; }
    Data f = { &b, true, false, 1, 0, 0, 0, 0, 0 };
    if (!f.Abort() || f.Aborted) { std::printf("009d6350_TEST FAIL\n"); return 1; }
    std::printf("009d6350_TEST PASS\n");
    return 0;
}