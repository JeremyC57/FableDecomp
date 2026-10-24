// _global::CopyFlaggedThings(std::list<CThing*>::iterator first, std::list<CThing*>::iterator last, std::back_insert_iterator<std::vector<CThing*> > result) -- append every listed thing whose +0xD4 flag is set to the vector, return the output iterator
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048D5C0, 122 bytes, /Gr `__fastcall` with the hidden result pointer in ecx and
// first, last, result on the stack (`ret 0Ch`).
// Callers: CPlayer::LocateCharacterStartHolySite @ 0x00488B20 (landed: "the /Gr template copy 0x0048D5C0 ... copies the
// list's things whose +0xD4 flag is set onto back_inserter(sites)", the list at +0x8C of ThingManager+4).
// Callees: vector<CThing*>::_M_insert_overflow(pos, x, const __true_type&, n, atend) @ 0x0048D0D0 (POD form; also
// called from 0x0048D3EA / 0x0048D518 / 0x004ADA98 / 0x004ADD06).
// Shape: a filtering copy loop: `for (; first != last; ++first) { CThing* thing = *first; if (thing->flag)
// *result++ = thing; }` with back_insert_iterator::operator= = STLport push_back inlined (placement store under its null
// test, `add [esi+4],4`, else _M_insert_overflow(finish, x, __true_type(), 1, true)). `thing` lives in the dead result
// argument slot; the empty tag is homed over it because _M_insert_overflow is defined in this TU (retail's copy sits
// beside it) and kept out of line (`__declspec(noinline)`) -- VC7.1 then knows the tag is never read. Speed TU (aligned loop head).
// Name: the algorithm is not an STLport one (no predicate argument); named by behaviour. The +0xD4 bool is the
// listed holy-site thing's flag (unnamed: the PDB CThing is 0xA8 bytes, so it belongs to a derived class).
// Label correction: catalog "LocationList_InsertActiveCharacters" is a guess.
// Leaf: CopyFlaggedThings
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6

class CThing {
public:
    unsigned char _pad00[0xd4];
    bool          FlagD4;                                   // +0xd4 (the holy site's selection flag)
};

namespace std {
struct __true_type { __true_type() { } };

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    __declspec(noinline) void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&,
                                                 unsigned int __fill_len, bool __atend = false);   // 0x0048D0D0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};

// 0x0048D0D0 (POD insert overflow), defined in this TU and kept out of line (`noinline`: the speed TU otherwise
// inlines the constant-argument call).
template <class _Tp>
void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, unsigned int __fill_len,
                                     bool __atend)
{
    const unsigned int __old_size = (unsigned int)(this->_M_finish - this->_M_start);
    const unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    pointer __new_start = __len != 0 ? (pointer)malloc(__len * sizeof(_Tp)) : 0;
    pointer __new_finish = __new_start;
    unsigned int __before = (unsigned int)((char*)__position - (char*)this->_M_start);
    if (__before != 0) {
        memmove(__new_start, this->_M_start, __before);
        __new_finish = (pointer)((char*)__new_start + __before);
    }
    for (unsigned int __n = __fill_len; __n > 0; --__n)
        *__new_finish++ = __x;
    if (!__atend) {
        unsigned int __after = (unsigned int)((char*)this->_M_finish - (char*)__position);
        memmove(__new_finish, __position, __after);
        __new_finish = (pointer)((char*)__new_finish + __after);
    }
    if (this->_M_start != 0)
        free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage = __new_start + __len;
}

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x08
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Container> class back_insert_iterator {
public:
    _Container* container;
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator(const back_insert_iterator& __x) : container(__x.container) { }
    template <class _Value> back_insert_iterator& operator=(const _Value& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
    back_insert_iterator& operator++(int) { return *this; }
};
}

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall CopyFlaggedThings(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    for (; __first != __last; ++__first) {
        CThing* __thing = *__first;
        if (__thing->FlagD4)
            *__result++ = __thing;
    }
    return __result;
}

template std::back_insert_iterator<std::vector<CThing*> > __fastcall
CopyFlaggedThings<std::_List_iterator<CThing*>, std::back_insert_iterator<std::vector<CThing*> > >(
    std::_List_iterator<CThing*>, std::_List_iterator<CThing*>, std::back_insert_iterator<std::vector<CThing*> >);