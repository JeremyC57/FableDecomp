// Behaviour test for 00bbdd40 (_STL::vector<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator>::resize(n)): growing appends through one fill insert at end(), shrinking erases the tail
// [begin() + n, end()), an equal size still issues an empty fill insert, and the surviving head is untouched.
#include <cstdio>
#include <new>
struct CListNode;
struct CPrimitiveIterator {
    bool      Valid;
    CListNode* Iterator;
    CPrimitiveIterator() : Valid(false) { }
};
static CListNode* NodeAt(int k) { static char nodes[16]; return (CListNode*)&nodes[k]; }
typedef CPrimitiveIterator T;
static int g_erase = 0;
static int g_fill = 0;
template <class _Tp> struct vector {
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector(_Tp* buf, unsigned int cap) : _M_start(buf), _M_finish(buf), _M_end_of_storage(buf + cap) { }
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)              // stands in for the retail out-of-line erase
    {
        ++g_erase;
        iterator __d = __first;
        for (iterator __s = __last; __s != this->_M_finish; ++__s, ++__d)
            *__d = *__s;
        this->_M_finish = __d;
        return __first;
    }
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x)   // stands in for the retail fill insert (always at end() here)
    {
        ++g_fill;
        if (__pos != this->_M_finish) return;
        for (; __n > 0; --__n) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
    }
    void insert(iterator __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
#define FAIL() { std::printf("00bbdd40_TEST FAIL\n"); return 1; }
int main()
{
    static T buf[8];
    vector<T> v(buf, 8);
    v.resize(5);                                       // grow from empty: one fill insert, no erase
    if (v.size() != 5 || g_fill != 1 || g_erase != 0) FAIL();
    for (int i = 0; i < 5; ++i) {
        if (!(!(buf[i]).Valid)) FAIL();
        ((buf[i]).Valid = true, (buf[i]).Iterator = NodeAt(i + 1));
    }
    v.resize(2);                                       // shrink: erase [begin()+2, end())
    if (v.size() != 2 || g_erase != 1 || g_fill != 1) FAIL();
    if ((int)((char*)(buf[0]).Iterator - (char*)NodeAt(0)) != 1 || (int)((char*)(buf[1]).Iterator - (char*)NodeAt(0)) != 2) FAIL();
    v.resize(2);                                       // same size: an empty fill insert at end()
    if (v.size() != 2 || g_erase != 1 || g_fill != 2) FAIL();
    v.resize(6);                                       // grow again: the tail gets the fill value, the head survives
    if (v.size() != 6 || g_fill != 3 || (int)((char*)(buf[0]).Iterator - (char*)NodeAt(0)) != 1 || (int)((char*)(buf[1]).Iterator - (char*)NodeAt(0)) != 2) FAIL();
    for (int i = 2; i < 6; ++i)
        if (!(!(buf[i]).Valid)) FAIL();
    std::printf("00bbdd40_TEST PASS\n");
    return 0;
}