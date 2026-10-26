// Behaviour test for 0x007FFDB0 CTCGatherPointBase::RemoveAsUser: remove(user) then the single-position erase at the new end.
#include <cstdio>
class CThing { public: int id; };
struct CPtr { const CThing* PItem; };
static CPtr* Find(CPtr* first, CPtr* last, const CPtr& value)
{
    for (; first != last; ++first)
        if (first->PItem == value.PItem)
            return first;
    return last;
}
static CPtr* RemoveCopy(CPtr* first, CPtr* last, CPtr* result, const CPtr& value)
{
    for (; first != last; ++first)
        if (!(first->PItem == value.PItem)) {
            *result = *first;
            ++result;
        }
    return result;
}
static CPtr* Copy(CPtr* first, CPtr* last, CPtr* result)
{
    for (int n = (int)(last - first); n > 0; --n, ++first, ++result)
        *result = *first;
    return result;
}
struct CGatherPoint {
    CPtr Storage[8];
    CPtr* _M_start;
    CPtr* _M_finish;
    void RemoveAsUser(const CThing& user)
    {
        CPtr pointer;
        pointer.PItem = &user;
        CPtr* it = Find(_M_start, _M_finish, pointer);
        if (it != _M_finish) {
            CPtr* next = it;
            it = RemoveCopy(++next, _M_finish, it, pointer);
        }
        if (it + 1 != _M_finish)
            Copy(it + 1, _M_finish, it);
        --_M_finish;
    }
    void Fill(const CThing* const* items, int count)
    {
        _M_start = Storage;
        _M_finish = Storage;
        for (int i = 0; i < count; ++i) {
            _M_finish->PItem = items[i];
            ++_M_finish;
        }
    }
    int Size() const { return (int)(_M_finish - _M_start); }
};

int main()
{
    CThing a = { 1 }, b = { 2 }, c = { 3 }, x = { 4 };
    const CThing* abc[3] = { &a, &b, &c };
    bool ok = true;

    CGatherPoint gp;
    gp.Fill(abc, 3);
    gp.RemoveAsUser(b);                                   // middle user goes, order kept
    ok = ok && gp.Size() == 2 && gp._M_start[0].PItem == &a && gp._M_start[1].PItem == &c;

    gp.Fill(abc, 3);
    gp.RemoveAsUser(c);                                   // last user: no copy, just the pop
    ok = ok && gp.Size() == 2 && gp._M_start[0].PItem == &a && gp._M_start[1].PItem == &b;

    gp.Fill(abc, 3);
    gp.RemoveAsUser(a);                                   // first user: the tail shifts down
    ok = ok && gp.Size() == 2 && gp._M_start[0].PItem == &b && gp._M_start[1].PItem == &c;

    gp.Fill(abc, 3);
    gp.RemoveAsUser(x);                                   // not a user: erase(end()) still pops the last slot
    ok = ok && gp.Size() == 2 && gp._M_start[0].PItem == &a && gp._M_start[1].PItem == &b;

    if (!ok) {
        std::printf("007ffdb0_TEST FAIL\n");
        return 1;
    }
    std::printf("007ffdb0_TEST PASS\n");
    return 0;
}