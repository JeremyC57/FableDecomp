// test for 00433cec __copy<CWideString*>
#include <stdio.h>
class CWideString {
public:
    void* PData;
    const CWideString& operator=(const CWideString& s);        // 0x0099B7D0
};
namespace std {
struct random_access_iterator_tag {};
template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template CWideString* __fastcall __copy<CWideString*, CWideString*, int>(CWideString*, CWideString*, CWideString*, const random_access_iterator_tag&, int*);
}

static int g_as;
const CWideString& CWideString::operator=(const CWideString& s) { PData = s.PData; ++g_as; return *this; }
int main()
{
    CWideString src[3]; src[0].PData = (void*)1; src[1].PData = (void*)2; src[2].PData = (void*)3;
    CWideString dst[4]; for (int i = 0; i < 4; ++i) dst[i].PData = (void*)9;
    CWideString* e = std::__copy(src, src + 3, dst, std::random_access_iterator_tag(), (int*)0);
    int ok = e == dst + 3 && g_as == 3 && dst[0].PData == (void*)1 && dst[2].PData == (void*)3 && dst[3].PData == (void*)9;
    ok = ok && std::__copy(src, src, dst, std::random_access_iterator_tag(), (int*)0) == dst && g_as == 3;
    printf(ok ? "00433cec_TEST PASS\n" : "00433cec_TEST FAIL\n");
    return 0;
}