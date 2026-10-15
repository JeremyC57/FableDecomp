// CPlayer::InitPlayerHeroLogBook() -- give the player a fresh, empty hero log book (no quests, no entries) held by its counted pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048A1A0, length 107 bytes.
// Callees: operator new @ 0x00BFEA1A (0x1c bytes), malloc @ 0x00BFEA0E (the 0x20-byte map header node),
// CCountedPointer<CHeroLogBook>::Reset(CHeroLogBook*) @ 0x0048D8B0 (manifest label propagated from another instantiation).
// Layout: engine/CPlayer.h PHeroLogBook (+0xF8). Ego_r.pdb CHeroLogBook with retail 12-byte STLport containers:
// GivenQuests (vector) +0x00, QuestBeingViewed +0x0c, LogEntries (map: header proxy +0x10, node count +0x14,
// comparator +0x18) = 0x1c bytes.
// Shape: the inline CHeroLogBook constructor is the STLport containers' own: _Vector_base zero words, the
// _Rb_tree_base header proxy built with 0 then assigned the malloc'd header, _M_node_count(0), and
// _M_empty_initialize() writing colour/parent/leftmost/rightmost through the re-read header pointer. The null `new`
// result takes its own `xor esi,esi` arm (VC duplicates the Reset call).
#include "engine/CPlayer.h"

extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

template <class T>
struct allocator {
    allocator() { }
};

template <class T, class A>
struct _STLP_alloc_proxy : A {                              // STLport: a pointer over an empty allocator
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};

template <class T>
struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};

template <class T>
struct vector : _Vector_base<T> {
    vector() : _Vector_base<T>(allocator<T>()) { }
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0 (red = false)
    _Rb_tree_node_base* _M_parent;                          // +4
    _Rb_tree_node_base* _M_left;                            // +8
    _Rb_tree_node_base* _M_right;                           // +0xc
};

template <class V>
struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};

template <class V>
struct _Rb_tree_base {
    _STLP_alloc_proxy<_Rb_tree_node<V>*, allocator<_Rb_tree_node<V> > > _M_header;
    _Rb_tree_base(const allocator<V>&) : _M_header(allocator<_Rb_tree_node<V> >(), (_Rb_tree_node<V>*)0)
    {
        _M_header._M_data = (_Rb_tree_node<V>*)malloc(sizeof(_Rb_tree_node<V>));
    }
};

template <class K>
struct less {
    less() { }
};

template <class K, class V>
struct _Rb_tree : _Rb_tree_base<V> {
    unsigned int _M_node_count;
    less<K>      _M_key_compare;
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // _S_rb_tree_red
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    _Rb_tree() : _Rb_tree_base<V>(allocator<V>()), _M_node_count(0), _M_key_compare() { _M_empty_initialize(); }
};

struct CQuestDesc;
struct SCategoryEntries {                                   // pair<const ECategory, vector<CHeroLogBookEntry> > (16 bytes)
    long          first;
    unsigned long second[3];
};

struct CHeroLogBookRetail {                                 // CHeroLogBook, 0x1c bytes in retail
    vector<CQuestDesc>               GivenQuests;           // +0x00
    long                             QuestBeingViewed;      // +0x0c
    _Rb_tree<long, SCategoryEntries> LogEntries;            // +0x10
    CHeroLogBookRetail() : QuestBeingViewed(0) { }
};

struct CHeroLogBookPointer {                                // CCountedPointer<CHeroLogBook> at +0xF8
    CHeroLogBookRetail* Data;
    void*               Info;
    void Reset(CHeroLogBookRetail* book);                   // 0x0048D8B0
};

struct CPlayer_Methods : CPlayer {
    void InitPlayerHeroLogBook();
};

void CPlayer_Methods::InitPlayerHeroLogBook()
{
    reinterpret_cast<CHeroLogBookPointer*>(&this->PHeroLogBook_Data)->Reset(new CHeroLogBookRetail);
}
