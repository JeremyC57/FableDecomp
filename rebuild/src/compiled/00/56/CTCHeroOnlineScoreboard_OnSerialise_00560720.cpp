// CTCHeroOnlineScoreboard::OnSerialise(CPersistContext& context, bool) -- persists the local scoreboard map as two parallel vectors ("ActivitiesVec", "ScoresVec"): saving copies the map out, loading copies the vectors back into the map
// Leaf: ?OnSerialise@CTCHeroOnlineScoreboard@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00560720, 506 bytes (0x1FA + int3 padding), virtual (vtable 0x0124DB34 slot 1),
//   thiscall `ret 8`.
// Identity: Ego_r.pdb ?OnSerialise@CTCHeroOnlineScoreboard@@UAEXAAVCPersistContext@@_N@Z; the RTTI vtable of
//   CTCHeroOnlineScoreboard holds 0x00560720 in slot 1. The catalog's "QuestData_PersistActivitiesAndScores" is
//   descriptive (and the landed Transfer rows 0x00569F20 / 0x00569FE0 inherited CQuestActivityRecord /
//   CQuestScoreRecord element names from it; the elements are the activity enum and float).
// Callees: CPersistContext::Transfer<EOnlineScoreboardActivity>(name, vector&) 0x00569F20,
//   CPersistContext::Transfer<float>(name, vector&) 0x00569FE0, vector<EOnlineScoreboardActivity>::_M_insert_overflow
//   0x00568150, vector<float>::_M_insert_overflow 0x004408A0, map<EOnlineScoreboardActivity, float>::operator[]
//   0x005692B0, free 0x00BFEA14; "ActivitiesVec" 0x0124C798, "ScoresVec" 0x0124C78C.
// Layout: retail CTCHeroOnlineScoreboard offsets (12-byte STLport containers): PHttp +0x0c, PictureCache (map) +0x10,
//   LocalScoreboard (map<EOnlineScoreboardActivity, float>) +0x1c. CPersistContext Mode +0x18 (saving = 1 or 3).
// Shape: each branch owns its two vectors. Saving walks the map (STLport `_M_increment` inline) copying key and
//   score into named locals and push_back'ing them (the POD `_M_insert_overflow` tag shares the dead context slot
//   with the score: its same-TU stand-in body never touches the tag), then transfers both vectors; loading transfers
//   both vectors and walks them in step with two iterators, `LocalScoreboard[key] = value`. The named locals are
//   declared score/value first: VC7.1 then loads and stores them in retail's order (saving: score, then key;
//   loading: key, then value, the value stored to its frame word after the subscript's argument push).
extern "C" void* __cdecl malloc(unsigned int size);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);    // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __true_type { __true_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
          ((const char*)__last - (const char*)__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef typename _Vector_base<_Tp, _Alloc>::allocator_type allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);
    public:
};

template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x,
                                                                  const __true_type&, size_type __fill_len,
                                                                  bool __atend)
{                                                   // 0x00568150 / 0x004408A0 (same-TU stand-in body)
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
    for (size_type __i = 0; __i < __fill_len; ++__i, ++__new_finish)
        _Construct(__new_finish, __x);
    if (!__atend)
        __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                                                // +0x00
    _Base_ptr _M_parent;                                                               // +0x04
    _Base_ptr _M_left;                                                                 // +0x08
    _Base_ptr _M_right;                                                                // +0x0c
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                                             // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr _M_increment(_Base_ptr _M_node);
};

template <class _Dummy>
_Rb_tree_node_base* _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Value> struct _Rb_tree_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Base_ptr __x) { _M_node = __x; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Key, class _Tp> class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                                                     // +0x00
    unsigned int        _M_node_count;                                                 // +0x04
    unsigned char       _M_key_compare;                                                // +0x08
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    _Tp& operator[](const _Key& __k);                                                  // 0x005692B0
};

}

class CPersistContext {
public:
    void*         PDefinitionManager;                                                  // +0x00
    void*         PSymbolMap;                                                          // +0x04
    void*         PParser;                                                             // +0x08
    void*         ErrorCallback;                                                       // +0x0c
    void*         PStringToClean;                                                      // +0x10
    bool          UseDefaults;                                                         // +0x14
    bool          UseCRCCheck;                                                         // +0x15
    bool          SingleTransfer;                                                      // +0x16
    bool          Finished;                                                            // +0x17
    long          Mode;                                                                // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    template <class T> void Transfer(const char* name, std::vector<T>& value);      // 0x00569F20 / 0x00569FE0
};

class CTCHeroOnlineScoreboard {
public:
    enum EOnlineScoreboardActivity { ONLINE_SCOREBOARD_ACTIVITY_0 };
    virtual void OnSerialise(CPersistContext& context, bool);
    void*                                          ParentThing;                       // +0x04
    long                                           m_8;                               // +0x08
    void*                                          PHttp;                             // +0x0c (scoped_ptr<LHHttp2>)
    unsigned char                                  PictureCache[0x0c];                // +0x10 (map<long, CCountedPointer<CTexture> >)
    std::map<EOnlineScoreboardActivity, float>    LocalScoreboard;                   // +0x1c
};

void CTCHeroOnlineScoreboard::OnSerialise(CPersistContext& context, bool)
{
    if (context.IsSaving()) {
        std::vector<EOnlineScoreboardActivity> activities;
        std::vector<float> scores;
        for (std::map<EOnlineScoreboardActivity, float>::iterator it = LocalScoreboard.begin();
             it != LocalScoreboard.end(); ++it) {
            float score = (*it).second;
            EOnlineScoreboardActivity activity = (*it).first;
            activities.push_back(activity);
            scores.push_back(score);
        }
        context.Transfer("ActivitiesVec", activities);
        context.Transfer("ScoresVec", scores);
    }
    else {
        std::vector<EOnlineScoreboardActivity> activities;
        std::vector<float> scores;
        context.Transfer("ActivitiesVec", activities);
        context.Transfer("ScoresVec", scores);
        std::vector<float>::iterator score = scores.begin();
        for (std::vector<EOnlineScoreboardActivity>::iterator activity = activities.begin();
             activity != activities.end(); ++activity, ++score) {
            float value = *score;
            EOnlineScoreboardActivity key = *activity;
            LocalScoreboard[key] = value;
        }
    }
}