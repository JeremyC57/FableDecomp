// Behaviour test for vector<CScriptGameResourceObjectScriptedThingBase>::_M_fill_insert (0x00CD378D): insert n
// copies of x before position -- in place when capacity allows (both the "tail longer than n" and the "tail not
// longer than n" paths), otherwise through the overflow insert; n == 0 does nothing.
#include <cstdio>
#include <new>

struct CThingBase {
    int value;
    CThingBase(int v) : value(v) { }
    CThingBase(const CThingBase& o) : value(o.value) { }
    CThingBase& operator=(const CThingBase& o) { value = o.value; return *this; }
};
template <class T> void Construct(T* p, const T& x) { new (p) T(x); }
template <class T> T* UninitializedCopy(T* first, T* last, T* result)
{
    T* cur = result;
    for (; first != last; ++first, ++cur) Construct(cur, *first);
    return cur;
}
template <class T> T* UninitializedFillN(T* first, unsigned int n, const T& x)
{
    T* cur = first;
    for (; n > 0; --n, ++cur) Construct(cur, x);
    return cur;
}
template <class T> T* CopyBackward(T* first, T* last, T* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
template <class T> void Fill(T* first, T* last, const T& val)
{
    for (; first != last; ++first) *first = val;
}
struct CThingVector {
    CThingBase* _M_start;
    CThingBase* _M_finish;
    CThingBase* _M_end_of_storage;
    int overflows;
    void _M_insert_overflow(CThingBase*, const CThingBase&, unsigned int) { ++overflows; }
    void _M_fill_insert(CThingBase* position, unsigned int n, const CThingBase& x)
    {
        if (n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                CThingBase x_copy = x;
                const unsigned int elems_after = (unsigned int)(_M_finish - position);
                CThingBase* old_finish = _M_finish;
                if (elems_after > n) {
                    UninitializedCopy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    CopyBackward(position, old_finish - n, old_finish);
                    Fill(position, position + n, x_copy);
                } else {
                    UninitializedFillN(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    UninitializedCopy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    Fill(position, old_finish, x_copy);
                }
            } else
                _M_insert_overflow(position, x, n);
        }
    }
};
static void Setup(CThingVector& v, unsigned char* storage, int count)
{
    v._M_start = (CThingBase*)storage;
    for (int i = 0; i < count; ++i) new (v._M_start + i) CThingBase(i + 1);
    v._M_finish = v._M_start + count;
    v._M_end_of_storage = v._M_start + 8;
    v.overflows = 0;
}
static bool Equals(const CThingVector& v, const int* expect, int count)
{
    if (v._M_finish - v._M_start != count) return false;
    for (int i = 0; i < count; ++i) if (v._M_start[i].value != expect[i]) return false;
    return true;
}

int main()
{
    static double storage1[16], storage2[16], storage3[16];
    CThingVector a; Setup(a, (unsigned char*)storage1, 4);    // 1 2 3 4
    a._M_fill_insert(a._M_start + 1, 2, CThingBase(9));        // tail 3 > 2
    const int ea[] = { 1, 9, 9, 2, 3, 4 };
    CThingVector b; Setup(b, (unsigned char*)storage2, 4);
    b._M_fill_insert(b._M_start + 3, 3, CThingBase(7));        // tail 1 <= 3
    const int eb[] = { 1, 2, 3, 7, 7, 7, 4 };
    CThingVector c; Setup(c, (unsigned char*)storage3, 7);
    c._M_fill_insert(c._M_start, 2, CThingBase(5));            // 1 slot left: overflow
    c._M_fill_insert(c._M_start, 0, CThingBase(5));            // nothing
    if (!Equals(a, ea, 6) || !Equals(b, eb, 7) || c.overflows != 1 || c._M_finish - c._M_start != 7 || a.overflows || b.overflows) {
        std::printf("00cd378d_TEST FAIL\n");
        return 1;
    }
    std::printf("00cd378d_TEST PASS\n");
    return 0;
}