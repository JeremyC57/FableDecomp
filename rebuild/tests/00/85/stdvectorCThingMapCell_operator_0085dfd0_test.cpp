// Behaviour test for 0x0085DFD0 std::vector<NThingMap::CThingMapCell>::operator= (chunk 402): the three STLport branches --
// reallocate when the source outgrows the capacity, assign in place when it fits in the live size, assign the live part
// and copy-construct the rest when it fits the capacity only -- and self-assignment; the size always becomes the
// source's.
#include <cstdio>
#include <cstdlib>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#endif

namespace NThingMap {
class CMapwhoNode;
class CThingMapCell {
public:
    CMapwhoNode* PFirstNode;
};
}
namespace stl402 {
struct random_access_iterator_tag { };
struct __false_type { };
static int gUninitCopies = 0;
template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    for (; first != last; ++first, ++result) {
        new (result) NThingMap::CThingMapCell(*first);
        ++gUninitCopies;
    }
    return result;
}
template <class RandomAccessIter, class OutputIter, class Distance>
inline OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class T> class vector {
public:
    typedef T* pointer;
    typedef const T* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { _M_clear(); }
    void Reserve(unsigned int n, unsigned int size)
    {
        _M_clear();
        _M_start = (pointer)malloc(n * sizeof(T));
        _M_finish = _M_start + size;
        _M_end_of_storage = _M_start + n;
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int n, const_pointer first, const_pointer last)
    {
        pointer result = (pointer)malloc(n * sizeof(T));
        __false_type tag;
        __uninitialized_copy(first, last, result, tag);
        return result;
    }
    void _M_clear()
    {
        if (_M_start)
            free(_M_start);
    }
    vector& operator=(const vector& x);
};
template <class T> vector<T>& vector<T>::operator=(const vector<T>& x)
{
    if (&x != this) {
        const unsigned int xlen = x.size();
        if (xlen > capacity()) {
            pointer tmp = _M_allocate_and_copy(xlen, (const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        }
        else if (size() >= xlen) {
            __copy((const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0, _M_start, random_access_iterator_tag(), (int*)0);
        }
        else {
            __copy((const_pointer)x._M_start, (const_pointer)x._M_start + size(), _M_start, random_access_iterator_tag(), (int*)0);
            __false_type isPOD;
            __uninitialized_copy((const_pointer)x._M_start + size(), (const_pointer)x._M_finish + 0, _M_finish, isPOD);
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}
}
typedef stl402::vector<NThingMap::CThingMapCell> TCells;

static char gNodes[128];
static NThingMap::CMapwhoNode* Node(int i) { return (NThingMap::CMapwhoNode*)(gNodes + i); }
static void FillSource(TCells& v, unsigned int n, int base)
{
    v.Reserve(n, n);
    for (unsigned int i = 0; i < n; ++i)
        v._M_start[i].PFirstNode = Node(base + (int)i);
}
static bool SameAs(const TCells& v, unsigned int n, int base)
{
    if (v.size() != n)
        return false;
    for (unsigned int i = 0; i < n; ++i)
        if (v._M_start[i].PFirstNode != Node(base + (int)i))
            return false;
    return true;
}

int main()
{
    int failures = 0;
    TCells source, target;
    FillSource(source, 5, 10);
    target = source;                                        // empty target: reallocate
    if (!SameAs(target, 5, 10) || target.capacity() != 5 || target._M_start == source._M_start) { printf("  reallocate branch\n"); ++failures; }
    TCells small;
    FillSource(small, 3, 40);
    stl402::gUninitCopies = 0;
    target = small;                                         // fits in the live size: in-place assignment only
    if (!SameAs(target, 3, 40) || target.capacity() != 5 || stl402::gUninitCopies != 0) { printf("  in-place branch\n"); ++failures; }
    TCells four;
    FillSource(four, 4, 70);
    target = four;                                          // size 3 < 4 <= capacity 5: assign 3, construct 1
    if (!SameAs(target, 4, 70) || target.capacity() != 5 || stl402::gUninitCopies != 1) { printf("  tail-construct branch\n"); ++failures; }
    TCells& alias = target;
    target = alias;                                         // self-assignment
    if (!SameAs(target, 4, 70)) { printf("  self-assignment\n"); ++failures; }
    TCells empty;
    target = empty;
    if (target.size() != 0 || target.capacity() != 5) { printf("  assigning an empty vector\n"); ++failures; }
    if (failures == 0) {
        printf("0085dfd0_TEST PASS\n");
        return 0;
    }
    printf("0085dfd0_TEST FAIL\n");
    return 1;
}