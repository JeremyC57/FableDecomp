// Behaviour test for 0x009B3630 vector<unsigned char>::_M_range_insert (STLport forward-iterator range insert, POD).
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace stlp {
struct forward_iterator_tag { forward_iterator_tag() { } };

inline void* copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)std::memmove(result, first, ((const char*)last - (const char*)first))) +
        ((const char*)last - (const char*)first);
}
inline void* copy_trivial_backward(const void* first, const void* last, void* result)
{
    const int num = (const char*)last - (const char*)first;
    return (num > 0) ? std::memmove((char*)result - num, first, num) : result;
}
template <class T> inline const T& max_of(const T& a, const T& b) { return a < b ? b : a; }

template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    template <class FI> void _M_range_insert(T* pos, FI first, FI last, const forward_iterator_tag&)
    {
        if (first != last) {
            unsigned int n = 0;
            n += last - first;
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                const unsigned int elems_after = _M_finish - pos;
                T* old_finish = _M_finish;
                if (elems_after > n) {
                    copy_trivial(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    copy_trivial_backward(pos, old_finish - n, old_finish);
                    copy_trivial(first, last, pos);
                } else {
                    FI mid = first;
                    mid += elems_after;
                    copy_trivial(mid, last, _M_finish);
                    _M_finish += n - elems_after;
                    copy_trivial(pos, old_finish, _M_finish);
                    _M_finish += elems_after;
                    copy_trivial(first, mid, pos);
                }
            } else {
                const unsigned int old_size = size();
                const unsigned int len = old_size + max_of(old_size, n);
                T* new_start = len != 0 ? (T*)std::malloc(len * sizeof(T)) : 0;
                T* new_finish = (T*)copy_trivial(_M_start, pos, new_start);
                new_finish = (T*)copy_trivial(first, last, new_finish);
                new_finish = (T*)copy_trivial(pos, _M_finish, new_finish);
                if (_M_start) std::free(_M_start);
                _M_start = new_start;
                _M_finish = new_finish;
                _M_end_of_storage = new_start + len;
            }
        }
    }
};
}

static int Check(const stlp::vector<unsigned char>& v, const char* want, unsigned int cap)
{
    unsigned int n = (unsigned int)std::strlen(want);
    if (v.size() != n) return 0;
    if ((unsigned int)(v._M_end_of_storage - v._M_start) != cap) return 0;
    return std::memcmp(v._M_start, want, n) == 0;
}

static void Make(stlp::vector<unsigned char>& v, const char* init, unsigned int cap)
{
    unsigned int n = (unsigned int)std::strlen(init);
    v._M_start = (unsigned char*)std::malloc(cap);
    std::memcpy(v._M_start, init, n);
    v._M_finish = v._M_start + n;
    v._M_end_of_storage = v._M_start + cap;
}

int main()
{
    const unsigned char xy[] = { 'X', 'Y' };
    const unsigned char wxyz[] = { 'W', 'X', 'Y', 'Z' };
    int ok = 1;
    stlp::vector<unsigned char> v;
    // in place, elems_after (4) > n (2)
    Make(v, "abcde", 10);
    v._M_range_insert(v._M_start + 1, xy, xy + 2, stlp::forward_iterator_tag());
    ok &= Check(v, "aXYbcde", 10);
    std::free(v._M_start);
    // in place, elems_after (1) <= n (4)
    Make(v, "abcde", 10);
    v._M_range_insert(v._M_start + 4, wxyz, wxyz + 4, stlp::forward_iterator_tag());
    ok &= Check(v, "abcdWXYZe", 10);
    std::free(v._M_start);
    // reallocation: size 5 + max(5, 4) = 10
    Make(v, "abcde", 6);
    v._M_range_insert(v._M_start + 2, wxyz, wxyz + 4, stlp::forward_iterator_tag());
    ok &= Check(v, "abWXYZcde", 10);
    // reallocation at the front, n larger than size: 9 + max(9, 4)
    v._M_end_of_storage = v._M_finish;
    v._M_range_insert(v._M_start, wxyz, wxyz + 4, stlp::forward_iterator_tag());
    ok &= Check(v, "WXYZabWXYZcde", 18);
    // empty range: untouched
    v._M_range_insert(v._M_start, wxyz, wxyz, stlp::forward_iterator_tag());
    ok &= Check(v, "WXYZabWXYZcde", 18);
    std::free(v._M_start);
    if (!ok) { std::printf("009b3630_TEST FAIL\n"); return 1; }
    std::printf("009b3630_TEST PASS\n");
    return 0;
}