// CTCWeaponTrail::GetLastWeaponTrailPoints(std::vector<std::pair<C3DVector,C3DVector> >& points, const std::list<CWeaponTrailEntry>& trail) const -- append the top/bottom pairs of the newest (interpolation count + 1) entries of a weapon trail, oldest first; false when the trail has fewer than two entries
// Grade: DIFFER(253v253) -- scheduling only: the push_back's in-place copy of the 24-byte pair temp; retail pipelines it (two loads ahead, rotating ecx/edx), VC7.1 here copies member by member (load/store pairs) because C3DVector's user copy ctor keeps the temp and the new slot aliased; every other byte matches
// Leaf: ?GetLastWeaponTrailPoints@CTCWeaponTrail@@ABE
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076E4D0, length 256 bytes (thiscall, `ret 8`; `this` is never read).
// Identity: Ego_r.pdb ?GetLastWeaponTrailPoints@CTCWeaponTrail@@ABE_NAAV?$vector@U?$pair@VC3DVector@@V1@@std@@...@std@@
//   ABV?$list@VCWeaponTrailEntry@CEnginePrimitiveWeaponTrail@@...@3@@Z (the private overload). Its two callers are the
//   public wrappers: 0x0076E5D0 GetLastProjectileWeaponTrailPoints (passes this+0x60, ProjectileWeaponTrail) and
//   0x0076E5F0 GetLastWeaponTrailPoints(points, long trail) (bounds-checks against NoTrails +0x0C, Trail at +0x48).
//   The catalog label "WeaponTrail_CollectPoints" is descriptive.
// Callees: vector<pair<C3DVector,C3DVector> >::_M_insert_overflow 0x007700D0 (push_back's growth path, fill 1 at end).
// Globals: 0x013828A0, a long the weapon-trail TU reads as its interpolation step count (descriptive name).
// Layout: CWeaponTrailEntry (Ego_r.pdb, 28 bytes) {C3DVector Top +0; C3DVector Bottom +0x0C; float CreationTime +0x18};
//   STLport list node {next +0, prev +4, data +8}; the output pair is 24 bytes.
// Shape: `long count = trail.size(); if (count <= 1) return false; it = trail.end(); advance(it, -(count < g + 1 ? count : g + 1));
//   for (; it != trail.end(); ++it) points.push_back(pair(it->Top, it->Bottom)); return true;` -- size() is the
//   STLport distance walk, advance is the bidirectional two-loop form.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

extern long g_WeaponTrailInterpolationSteps;            // 0x013828A0

inline void* operator new(unsigned int, void* p) { return p; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry {                           // Ego_r.pdb, 28 bytes
    public:
        C3DVector Top;                                  // +0x00
        C3DVector Bottom;                               // +0x0C
        float     CreationTime;                         // +0x18
    };
};

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                  const __false_type&);                                       // 0x0076F780
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);   // 0x0076F7D0

template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
};

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    typename _Traits::pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_incr(); return *this; }
    _Self& operator--() { _M_decr(); return *this; }
    bool operator==(const _Self& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Traits>
inline bidirectional_iterator_tag __ITERATOR_CATEGORY(const _List_iterator<_Tp, _Traits>&)
{
    return bidirectional_iterator_tag();
}

template <class _InputIterator, class _Distance>
inline void __distance(const _InputIterator& __first, const _InputIterator& __last, _Distance& __n,
                       const input_iterator_tag&)
{
    _InputIterator __it(__first);
    while (__it != __last) { ++__it; ++__n; }
}

template <class _InputIterator, class _Distance>
inline void distance(const _InputIterator& __first, const _InputIterator& __last, _Distance& __n)
{
    __distance(__first, __last, __n, __ITERATOR_CATEGORY(__first));
}

template <class _BidirectionalIterator, class _Distance>
inline void __advance(_BidirectionalIterator& __i, _Distance __n, const bidirectional_iterator_tag&)
{
    if (__n > 0)
        while (__n--) ++__i;
    else
        while (__n++) --__i;
}

template <class _InputIterator, class _Distance>
inline void advance(_InputIterator& __i, _Distance __n)
{
    __advance(__i, __n, __ITERATOR_CATEGORY(__i));
}

template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    const_iterator begin() const { return const_iterator((_Node*)this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    size_type size() const
    {
        size_type __result = 0;
        distance(begin(), end(), __result);
        return __result;
    }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear() { this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);      // 0x007700D0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x,
                                                                  const __false_type&, size_type __fill_len,
                                                                  bool __atend)
{                                                       // 0x007700D0 (same-TU stand-in; not graded)
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
#pragma auto_inline(on)

}

class CTCWeaponTrail {
public:
    typedef CEnginePrimitiveWeaponTrail::CWeaponTrailEntry CWeaponTrailEntry;
private:
    bool GetLastWeaponTrailPoints(std::vector<std::pair<C3DVector, C3DVector> >& points,
                                  const std::list<CWeaponTrailEntry>& trail) const;
};

bool CTCWeaponTrail::GetLastWeaponTrailPoints(std::vector<std::pair<C3DVector, C3DVector> >& points,
                                              const std::list<CWeaponTrailEntry>& trail) const
{
    long count = trail.size();
    if (count > 1) {
        long steps = g_WeaponTrailInterpolationSteps + 1;
        long n = count < steps ? count : steps;
        std::list<CWeaponTrailEntry>::const_iterator it = trail.end();
        std::advance(it, -n);
        for (; it != trail.end(); ++it)
            points.push_back(std::pair<C3DVector, C3DVector>(it->Top, it->Bottom));
        return true;
    }
    return false;
}
