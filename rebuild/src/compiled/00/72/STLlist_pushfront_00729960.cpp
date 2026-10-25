// _STL::list::push_front(const CTCCreatureExpression::CExpressionState& x) -- STLport list<CExpressionState> push_front of a creature expression state: allocate a node, copy the state into it, link it in before the first node
// Grade: RELOCATION_MATCH (agent902, 1st grade of this pass) -- the parked flat `CExpressionStateList` (raw node pointers) shrink-wrapped edi into the placement-copy block; the real STLport 4.6 list (by-value _List_iterator<T, _Nonconst_traits<T> > with its declared copy ctor, begin() -> insert(iterator, x) -> _M_create_node) keeps retail's entry `push esi ; push edi`.
// Leaf: ?push_front@?$list@VCExpressionState@CTCCreatureExpression@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00729960, 73 bytes, `ret 4`.
// Label correction: the catalog "ExpressionSet_InsertNode" is descriptive; its caller CTCCreatureExpression::
//   ForcePrimaryExpression (landed 0x007293A0) calls it as list<CExpressionState>::push_front.
// Identity: CExpressionState is the nested class CTCCreatureExpression::CExpressionState (Ego_r.pdb
//   `?Update@CExpressionState@CTCCreatureExpression@@QAEXM@Z`), so the instantiation is
//   `_STL::list<CTCCreatureExpression::CExpressionState>` (an out-of-line COMDAT).
// Callee: malloc @ 0x00BFEA0E (the node allocator, 0x1c bytes).
// Layout: STLport list = one sentinel pointer (the alloc proxy's _M_data); node {next +0, prev +4, CExpressionState +8};
//   CExpressionState (0x14): AnimIndex +0, MaxWeight +4, Weight +8, OldWeight +0xc, Primary +0x10, Stage +0x11.
// Shape: STLport `push_front(x) { insert(begin(), x); }` -- begin() (head->next) is read before the allocation;
//   `_M_create_node`: allocate + the placement copy behind its null test (the state's trivial copy as five dwords
//   through edi); insert reads `prev = pos->prev` before `node->next = pos` and links prev->next / pos->prev last.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CTCCreatureExpression {
public:
    class CExpressionState {                            // 0x14 bytes (PDB)
    public:
        long          AnimIndex;                        // +0x00
        float         MaxWeight;                        // +0x04
        float         Weight;                           // +0x08
        float         OldWeight;                        // +0x0c
        bool          Primary;                          // +0x10
        unsigned char Stage;                            // +0x11
        CExpressionState(long animIndex, float maxWeight)
            : AnimIndex(animIndex), MaxWeight(maxWeight), Weight(0.0f), OldWeight(0.0f), Primary(true), Stage(0) {}
    };
};

namespace _STL {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x08
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
};
template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // the sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
public:
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_front(const _Tp& __x) { insert(begin(), __x); }
};
}

template void _STL::list<CTCCreatureExpression::CExpressionState>::push_front(const CTCCreatureExpression::CExpressionState&);