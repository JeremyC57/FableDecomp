// std::list<CCountedPointer<CSpawnedFuncBase> >::insert(iterator pos, const CCountedPointer<CSpawnedFuncBase>& x) -- STLport list insert: malloc a 16-byte node, placement copy-construct the counted pointer into it (bumping the shared reference count), link it in front of `pos` and return an iterator to it
// Leaf: insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005355C0, 67 bytes, `ret 0Ch` (__fastcall with `this` unused: the hidden
// iterator result, the by-value iterator and the element reference on the stack). Callee: malloc @ 0x00BFEA0E.
// A byte-identical twin sits at 0x00AEC6F0 (another TU). Call sites: 0x00540D8B (push_back @ 0x00540D70, `insert(end(), x)` on the list
// at this+0x15C) and 0x00540DBB.
// Shape: STLport 4.x `_Node* tmp = _M_create_node(x); n = pos._M_node; p = n->_M_prev; tmp->_M_next = n; tmp->_M_prev =
// p; p->_M_next = tmp; n->_M_prev = tmp; return iterator(tmp);` -- the node allocator is the static malloc allocator (no
// out-of-memory check), the element is built with placement new (the null test on &tmp->_M_data), and the iterator has
// user constructors, so it is returned through the hidden pointer (eax = the result address).
// Types (Ego_r.pdb): CCountedPointer<T> (8 bytes) = T* Data +0, CCPPointerInfo* Info +4; CCPPointerInfo.RefCount at +0.
// Label correction: the catalog's `_List_val<CCountedPointer<CSpawnedFuncBase> >::_Buynode` (Dinkumware) is propagated;
// the body is the whole STLport insert.
extern "C" void* __cdecl malloc(unsigned int size);

inline void* operator new(unsigned int, void* p) { return p; }

class CSpawnedFuncBase;

class CCPPointerInfo { public:                          // 12 bytes
    unsigned long RefCount;                      // +0x00
    void (__cdecl* DeleteFunc)(void*);           // +0x04
    void*         Data;                          // +0x08
};

template <class T>
class CCountedPointer { public:                         // 8 bytes
    T*              Data;                        // +0x00
    CCPPointerInfo* Info;                        // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

struct _List_node_base {
    _List_node_base* _M_next;                    // +0x00
    _List_node_base* _M_prev;                    // +0x04
};

template <class T>
struct _List_node : _List_node_base {
    T _M_data;                                   // +0x08
};

template <class T>
struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_iterator(const _List_iterator& x) : _M_node(x._M_node) { }
};

template <class T>
struct list {
    typedef _List_node<T>     _Node;
    typedef _List_iterator<T> iterator;

    _List_node_base* _M_node;                    // the sentinel node

    _Node* _M_create_node(const T& x)
    {
        _Node* p = (_Node*)malloc(sizeof(_Node));
        new (&p->_M_data) T(x);
        return p;
    }

    iterator insert(iterator pos, const T& x);
};

template <class T>
typename list<T>::iterator list<T>::insert(iterator pos, const T& x)
{
    _Node* tmp = _M_create_node(x);
    _List_node_base* n = pos._M_node;
    _List_node_base* p = n->_M_prev;
    tmp->_M_next = n;
    tmp->_M_prev = p;
    p->_M_next = tmp;
    n->_M_prev = tmp;
    return tmp;
}

template struct list<CCountedPointer<CSpawnedFuncBase> >;