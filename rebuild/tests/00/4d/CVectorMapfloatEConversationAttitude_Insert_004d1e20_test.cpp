// Behaviour test for 0x004D1E20 CVectorMap<float, EConversationAttitude>::Insert(key, value): push_back {key, value}
// (in place while there is room, else reallocate to old size + max(old size, 1)) and mark the map dirty.
#include <stdio.h>
#ifndef _MSC_VER
#define __cdecl
#define __declspec(x)
#endif
#include <stddef.h>
#include <stdlib.h>

#include <new>

enum EConversationAttitude { ATTITUDE_NONE };

struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {     // STLport pair
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

template <class T> struct allocator { allocator() { } };

template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};

template <class T> struct vector {              // STLport vector (12 bytes in retail)
    T*                   _M_start;              // +0
    T*                   _M_finish;             // +4
    _STLP_alloc_proxy<T> _M_end_of_storage;     // +8
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_t fill_len, bool atend);   // 0x004D1250
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};

template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, size_t fill_len, bool atend)
{
    size_t old_size = _M_finish - _M_start;
    size_t len = old_size + (old_size > fill_len ? old_size : fill_len);
    T* new_start = (T*)malloc(len * sizeof(T));
    T* new_finish = new_start;
    for (T* p = _M_start; p != position; ++p, ++new_finish)
        _Construct(new_finish, *p);
    for (size_t i = 0; i < fill_len; ++i, ++new_finish)
        _Construct(new_finish, x);
    if (!atend)
        for (T* p = position; p != _M_finish; ++p, ++new_finish)
            _Construct(new_finish, *p);
    if (_M_start)
        free(_M_start);
    _M_start = new_start;
    _M_finish = new_finish;
    _M_end_of_storage._M_data = new_start + len;
}

template <class K, class V> struct CKeyPairCompareLess { };   // empty

template <class K, class V> struct CVectorMap {
    vector<pair<K, V> >       Vector;           // +0x00
    CKeyPairCompareLess<K, V> Compare;          // +0x0c
    bool                      Dirty;            // +0x0d
    void Insert(const K& key, const V& value)
    {
        Vector.push_back(pair<K, V>(key, value));
        Dirty = true;
    }
};




int main()
{
    CVectorMap<float, EConversationAttitude> m;
    m.Vector._M_start = m.Vector._M_finish = m.Vector._M_end_of_storage._M_data = 0; m.Dirty = false;
    int ok = 1;
    for (int k = 0; k < 5; ++k) {
        float key = 0.25f * k; EConversationAttitude att = (EConversationAttitude)(k + 1);
        m.Insert(key, att);
        if (!m.Dirty) ok = 0;
        m.Dirty = false;
    }
    if (m.Vector._M_finish - m.Vector._M_start != 5 || m.Vector._M_end_of_storage._M_data - m.Vector._M_start != 8) ok = 0;
    for (int k = 0; k < 5 && ok; ++k)
        if (m.Vector._M_start[k].first < 0.25f * k - 0.0001f || m.Vector._M_start[k].first > 0.25f * k + 0.0001f || m.Vector._M_start[k].second != (EConversationAttitude)(k + 1)) ok = 0;
    printf(ok ? "004d1e20_TEST PASS\n" : "004d1e20_TEST FAIL\n");
    return ok ? 0 : 1;
}