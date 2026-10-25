// std::vector<pair<unsigned __int64, list<C2DLineF> > >::push_back(const value_type&) -- append a
// CNavigationLayer::SwitchableLineLists entry: copy-construct it in place at _M_finish when there is room (the pair's
// out-of-line copy constructor @ 0x00A825D0), else grow through _M_insert_overflow @ 0x00A84C20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A84FB0, oracle length 59 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; cmp ecx,[esi+8] ; je G ; test ecx,ecx ; je S ; mov eax,[esp+8] ; push eax ; call pair::pair(const pair&) ; S: add dword ptr [esi+4],0x10 ; pop esi ; ret 4 ; G: mov eax,[esp+8] ; push 1 ; push 1 ; lea edx,[esp+10h] ; push edx ; push eax ; push ecx ; mov ecx,esi ; call _M_insert_overflow ; pop esi ; ret 4
// STLport 4.6 `push_back`: `_Construct(_M_finish, x)` (placement new, hence the null test) and `++_M_finish`, else
// `_M_insert_overflow(_M_finish, x, _IsPODType(), 1, true)`; the empty __false_type temporary is homed in this function's
// dead argument slot, which is the address retail pushes (modelled with a user ctor: no zero-byte store).
// Element: pair<unsigned __int64, list<C2DLineF> > (16 bytes), the value_type of Ego_r.pdb CNavigationLayer::
//   SwitchableLineLists (CVectorMap<unsigned __int64, list<C2DLineF>, ...>, which derives from this vector); 0x00A825D0 is
//   that pair's copy constructor with the list copy inline and 0x00A84C20 the vector's _M_insert_overflow (agent422
//   NOTES / LESSONS). Was catalogued `_global::PushBack_CElement16_00a825d0_00a84fb0` (CRetailVector_CElement16 model).
// Leaf: ?push_back@?$vector@U?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@std@@V?$allocator@U?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@std@@@2@@std@@QAEXABU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@2@@Z
#include <new>

class C2DVector {
public:
    float X, Y;
};
class C2DLineF {                                   // 16 bytes (Ego_r.pdb)
public:
    C2DVector Point1;                              // +0x00
    C2DVector Point2;                              // +0x08
};

namespace std {
struct __false_type { __false_type() { } };        // user ctor: no zero-byte store for the tag temporary

template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                       // STLport 4.6: one word, the sentinel node pointer
public:
    void* _M_node;
};

template <class _T1, class _T2> struct pair {
    _T1 first;                                     // +0x00
    _T2 second;                                    // +0x08
    pair(const pair& __p);                         // 0x00A825D0 (the implicit copy, out of line)
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;                                 // +0x00
    _Tp* _M_finish;                                // +0x04
    _Tp* _M_end_of_storage;                        // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x00A84C20
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

typedef pair<unsigned __int64, list<C2DLineF> > _CSwitchableLineEntry;
template void vector<_CSwitchableLineEntry>::push_back(const _CSwitchableLineEntry& __x);   // emit just this member
}
