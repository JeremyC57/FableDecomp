// std::list<pair<C2DVector,long>>::push_back(const pair<C2DVector,long>& x) -- STLport list push_back: malloc a node, copy a (pulse vector, timer) pair in, link it before the sentinel
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A4A70, length 59 bytes (+ int3 padding), `ret 4`.
// Callees: malloc @ 0x00BFEA0E (the 0x14-byte node: next, prev, the 12-byte value).
// Identity: CInputProcessJumpingAndRolling::ProcessRolling 0x006A40D0 calls 0x006A4A70 on +0x38 and 0x006A4AB0 on +0x3c:
//   Ego_r.pdb CInputProcessJumpingAndRolling::Pulses = list<pair<C2DVector,long>> and InitialFacingDirections =
//   list<C3DVector> (retail lists are the 4-byte sentinel pointer). This row is the Pulses instance; the two
//   instantiations compile to identical bytes. The catalog's "CTCRolling_InsertEvent" is a descriptive auto label.
// Shape: STLport `push_back(x) { insert(end(), x); }` -- end() reads the sentinel before the allocation;
//   _M_create_node is malloc + the placement copy (null test, three dword moves of the implicit copy);
//   insert links tmp between the sentinel's prev and the sentinel. The iterator needs STLport's declared copy
//   constructor: with the implicit one VC7.1 shrink-wraps the edi push into the copy block.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};

template <class _Tp>
struct _List_node : _List_node_base {
    _Tp _M_data;                                        // +0x08
};

template <class _Tp>
class list {                                            // retail STLport list: the sentinel node pointer only
public:
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;                           // +0x00
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* __x) : _M_node(__x) { }
        iterator(const iterator& __x) : _M_node(__x._M_node) { }
    };
    iterator end() { return iterator(_M_node); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new ((void*)&__p->_M_data) _Tp(__x);
        return __p;
    }
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
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

struct C2DVector {
    float X, Y;
};

template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};

typedef pair<C2DVector, long> SPulse;                  // 12 bytes: the pulse vector and its start time

template class list<SPulse>;