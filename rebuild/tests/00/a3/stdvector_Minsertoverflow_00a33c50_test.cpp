#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;                         // live element objects (copy ctor ++, dtor --)

class C3DGeneratorObject { public:
    float Transform[12];
    int BoneIndex;
    int ObjectName;
    unsigned int BankIndex;
    bool UseLocalOrigin;
    C3DGeneratorObject(int k) : BoneIndex(k), ObjectName(k * 100), BankIndex((unsigned int)k + 1), UseLocalOrigin((k & 1) != 0) { for (int i = 0; i < 12; ++i) Transform[i] = (float)(k + i); ++g_live; }
    C3DGeneratorObject(const C3DGeneratorObject& o) : BoneIndex(o.BoneIndex), ObjectName(o.ObjectName), BankIndex(o.BankIndex), UseLocalOrigin(o.UseLocalOrigin) { for (int i = 0; i < 12; ++i) Transform[i] = o.Transform[i]; ++g_live; }
    C3DGeneratorObject& operator=(const C3DGeneratorObject& o) { for (int i = 0; i < 12; ++i) Transform[i] = o.Transform[i]; BoneIndex = o.BoneIndex; ObjectName = o.ObjectName; BankIndex = o.BankIndex; UseLocalOrigin = o.UseLocalOrigin; return *this; }
    ~C3DGeneratorObject() { --g_live; }
};
static C3DGeneratorObject MakeElem(int k) { return C3DGeneratorObject(k); }
static int KeyOf(const C3DGeneratorObject& e) { return e.BoneIndex; }
static bool CheckElem(const C3DGeneratorObject& e, int k) { return e.ObjectName == k * 100 && e.BankIndex == (unsigned int)k + 1 && e.UseLocalOrigin == ((k & 1) != 0) && e.Transform[5] == (float)(k + 5); }

// STLport 4.6 vector<T> growth / insert paths, as the source reconstructs them (element by element, __false_type).
template <class T> struct TVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    TVector(unsigned int cap) : _M_start((T*)malloc(cap * sizeof(T))), _M_finish(_M_start), _M_end_of_storage(_M_start + cap) { }
    ~TVector() { _M_clear(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear() { for (T* p = _M_start; p != _M_finish; ++p) p->~T(); free(_M_start); _M_start = _M_finish = _M_end_of_storage = 0; }
    static T* __uninitialized_copy(const T* first, const T* last, T* result)
    { for (; first != last; ++first, ++result) new (result) T(*first); return result; }
    static T* __uninitialized_fill_n(T* first, unsigned int n, const T& x)
    { for (; n > 0; --n, ++first) new (first) T(x); return first; }
    static T* __copy_backward(T* first, T* last, T* result)
    { while (first != last) *--result = *--last; return result; }
    static void fill(T* first, T* last, const T& x) { for (; first != last; ++first) *first = x; }
    static T* copy(const T* first, const T* last, T* result) { for (; first != last; ++first, ++result) *result = *first; return result; }
    void _M_insert_overflow(T* position, const T& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size > fill_len ? old_size : fill_len);
        T* new_start = (T*)malloc(len * sizeof(T));
        T* new_finish = __uninitialized_copy(_M_start, position, new_start);
        if (fill_len == 1) {
            new (new_finish) T(x);
            ++new_finish;
        }
        else
            new_finish = __uninitialized_fill_n(new_finish, fill_len, x);
        if (!atend)
            new_finish = __uninitialized_copy(position, _M_finish, new_finish);
        _M_clear();
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
    void _M_fill_insert(T* position, unsigned int n, const T& x)
    {
        if (n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                T x_copy = x;
                const unsigned int elems_after = (unsigned int)(_M_finish - position);
                T* old_finish = _M_finish;
                if (elems_after > n) {
                    __uninitialized_copy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    __copy_backward(position, old_finish - n, old_finish);
                    fill(position, position + n, x_copy);
                }
                else {
                    __uninitialized_fill_n(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    __uninitialized_copy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    fill(position, old_finish, x_copy);
                }
            }
            else
                _M_insert_overflow(position, x, n, false);
        }
    }
    void _M_range_insert(T* position, const T* first, const T* last)
    {
        if (first != last) {
            unsigned int n = (unsigned int)(last - first);
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                const unsigned int elems_after = (unsigned int)(_M_finish - position);
                T* old_finish = _M_finish;
                if (elems_after > n) {
                    __uninitialized_copy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    __copy_backward(position, old_finish - n, old_finish);
                    copy(first, last, position);
                }
                else {
                    const T* mid = first + elems_after;
                    __uninitialized_copy(mid, last, _M_finish);
                    _M_finish += n - elems_after;
                    __uninitialized_copy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    copy(first, mid, position);
                }
            }
            else {
                const unsigned int old_size = size();
                const unsigned int len = old_size + (old_size > n ? old_size : n);
                T* new_start = (T*)malloc(len * sizeof(T));
                T* new_finish = __uninitialized_copy(_M_start, position, new_start);
                new_finish = __uninitialized_copy(first, last, new_finish);
                new_finish = __uninitialized_copy(position, _M_finish, new_finish);
                _M_clear();
                _M_start = new_start;
                _M_finish = new_finish;
                _M_end_of_storage = new_start + len;
            }
        }
    }
};

typedef C3DGeneratorObject TElem;

static void Fill(TVector<TElem>& v, int count)
{
    for (int i = 0; i < count; ++i) {
        new (v._M_finish) TElem(MakeElem(i + 1));
        ++v._M_finish;
    }
}

static bool Same(const TVector<TElem>& v, const int* keys, int count)
{
    if ((int)v.size() != count || g_live != count)
        return false;
    for (int i = 0; i < count; ++i)
        if (KeyOf(v._M_start[i]) != keys[i])
            return false;
    return true;
}

static int Fail()
{
    std::printf("00a33c50_TEST FAIL\n");
    return 1;
}

int main()
{
    {   // push_back overflow: append one copy at the end, storage doubles
        TVector<TElem> v(3);
        Fill(v, 3);
        TElem x = MakeElem(9);
        v._M_insert_overflow(v._M_finish, x, 1, true);
        const int k[] = { 1, 2, 3, 9 };
        g_live -= 1;
        if (!Same(v, k, 4) || v._M_end_of_storage - v._M_start != 6) return Fail();
        g_live += 1;
        if (!CheckElem(v._M_start[3], 9)) return Fail();
    }
    if (g_live != 0) return Fail();
    {   // insert of three copies in the middle (fill_len > old size)
        TVector<TElem> v(2);
        Fill(v, 2);
        TElem x = MakeElem(7);
        v._M_insert_overflow(v._M_start + 1, x, 3, false);
        const int k[] = { 1, 7, 7, 7, 2 };
        g_live -= 1;
        if (!Same(v, k, 5) || v._M_end_of_storage - v._M_start != 5) return Fail();
        g_live += 1;
    }
    if (g_live != 0) return Fail();
    std::printf("00a33c50_TEST PASS\n");
    return 0;
}