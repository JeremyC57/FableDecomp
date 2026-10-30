// Behaviour test for 0x0085AE20 std::list<CCountedPointer<CCombatActionBase::CStrikeEvent> >::push_back (see
// src/0085ae20.cpp): a node holding a copy of the counted pointer is linked in before the sentinel (at the back); the
// copy shares the pointer info, so its reference count goes up by one.
#include <cstdio>
#include <list>
struct Info { unsigned long RefCount; };
struct Ptr {
    int* Data; Info* Info_;
    Ptr(int* d, Info* i) : Data(d), Info_(i) { }
    Ptr(const Ptr& o) : Data(o.Data), Info_(o.Info_) { if (Info_) ++Info_->RefCount; }
    ~Ptr() { if (Info_) --Info_->RefCount; }
};
int main()
{
    int a = 1, b = 2; Info ia = { 1 }, ib = { 1 };
    bool ok;
    {
        std::list<Ptr> l;
        Ptr pa(&a, &ia), pb(&b, &ib), pn(0, 0);
        l.push_back(pa); l.push_back(pb); l.push_back(pn);
        ok = l.size() == 3 && *l.front().Data == 1 && l.back().Data == 0 && ia.RefCount == 2 && ib.RefCount == 2;
        pa.Info_ = 0; pb.Info_ = 0;
    }
    ok = ok && ia.RefCount == 1 && ib.RefCount == 1;
    std::printf(ok ? "0085ae20_TEST PASS\n" : "0085ae20_TEST FAIL\n");
    return ok ? 0 : 1;
}