// C3DAnimationStats::GetAllEventNames(std::vector<CCharString>& names) const -- reserve one slot per timing event and append every event's name (the TimingEvents multimap keys) through a back inserter
// Leaf: ?GetAllEventNames@C3DAnimationStats@@QBEXAAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4EF90, 42 bytes, `ret 4`. Name: Ego_r.pdb / FableWin.pdb
//   ?GetAllEventNames@C3DAnimationStats@@QBEXAAV?$vector@VCCharString@@...; the landed caller
//   NAnimationEvents::CManager::GetEmbeddedEventsBetweenTimes 0x006F9820 (and 0x006F9240) passes a C3DAnimationStats and
//   a vector<CCharString>. The catalog label "CGraphicDataBank::ChangeScene" is propagated.
// Layout: Ego_r.pdb C3DAnimationStats TimingEvents multimap<CCharString, float> at +0x20 (STLport header +0x20, node count +0x24).
// Callees: vector<CCharString>::reserve 0x004BA7A0 (landed), the key copy 0x00A557E0 (landed as STLport `copy`; __fastcall:
//   hidden back_insert_iterator result in ecx -- here the dead `names` slot -- first, last and the inserter on the stack).
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
template <class _InputIter, class _OutputIter>
_OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result);   // 0x00A557E0
}

void C3DAnimationStats::GetAllEventNames(std::vector<CCharString>& names) const
{
    names.reserve(TimingEvents.size());
    std::copy(TimingEvents.begin(), TimingEvents.end(), std::back_inserter(names));
}