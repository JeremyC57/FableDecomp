// behaviour test for 0x005C14A6
#include <stdio.h>
#include <string.h>
#include <new>
namespace NInventory {
template <class T> class allocator { public: allocator() {} };
template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};
template <class T, class A> class _Vector_base {
public:
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) {}
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    explicit vector(const A& a = A()) : _Vector_base<T, A>(a) {}
};
class CPerItemData {
public:
    vector<long> Augmentations;                          // +0x00
    long         NumberOfKills;                          // +0x0c
    long         TimeWielded;                            // +0x10
    long         BestTrophyWitnessesAheadToDate;         // +0x14
    bool         QuestPersistent;                        // +0x18
    long         QuestPersistentRefCount;                // +0x1c
    float        BestFishWeight;                         // +0x20
    CPerItemData();
};
CPerItemData::CPerItemData()
    : NumberOfKills(0), TimeWielded(0), BestTrophyWitnessesAheadToDate(0), QuestPersistent(true), QuestPersistentRefCount(-1)
{
}
}

int main()
{
    unsigned char buf[0x24]; memset(buf, 0x77, sizeof(buf));
    NInventory::CPerItemData* d = new (buf) NInventory::CPerItemData;
    int ok = d->Augmentations._M_start == 0 && d->Augmentations._M_finish == 0 && d->Augmentations._M_end_of_storage._M_data == 0
          && d->NumberOfKills == 0 && d->TimeWielded == 0 && d->BestTrophyWitnessesAheadToDate == 0
          && d->QuestPersistent == true && d->QuestPersistentRefCount == -1 && buf[0x20] == 0x77 && sizeof(*d) == 0x24;
    printf(ok ? "005c14a6_TEST PASS\n" : "005c14a6_TEST FAIL\n");
    return ok ? 0 : 1;
}