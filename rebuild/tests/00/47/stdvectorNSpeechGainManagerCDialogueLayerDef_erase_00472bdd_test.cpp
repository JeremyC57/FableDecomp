// Behaviour test for 0x00472BDD std::vector<NSpeechGainManager::CDialogueLayerDef>::erase(first, last): the tail is
// assigned down over first, the leftover elements are destroyed in place, finish is pulled back and first returned.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif

static int g_dtors = 0, g_assigns = 0;
namespace NSpeechGainManager {
struct CDialogueLayerDef {
    int Data[15];
    virtual ~CDialogueLayerDef() { ++g_dtors; }
    CDialogueLayerDef& operator=(const CDialogueLayerDef& other) { ++g_assigns; memcpy(Data, other.Data, sizeof(Data)); return *this; }
};
}

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { };

template <class RandomAccessIter, class OutputIter, class Distance>
__forceinline OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                                const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class InputIter, class OutputIter>
__forceinline OutputIter __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
template <class ForwardIter>
__declspec(noinline) void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        (&*first)->NSpeechGainManager::CDialogueLayerDef::~CDialogueLayerDef();
}
template <class ForwardIter, class T>
__forceinline void __destroy(ForwardIter first, ForwardIter last, T*)
{
    __destroy_aux(first, last, __false_type());
}
template <class T> __forceinline T* __value_type(const T*) { return (T*)0; }
template <class ForwardIter>
__forceinline void _Destroy(ForwardIter first, ForwardIter last)
{
    __destroy(first, last, __value_type(first));
}
template <class T> class vector {
public:
    typedef T* iterator;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    iterator erase(iterator first, iterator last)
    {
        T* i = __copy_ptrs(last, _M_finish, first, __false_type());
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
};
}

int main()
{
    using NSpeechGainManager::CDialogueLayerDef;
    CDialogueLayerDef* a = new CDialogueLayerDef[6];
    for (int k = 0; k < 6; ++k) for (int j = 0; j < 15; ++j) a[k].Data[j] = k * 100 + j;
    std::vector<CDialogueLayerDef> v;
    v._M_start = a; v._M_finish = a + 6; v._M_end_of_storage = a + 6;
    int ok = 1;
    CDialogueLayerDef* r = v.erase(a + 1, a + 3);      // drop elements 1 and 2
    if (r != a + 1) ok = 0;
    if (v._M_finish != a + 4) ok = 0;
    if (g_assigns != 3 || g_dtors != 2) ok = 0;
    int expect[4] = { 0, 3, 4, 5 };
    for (int k = 0; k < 4; ++k) if (a[k].Data[0] != expect[k] * 100 || a[k].Data[14] != expect[k] * 100 + 14) ok = 0;
    r = v.erase(a + 2, a + 2);                          // empty range: the tail self-assigns, nothing is destroyed
    if (r != a + 2 || v._M_finish != a + 4 || g_assigns != 5 || g_dtors != 2) ok = 0;
    r = v.erase(a + 2, a + 4);                          // erase the tail: no assignment, two destructions
    if (r != a + 2 || v._M_finish != a + 2 || g_assigns != 5 || g_dtors != 4) ok = 0;
    if (a[0].Data[0] != 0 || a[1].Data[0] != 300) ok = 0;
    printf(ok ? "00472bdd_TEST PASS\n" : "00472bdd_TEST FAIL\n");
    return ok ? 0 : 1;
}