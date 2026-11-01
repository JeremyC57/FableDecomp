// Behaviour test for CFileInstaller::ProcessAlways (0x009C4F20): a model of the completion hand-off, the finished-write banking,
// the head-of-list retirement loop and the next-write start.
#include <cstdio>
#include <list>
#include <set>
struct File { int finished; long out; long in; int decompress; int id; };
struct Inst { File* read; File* write; long total; std::list<File> active; std::list<int> finish, copy; std::set<int> done; int completed; int started; };
static void ProcessAlways(Inst& i)
{
    if (i.read) { i.read = 0; }
    if (i.write && i.write->finished) { i.total += i.write->out; i.write = 0; }
    while (!i.active.empty() && i.active.front().finished) {
        File& f = i.active.front(); i.finish.remove(f.id); i.done.insert(f.id); i.active.pop_front();
        if (i.finish.empty() && i.copy.empty()) i.completed = 1;
    }
    if (i.write == 0 && !i.active.empty()) { File& f = i.active.front(); if (!f.decompress) ++i.started; i.write = &i.active.front(); }
}
int main()
{
    Inst i = { 0, 0, 0 }; i.completed = 0; i.started = 0;
    File a = { 1, 10, 5, 0, 1 }, b = { 0, 20, 7, 1, 2 };
    i.active.push_back(a); i.active.push_back(b); i.finish.push_back(1); i.finish.push_back(2);
    File r = { 0, 0, 0, 0, 9 }; i.read = &r; i.write = &i.active.front();
    ProcessAlways(i);
    bool ok = i.read == 0 && i.total == 10 && i.done.count(1) == 1 && i.active.size() == 1 && i.write == &i.active.front() && i.started == 0 && !i.completed;
    i.active.front().finished = 1; i.copy.clear(); ProcessAlways(i);
    ok = ok && i.total == 30 && i.active.empty() && i.completed == 1 && i.write == 0;
    printf(ok ? "009c4f20_TEST PASS\n" : "FAIL\n"); return ok ? 0 : 1;
}