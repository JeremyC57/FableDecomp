// list<CExpressionState>::push_front(const CExpressionState& x) -- STLport list push_front of a creature expression state: allocate a node, copy the state into it, link it in before the first node
// Grade: DIFFER(73v73) -- scheduling only: retail pushes edi at entry (push esi ; push edi) and pops it in the common tail; ours shrink-wraps edi into the placement-copy block (push edi after the null test, pop edi before the link). Tried: _Construct placement copy (this), a direct placement new with explicit padding bytes.
// Leaf: ?push_front@CExpressionStateList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00729960, 73 bytes, `ret 4`.
// Label correction: the catalog "ExpressionSet_InsertNode" is descriptive; its caller CTCCreatureExpression::
//   ForcePrimaryExpression (landed 0x007293A0) documents it as list<CExpressionState>::push_front.
// Callee: malloc @ 0x00BFEA0E (the node allocator, 0x1c bytes).
// Layout: STLport list = one sentinel pointer; node {next +0, prev +4, CExpressionState +8}; Ego_r.pdb
//   CExpressionState (0x14): AnimIndex +0, MaxWeight +4, Weight +8, OldWeight +0xc, Primary +0x10, Stage +0x11
//   (model of the landed caller).
// Shape: STLport `push_front(x) { insert(begin(), x); }` -- begin() (head->next) is read before the allocation;
//   `_M_create_node`: malloc + the placement copy behind its null test (the state's trivial copy as five dwords
//   through edi); insert reads `prev = pos->prev` before `node->next = pos` and links prev->next / pos->prev last.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

struct CExpressionState {                               // 0x14 bytes (PDB)
    long          AnimIndex;                            // +0x00
    float         MaxWeight;                            // +0x04
    float         Weight;                               // +0x08
    float         OldWeight;                            // +0x0c
    bool          Primary;                              // +0x10
    unsigned char Stage;                                // +0x11
    CExpressionState(long animIndex, float maxWeight)
        : AnimIndex(animIndex), MaxWeight(maxWeight), Weight(0.0f), OldWeight(0.0f), Primary(true), Stage(0) {}
};

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
struct _List_node : _List_node_base {
    CExpressionState _M_data;                           // +0x08
};

struct CExpressionStateList {                           // STLport list<CExpressionState>: one sentinel pointer
    _List_node_base* _M_node;                           // +0x00
    _List_node_base* begin() { return _M_node->_M_next; }
    _List_node* _M_create_node(const CExpressionState& x)
    {
        _List_node* p = (_List_node*)malloc(sizeof(_List_node));
        _Construct(&p->_M_data, x);
        return p;
    }
    _List_node_base* insert(_List_node_base* position, const CExpressionState& x)
    {
        _List_node* tmp = _M_create_node(x);
        _List_node_base* n = position;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_front(const CExpressionState& x);
};

void CExpressionStateList::push_front(const CExpressionState& x)
{
    insert(begin(), x);
}
