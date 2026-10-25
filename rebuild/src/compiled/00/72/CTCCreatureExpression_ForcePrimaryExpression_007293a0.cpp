// CTCCreatureExpression::ForcePrimaryExpression(long animIndex, float maxWeight) -- make the expression the primary one: when it already is the front state just update its max weight, else demote the front state and push a new primary state for it (index 0 = none)
// Leaf: ?ForcePrimaryExpression@CTCCreatureExpression@@QAEXJM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007293A0, 99 bytes, thiscall `ret 8`. Callee: std::list<CTCCreatureExpression::
//   CExpressionState>::push_front(const CExpressionState&) @ 0x00729960 (out-of-line COMDAT, landed under that name;
//   manifest "ExpressionSet_InsertNode").
// Grade: RELOCATION_MATCH (sweep1, 2026-10-25) -- re-modelled with the real STLport list (FINDINGS near-miss pass 2: "0x7293A0
//   calls push_front on a model list type"): CExpressionState is the nested class CTCCreatureExpression::CExpressionState
//   (Ego_r.pdb `?Update@CExpressionState@CTCCreatureExpression@@QAEXM@Z`), so the call binds the same
//   `?push_front@?$list@VCExpressionState@CTCCreatureExpression@@...` the landed 0x00729960 defines, not the old
//   `?push_front@CExpressionStateList@@`. Tried: one `front()` reference local (101v99: the reference costs `add eax,8`);
//   `front()` at each use, an iterator local and a raw node pointer all match.
// Layout: as the landed ctor 0x00729360: ExpressionStates (STLport list, one sentinel pointer) at +0x0c; node {next,
//   prev, CExpressionState +8}; Ego_r.pdb CExpressionState (0x14): AnimIndex +0, MaxWeight +4, Weight +8, OldWeight +0xc,
//   Primary +0x10, Stage +0x11.
// Shape: `!empty()`, then the front state read through `head->next` at each use; the new state is a 0x14-byte frame
//   temporary built by the inline constructor (index, max weight, two zero floats, Primary = true, Stage = 0) and passed
//   by reference to the out-of-line push_front (ecx kept as `this + 0xc`).
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
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
};
template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
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
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
    _Tp& front() { return *begin(); }
    void push_front(const _Tp& __x);                    // 0x00729960 (out-of-line COMDAT)
};
}
class CTCCreatureExpression {
public:
    class CExpressionState {                            // 0x14 bytes (Ego_r.pdb)
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
    virtual void v00();
    void*                       ParentThing;            // +0x04 (CTCBase)
    unsigned char               _pad08[4];
    std::list<CExpressionState> ExpressionStates;       // +0x0c
    bool                        Blinking;               // +0x10
    void ForcePrimaryExpression(long animIndex, float maxWeight);
};
void CTCCreatureExpression::ForcePrimaryExpression(long animIndex, float maxWeight)
{
    if (!ExpressionStates.empty())
    {
        if (ExpressionStates.front().AnimIndex == animIndex)
        {
            ExpressionStates.front().MaxWeight = maxWeight;
            return;
        }
        ExpressionStates.front().Primary = false;
    }
    if (animIndex)
        ExpressionStates.push_front(CExpressionState(animIndex, maxWeight));
}
