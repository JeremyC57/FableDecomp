// CSoundManagerLH::vfunc_4(const C3DVector& position) -- when the manager has room (slot 6), create a CSoundEmitterLH for this manager at the position, append it to the emitter list (+0x4C), call its slot 1 and return it; null when full
// Leaf: vfunc_4
// Retail: Fable.exe @ 0x00A3ECB0, 119 bytes (+ int3 padding), thiscall `ret 4`. Slot 4 of the CSoundManagerLH vtable
//   0x0129D2C4 (the vtable's only reference to the function; landed siblings vfunc_1/5/6 0x00A3E430/0x00A3EB60/
//   0x00A3EC70 use the same layout).
// Callees: vfunc_6 (+0x18; "below the entry limit", tested as a bool in al), operator new 0x00BFEA1A (0x20 bytes),
//   CSoundEmitterLH::CSoundEmitterLH(owner, position) 0x00AAC580 (landed), malloc 0x00BFEA0E (the 12-byte list node),
//   the emitter's slot 1 (+0x04).
// Label: catalog "PrimitiveStats_EnumerateAndInsert" is a fold label; the sound module is absent from the PDBs, so the
//   slot keeps the lane's vfunc_N name.
// Shape: `new` with its null arm (`xor ebx,ebx`); STLport list push_back = insert(end(), x) inline (end() read before
//   the node is malloc'd, placement-new null test on &node->_M_data, the four link stores); back() re-reads the
//   sentinel for both the slot-1 call and the return value; the `return 0` written last keeps its block at the end.
extern "C" void* __cdecl malloc(unsigned int size);         // MSVCR71 import thunk 0x00BFEA0E
void* __cdecl operator new(unsigned int size);               // MSVCR71 import thunk 0x00BFEA1A
inline void* operator new(unsigned int, void* p) { return p; }

struct C3DVector { float X, Y, Z; };

class CSoundManagerLH;

class CSoundEmitterLH {                                     // vtable 0x0129E6EC, 0x20 bytes
public:
    virtual void v00();
    virtual void Start();                                   // slot 1 (+0x04)
    unsigned char _04[0x1c];                                // +0x04..+0x1f
    CSoundEmitterLH(CSoundManagerLH* owner, const C3DVector& position);   // 0x00AAC580
};

namespace std {
template <class _Tp> class allocator { public: allocator() { } };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;                               // retail list = the sentinel pointer only

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator end() { return iterator(_M_node); }
    _Tp& back() { return *(--end()); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

class CSoundManagerLH {                                     // vtable 0x0129D2C4
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual CSoundEmitterLH* vfunc_4(const C3DVector& position);
    virtual void v05();
    virtual bool vfunc_6();                                 // slot 6 (+0x18): entries below the limit
    unsigned char _04[0x48];                                // +0x04..+0x4b
    std::list<CSoundEmitterLH*> m_entries;                 // +0x4c
};

CSoundEmitterLH* CSoundManagerLH::vfunc_4(const C3DVector& position)
{
    if (vfunc_6()) {
        m_entries.push_back(new CSoundEmitterLH(this, position));
        m_entries.back()->Start();
        return m_entries.back();
    }
    return 0;
}