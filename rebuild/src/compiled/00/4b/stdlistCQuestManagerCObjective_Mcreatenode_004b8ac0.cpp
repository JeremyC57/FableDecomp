// std::list<CQuestManager::CObjective>::_M_create_node(const CQuestManager::CObjective& x) -- STLport list node allocation: mallocs a 0x14-byte node and copy-constructs the quest objective into it
// Leaf: ?_M_create_node@?$list@VCObjective@CQuestManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B8AC0, 54 bytes, thiscall `ret 4` (this unused), returns the node in eax.
//   An out-of-line COMDAT with no direct caller (the xref.py hit 0x00501E3C decodes to no instruction).
// Identity (probable): the node is 0x14 bytes (next, prev, then a 12-byte value at +8); the value's copy is a
//   CWideString copy ctor 0x0099B720 on +0 then two plain dwords +4/+8 (an implicit memberwise copy). The only 12-byte
//   Ego_r type with exactly one leading CWideString that FableWin.pdb keeps in a std::list is CQuestManager::CObjective
//   { CWideString Name; unsigned long Index; ECompletionStatus CompletionStatus; } (list<CQuestManager::CObjective>);
//   the neighbouring rows (0x004B87F0 list<CQuestManager::CBoast>::erase, 0x004B91A0 list<CQuestManager::CQuestCard>)
//   are the quest manager's lists. CProfileManagerEntry is also CWideString+8 but has no list in either PDB.
// Callees: malloc 0x00BFEA0E (Fable's STLport node allocator), CWideString::CWideString(const CWideString&) 0x0099B720.
// Shape: STLport `_M_create_node`: `_M_node.allocate(1)` then `_Construct(&tmp->_M_data, x)`; the placement new tests
//   the element address (`lea esi,[ebx+8] ; test esi,esi`). No EH frame (no /EHsc).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);                  // 0x0099B720
};

class CQuestManager {
public:
    enum ECompletionStatus { COMPLETION_STATUS_0 };
    class CObjective {                                      // Ego_r.pdb 12 bytes
    public:
        CWideString       Name;                             // +0x0
        unsigned long     Index;                            // +0x4
        ECompletionStatus CompletionStatus;                 // +0x8
    };
};

namespace std {
class __malloc_alloc {                                      // Fable's STLport node allocator: malloc
public:
    static void* allocate(unsigned int n) { return malloc(n); }
};
template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)__malloc_alloc::allocate(n * sizeof(T)) : 0; }
};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    Tp* allocate(unsigned int n) { return A::allocate(n); }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
struct _List_node_base {
    _List_node_base* _M_next;                               // +0
    _List_node_base* _M_prev;                               // +4
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                              // +8
};
template <class T, class A> class _List_base {
public:
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // the header node pointer
};
template <class T, class A = allocator<T> > class list : public _List_base<T, A> {
public:
    typedef _List_node<T> _Node;
    _Node* _M_create_node(const T& x)
    {
        _Node* tmp = this->_M_node.allocate(1);
        _Construct(&tmp->_M_data, x);
        return tmp;
    }
};
template class list<CQuestManager::CObjective>;
} // namespace std