// Behaviour test for 0x00683B00 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <list>
#include <map>
struct Deed { int Type; long Handle; };
struct Opinion {
    std::list<long> Pending; std::list<long> Known[4];
    void ClearPendingDeeds(const std::map<long, Deed>* log) {
        if (log) {
            while (!Pending.empty()) {
                std::map<long, Deed>::const_iterator it = log->find(Pending.back());
                if (it != log->end()) Known[it->second.Type].push_front(it->second.Handle);
                Pending.pop_back();
            }
        } else Pending.clear();
    }
};
int main() {
    std::map<long, Deed> log; Deed a = { 1, 11 }, b = { 2, 22 }; log[11] = a; log[22] = b;
    Opinion o; o.Pending.push_back(11); o.Pending.push_back(99); o.Pending.push_back(22);
    o.ClearPendingDeeds(&log);
    bool ok = o.Pending.empty() && o.Known[1].size() == 1 && o.Known[1].front() == 11 && o.Known[2].front() == 22;
    o.Pending.push_back(11); o.ClearPendingDeeds(0); ok = ok && o.Pending.empty() && o.Known[1].size() == 1;
    std::printf(ok ? "00683b00_TEST PASS\n" : "00683b00_TEST FAIL\n");
    return ok ? 0 : 1;
}
