// _STL::list<CCountedPointer<CModeBase> >::remove_if(CTCCreatureModeManager::CModeBase::CIsType pred) -- erase every creature mode whose GetModeType() equals the predicate's type (each erased node releases its counted mode pointer and is freed)
// Leaf: ??$remove_if@VCIsType@CModeBase@CTCCreatureModeManager@@@?$list@V?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073E790, 104 bytes, thiscall, `ret 4` (the one-word predicate by value).
// Identity: FableWin.pdb `??$remove_if@VCIsType@CModeBase@CTCCreatureModeManager@@@?$list@V?$CCountedPointer@VCModeBase@
//   CTCCreatureModeManager@@@@...`; called by the landed CTCCreatureModeManager::RemoveMode 0x0073ABA0 (PModes +0x0c).
// Callees: CModeBase virtual slot 1 GetModeType (`call [eax+4]`), the count block's DeleteFunc (`call [eax+4]` with the
//   object in ecx), operator delete @ 0x00BFE9BC (the count block), free @ 0x00BFEA14 (the node).
// Layout: retail STLport list = the sentinel node pointer (4 bytes); node { next +0, prev +4, CCountedPointer { Data +8,
//   Info +0xc } }; CCPPointerInfo { RefCount +0, DeleteFunc +4, Data +8 } (Ego_r.pdb).
// Shape: STLport 4.6 `remove_if`: `first = begin(); last = end(); while (first != last) { next = first; ++next;
//   if (pred(*first)) erase(first); first = next; }` with the 4.6 `erase` (unlink through next/prev, _Destroy, deallocate);
//   after the virtual call the unlink re-reads node->next. The counted pointer's destructor (release at zero, then zero
//   both words) shares one zero register (ebp) for the null test and the two stores.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

struct CCPPointerInfo {                                     // Ego_r.pdb
    unsigned long RefCount;                                 // +0x0
    void (__fastcall* DeleteFunc)(void* data);              // +0x4
    void* Data;                                             // +0x8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    T* operator->() const { return Data; }
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};

class CTCCreatureModeManager {
public:
    class CModeBase {
    public:
        virtual void v00();
        virtual unsigned long GetModeType();                // slot 1
        class CIsType {
        public:
            unsigned long Type;
            bool operator()(const CCountedPointer<CModeBase>& mode) const { return mode->GetModeType() == Type; }
        };
    };
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first))
                erase(__first);
            __first = __next;
        }
    }
};

template void list<CCountedPointer<CTCCreatureModeManager::CModeBase> >::remove_if(CTCCreatureModeManager::CModeBase::CIsType);
}