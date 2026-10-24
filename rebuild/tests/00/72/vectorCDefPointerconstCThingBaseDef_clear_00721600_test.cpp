// Behaviour test for 0x00721600 vector<CDefPointer<const CThingBaseDef> >::clear (callees modelled).
#include <cstdio>
struct CDef { long RefCount; };
struct TDefPtr { CDef* Object; };
static TDefPtr* copy_(const TDefPtr* first, const TDefPtr* last, TDefPtr* result)
{
    for (long n = (long)(last - first); n > 0; --n, ++first, ++result)
        if (result->Object != first->Object) {
            if (result->Object) --result->Object->RefCount;
            result->Object = first->Object;
            if (result->Object) ++result->Object->RefCount;
        }
    return result;
}
static void destroy_(TDefPtr* first, TDefPtr* last)
{
    for (; first != last; ++first) { if (first->Object) --first->Object->RefCount; first->Object = 0; }
}
struct CDefPointerVector {
    TDefPtr* _M_start; TDefPtr* _M_finish; TDefPtr* _M_end_of_storage;
    TDefPtr* erase(TDefPtr* first, TDefPtr* last)
    {
        TDefPtr* i = copy_(last, _M_finish, first);
        destroy_(i, _M_finish);
        _M_finish = i;
        return first;
    }
    void clear() { erase(_M_start, _M_finish); }
};
int main()
{
    CDef d[2];
    d[0].RefCount = 2; d[1].RefCount = 1;
    TDefPtr buf[4];
    buf[0].Object = &d[0]; buf[1].Object = &d[1]; buf[2].Object = &d[0];
    CDefPointerVector v;
    v._M_start = buf; v._M_finish = buf + 3; v._M_end_of_storage = buf + 4;
    v.clear();
    if (v._M_finish != v._M_start || d[0].RefCount != 0 || d[1].RefCount != 0 || buf[1].Object != 0) { std::printf("00721600_TEST FAIL\n"); return 1; }
    v.clear();
    if (v._M_finish != buf) { std::printf("00721600_TEST FAIL\n"); return 1; }
    std::printf("00721600_TEST PASS\n");
    return 0;
}