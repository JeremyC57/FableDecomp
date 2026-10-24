// Behaviour test for 0x0071F2E0 vector<CDefPointer<const CThingBaseDef> >::_M_allocate_and_copy.
#include <cstdio>
#include <cstdlib>
#include <new>
struct CDef { long RefCount; };
struct TDefPtr {
    CDef* Object;
    TDefPtr(const TDefPtr& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
};
struct CDefPointerVector {
    TDefPtr* _M_start; TDefPtr* _M_finish; TDefPtr* _M_end_of_storage;
    TDefPtr* allocate(unsigned int n) { return n != 0 ? (TDefPtr*)std::malloc(n * sizeof(TDefPtr)) : 0; }
    TDefPtr* _M_allocate_and_copy(unsigned int n, const TDefPtr* first, const TDefPtr* last)
    {
        TDefPtr* result = allocate(n);
        TDefPtr* cur = result;
        for (; first != last; ++first, ++cur)
            new (cur) TDefPtr(*first);
        return result;
    }
};
int main()
{
    CDef d[3];
    for (int i = 0; i < 3; ++i) d[i].RefCount = 1;
    TDefPtr* src = (TDefPtr*)std::malloc(4 * sizeof(TDefPtr));
    src[0].Object = &d[0]; src[1].Object = 0; src[2].Object = &d[2]; src[3].Object = &d[0];
    CDefPointerVector v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0;
    TDefPtr* r = v._M_allocate_and_copy(6, src, src + 4);
    if (!r || r[0].Object != &d[0] || r[1].Object != 0 || r[2].Object != &d[2] || r[3].Object != &d[0]
        || d[0].RefCount != 3 || d[1].RefCount != 1 || d[2].RefCount != 2) { std::printf("0071f2e0_TEST FAIL\n"); return 1; }
    TDefPtr* z = v._M_allocate_and_copy(0, src, src);
    if (z != 0) { std::printf("0071f2e0_TEST FAIL\n"); return 1; }
    std::free(r); std::free(src);
    std::printf("0071f2e0_TEST PASS\n");
    return 0;
}