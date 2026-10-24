// Behaviour test for 006f3260 (STLport vector<pair<C3DVector,bool> >::_M_insert_overflow): grow-and-insert one copy at
// the end and three copies in the middle; contents, size and capacity (old size + max(old size, count)) must match.
#include <cstdio>
#include <cstdlib>
#include <new>

#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
#pragma pack(pop)

struct __false_type { __false_type() {} };

#pragma pack(push, 1)
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
#pragma pack(pop)

template<class T> inline const T& tmax(const T& a, const T& b) { return a < b ? b : a; }
template<class T1, class T2> inline void _Construct(T1* pointer, const T2& value) { new (pointer) T1(value); }

template<class InputIter, class ForwardIter>
ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template<class ForwardIter, class Size, class T>
ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template<class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        if (_M_start)
            std::free(_M_start);
    }
    void _M_set(T* s, T* f, T* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + tmax(old_size, fill_len);
        T* new_start = len ? (T*)std::malloc(len * sizeof(T)) : 0;
        T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
        if (fill_len == 1) {
            _Construct(new_finish, x);
            ++new_finish;
        }
        else
            new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
        if (!atend)
            new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
        _M_clear();
        _M_set(new_start, new_finish, new_start + len);
    }
};

typedef pair<C3DVector, bool> SEntry;

int main()
{
    int ok = sizeof(SEntry) == 13;
    vector<SEntry> v;
    SEntry* block = (SEntry*)std::malloc(2 * sizeof(SEntry));
    new (block) SEntry(C3DVector(1, 2, 3), true);
    new (block + 1) SEntry(C3DVector(4, 5, 6), false);
    v._M_set(block, block + 2, block + 2);
    v._M_insert_overflow(v._M_finish, SEntry(C3DVector(7, 8, 9), true), __false_type(), 1, true);
    if (v.size() != 3 || v._M_end_of_storage - v._M_start != 4 || v._M_start[2].first.Z != 9 || !v._M_start[2].second ||
        v._M_start[0].first.X != 1 || v._M_start[1].second)
        ok = 0;
    v._M_insert_overflow(v._M_start + 1, SEntry(C3DVector(-1, -2, -3), false), __false_type(), 3, false);
    // expected: (1,2,3) x3 (-1,-2,-3) (4,5,6) (7,8,9); capacity 3 + max(3, 3) = 6
    if (v.size() != 6 || v._M_end_of_storage - v._M_start != 6)
        ok = 0;
    else {
        const float xs[6] = { 1, -1, -1, -1, 4, 7 };
        for (int i = 0; i < 6; ++i)
            if (v._M_start[i].first.X != xs[i])
                ok = 0;
        if (v._M_start[3].second || !v._M_start[5].second || v._M_start[4].first.Y != 5)
            ok = 0;
    }
    v._M_clear();
    if (!ok) {
        std::printf("006f3260_TEST FAIL\n");
        return 1;
    }
    std::printf("006f3260_TEST PASS\n");
    return 0;
}