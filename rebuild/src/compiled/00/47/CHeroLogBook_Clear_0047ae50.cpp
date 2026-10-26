// CHeroLogBook::Clear() -- empty the hero log book: clear the given-quest vector and the per-category log-entry map, reset the viewed quest
// Leaf: ?Clear@CHeroLogBook@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047AE50, 96 bytes, thiscall, plain `ret`.
// Identity: the manifest's "std::vector::erase_with_tree_cleanup" is descriptive. Ego_r.pdb ?Clear@CHeroLogBook@@QAEXXZ;
//   CHeroLogBook = { vector<CQuestDesc> GivenQuests +0; long QuestBeingViewed +0x0C (PDB +0x10 with 16-byte Dinkumware
//   vectors); map<CHeroLogBookEntry::ECategory, vector<CHeroLogBookEntry> > LogEntries +0x10 (header +0x10, count
//   +0x14) } -- the bytes touch exactly these three. The only caller 0x00487559 loads CPlayer +0xF8 (PDB
//   CCountedPointer<CHeroLogBook> PHeroLogBook) and then tail-jumps CPlayer::ResetModes 0x00634870. CQuestDesc is the
//   24-byte { ulong ScriptID; CCharString TextDescription; EQuestStatus QuestStatus; vector<CQuestObjectiveDesc> } that
//   the landed __copy 0x0047C600 assigns and the landed __destroy_aux 0x0047C800 destroys.
// Callees: __copy(first, last, result, const random_access_iterator_tag&, int*) 0x0047C600 (`__fastcall`, `ret 0Ch`),
//   __destroy_aux(first, last, const __false_type&) 0x0047C800 (`__fastcall`, `ret 4`), _Rb_tree::_M_erase(root)
//   0x0047C6A0 (thiscall on the tree).
// Shape: STLport 4.6 `vector::clear() { erase(begin(), end()); }` with erase inline (`__copy_ptrs(last, finish, first,
//   __false_type())` -> the out-of-line random-access __copy with `(int*)0`, then `_Destroy(i, finish)` -> the out-of-line
//   __destroy_aux, finish = i), then `_Rb_tree::clear()` (`if (count) { _M_erase(root); leftmost = header; root = 0;
//   rightmost = header; count = 0; }`, the header re-read for each store), then QuestBeingViewed = 0. Both empty tags
//   share the high byte of the `push ecx` frame word; ebx holds the zero for the distance pointer and all the stores.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                           // +0x00
};

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);              // 0x0047C600
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x0047C800

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;

    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08

    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }

private:
    void _M_erase(_Link_type __x);                          // 0x0047C6A0
public:
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};

template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare> _M_t;
    void clear() { _M_t.clear(); }
};

} // namespace std

class CHeroLogBook {
public:
    enum EQuestStatus { };
    class CQuestObjectiveDesc {                             // 12 bytes in retail
    public:
        unsigned long SubObjectiveID;                       // +0x00
        CCharString   TextDescription;                      // +0x04
        bool          QuestObjectiveCompleted;              // +0x08
    };
    class CQuestDesc {                                      // 24 bytes in retail
    public:
        unsigned long                          ScriptID;             // +0x00
        CCharString                            TextDescription;      // +0x04
        EQuestStatus                           QuestStatus;          // +0x08
        std::vector<CQuestObjectiveDesc>      QuestSubObjectives;   // +0x0C
    };
    class CHeroLogBookEntry {                               // 12 bytes: Name, AbbreviatedName, Content (CWideString)
    public:
        enum ECategory { };
        void* Name;
        void* AbbreviatedName;
        void* Content;
    };

    std::vector<CQuestDesc> GivenQuests;                   // +0x00
    long                     QuestBeingViewed;              // +0x0C
    std::map<CHeroLogBookEntry::ECategory, std::vector<CHeroLogBookEntry> > LogEntries;   // +0x10

    void Clear();
};

void CHeroLogBook::Clear()
{
    GivenQuests.clear();
    LogEntries.clear();
    QuestBeingViewed = 0;
}