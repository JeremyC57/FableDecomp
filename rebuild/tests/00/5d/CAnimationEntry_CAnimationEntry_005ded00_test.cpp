#include <cstdio>
struct CComp { virtual ~CComp() {} unsigned long IVRefCount; long Value; int Kind; CComp(int k) : IVRefCount(1), Value(0), Kind(k) {} };
struct CAnimationEntry {
    long BankIndex, AnimName, GroupName; void* Components;
    CComp* added[3]; int count;
    void AddComponent(CComp* c) { added[count++] = c; }
    CAnimationEntry(long bankIndex, long transitionInTime, long flags, long delay)
        : AnimName(-1), GroupName(-1), Components(0), count(0)
    {
        BankIndex = bankIndex;
        CComp* t = new CComp(0); t->Value = transitionInTime; AddComponent(t);
        CComp* f = new CComp(1); f->Value = flags; AddComponent(f);
        CComp* d = new CComp(2); d->Value = delay; AddComponent(d);
    }
};
int main()
{
    CAnimationEntry e(7, 100, 0x30, 250);
    bool ok = e.BankIndex == 7 && e.AnimName == -1 && e.GroupName == -1 && e.Components == 0 && e.count == 3 &&
              e.added[0]->Kind == 0 && e.added[0]->Value == 100 && e.added[1]->Kind == 1 && e.added[1]->Value == 0x30 &&
              e.added[2]->Kind == 2 && e.added[2]->Value == 250 && e.added[2]->IVRefCount == 1;
    for (int i = 0; i < e.count; ++i) delete e.added[i];
    if (!ok) { std::printf("005ded00_TEST FAIL\n"); return 1; }
    std::printf("005ded00_TEST PASS\n");
    return 0;
}