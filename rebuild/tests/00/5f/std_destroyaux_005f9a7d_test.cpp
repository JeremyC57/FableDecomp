#include <stdio.h>
// Behaviour test for std::__destroy_aux<std::pair<long, CDefPointer<const CHeroAbilityDef> >*> (0x005F9A7D): links the row object,
// defines the pair destructor (0x005F8736) here and counts its calls over the range.
class CHeroAbilityDef;
template <class T> class CDefPointer { public:
    T* Object;
};

static int g_count = 0;
static long g_sum = 0;

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    ~pair();
};
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <> pair<long, CDefPointer<const CHeroAbilityDef> >::~pair()
{
    g_count++;
    g_sum += first;
}
}
typedef std::pair<long, CDefPointer<const CHeroAbilityDef> > Elem;

int main()
{
    static unsigned char storage[4 * sizeof(Elem)];
    Elem* arr = (Elem*)storage;
    for (int i = 0; i < 4; ++i) { arr[i].first = i + 1; arr[i].second.Object = 0; }
    std::__destroy_aux(arr, arr + 4, std::__false_type());
    // expect 4 calls, sum = 1+2+3+4 = 10
    if (g_count == 4 && g_sum == 10) {
        g_count = 0;
        std::__destroy_aux(arr, arr, std::__false_type());
        if (g_count == 0) {
            printf("DESTROY_RANGE_OK\n");
            return 0;
        }
    }
    printf("FAIL count=%d sum=%ld\n", g_count, g_sum);
    return 1;
}
