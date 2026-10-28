// C3DAnimationStats::GetAllEvents(std::vector<std::pair<CCharString, float> >& events) const -- reserve one slot per timing event and append every (name, time) entry of the TimingEvents multimap through a back inserter
// Grade: DIFFER(50v48) -- the inline copy's back_insert_iterator result gets its own frame slot (push ecx; lea ecx,[esp+18h]) where retail homes the result AND the category tag in the dead `events` argument slot (lea edx/ecx,[esp+1Ch]); tried a bare tag temporary, a by-value __iterator_category, a named inserter, no __copy_aux hop
// Leaf: ?GetAllEvents@C3DAnimationStats@@QBEXAAV?$vector@U?$pair@VCCharString@@M@std@@V?$allocator@U?$pair@VCCharString@@M@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A50BF0, 48 bytes, `ret 4`. Name: Ego_r.pdb / FableWin.pdb
//   ?GetAllEvents@C3DAnimationStats@@QBEXAAV?$vector@U?$pair@VCCharString@@M@std@@...; sole caller 0x006F9203 (the
//   landed NAnimationEvents event-name query 0x006F91E0 family). The catalog label "CGraphicDataBank::ChangeScene" is propagated.
// Layout: Ego_r.pdb C3DAnimationStats TimingEvents multimap<CCharString, float> at +0x20 (STLport header +0x20, node count +0x24).
// Callees: vector<pair<CCharString, float> >::reserve 0x00A55500 (landed), STLport `__copy(first, last, result,
//   const input_iterator_tag&, int*)` 0x00A55880 (landed; __fastcall: hidden result in ecx, the tag reference in edx --
//   both the dead `events` slot -- first, last, the inserter and the null distance pointer on the stack).
// Shape: STLport `copy` and `__copy_aux` inline (std::copy -> __copy_aux(__false_type) -> __copy(_STLP_ITERATOR_CATEGORY,
//   _STLP_DISTANCE_TYPE)); the category comes back BY VALUE from the inline __iterator_category, which puts the tag and the
//   result in the same dead argument slot (a bare tag temporary gets a slot of its own).
class CCharString {
public:
    void* PStringData;                                  // +0x00
};
class C3DVector { public: float X, Y, Z; };
enum EAnimationType { ANIMATION_TYPE_DUMMY };

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct _Const_traits {
    typedef const _Tp& reference;
};
struct _Rb_tree_node_base {
    char                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Value, class _Traits> struct _Rb_tree_iterator {   // STLport 4.6: implicit copy constructor
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Rb_tree_node<_Tp>, _Alloc> _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;
    unsigned int _M_node_count;                         // +0x04
    _Compare     _M_key_compare;                        // +0x08
    const_iterator begin() const { return const_iterator(this->_M_header._M_data->_M_left); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
    unsigned int size() const { return _M_node_count; }
};
template <class _Pair> struct _Select1st { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class multimap {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    const_iterator begin() const { return _M_t.begin(); }
    const_iterator end() const { return _M_t.end(); }
    unsigned int size() const { return _M_t.size(); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp value_type;
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    void reserve(unsigned int __n);
};
template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
};
template <class _Container> inline back_insert_iterator<_Container> back_inserter(_Container& __x)
{
    return back_insert_iterator<_Container>(__x);
}
}   // namespace std

class C3DAnimationStats {                               // Ego_r.pdb, 0x30 bytes
public:
    virtual ~C3DAnimationStats();
    unsigned long  _pad04;                              // +0x04
    EAnimationType AnimType;                            // +0x08
    float          Duration;                            // +0x0C
    bool           LoopFlag;                            // +0x10
    bool           MovementFlag;                        // +0x11
    C3DVector      MovementVector;                      // PDB +0x12 (aligned here to +0x14; TimingEvents stays +0x20)
    std::multimap<CCharString, float> TimingEvents;     // +0x20
    void GetAllEventNames(std::vector<CCharString>& names) const;
    void GetAllEvents(std::vector<std::pair<CCharString, float> >& events) const;
};

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
template <class _InputIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_InputIter __first, _InputIter __last, _OutputIter __result, const input_iterator_tag&,
                              _Distance*);       // 0x00A55880
template <class _Iter> inline bidirectional_iterator_tag __iterator_category(const _Iter&, const __false_type&)
{
    return bidirectional_iterator_tag();           // iterator_traits<_Rb_tree_iterator>::iterator_category
}
template <class _Iter> inline int* __distance_type(const _Iter&) { return (int*)0; }
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, __iterator_category(__first, __false_type()), __distance_type(__first));
}
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_aux(__first, __last, __result, __false_type());
}
}

void C3DAnimationStats::GetAllEvents(std::vector<std::pair<CCharString, float> >& events) const
{
    events.reserve(TimingEvents.size());
    std::copy(TimingEvents.begin(), TimingEvents.end(), std::back_inserter(events));
}
