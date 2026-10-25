// NParticleEngine::CPSCSingleSprite::AddTrailPosition(const C3DVector& pos, float width) -- appends one (position, width) entry to the sprite's trail primitive and drops the oldest entries while the trail is longer than MaxTrailLength
// Leaf: ?AddTrailPosition@CPSCSingleSprite@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B081A0, length 164 bytes, thiscall `ret 8`.
// Signature from Ego_r.pdb: ?AddTrailPosition@CPSCSingleSprite@NParticleEngine@@IAEXABVC3DVector@@M@Z (protected).
//   Callers: CPSCSingleSprite::Update @ 0x00B08530 (calls 0x00B0866A / 0x00B08763, width = TrailWidth).
// Callees: malloc @ 0x00BFEA0E (the STLport list node, 0x18 bytes) and free @ 0x00BFEA14 (pop_front).
// Layout (Ego_r.pdb, retail 4 bytes lower -- one 12-byte STLport vector ahead of these members): CPSCSingleSprite
//   TrailWidth +0x84, MaxTrailLength +0x88, CEnginePrimitiveSpriteTrail TrailUpdatePrimitive +0xAC whose
//   list<CSpriteTrailEntry> Trail sits at +0x0C (absolute +0xB8; retail STLport list = one sentinel pointer).
//   CSpriteTrailEntry = C3DVector Position +0, float Width +0x0C.
// Shape: `Trail.push_back(CSpriteTrailEntry(pos, width)); while (Trail.size() > MaxTrailLength) Trail.pop_front();`
//   -- the entry temporary keeps a copy of pos across the node allocation (width is read from its argument slot),
//   end() is loaded before the malloc, the placement-new null test guards the copy, size() is STLport's counting walk
//   and pop_front the inline erase(begin()) unlink + free.  C3DVector needs its memberwise user copy constructor: with
//   the implicit one VC7.1 keeps the pos copy in ebx/ebp instead of retail's three frame words (DIFFER 166v164).
// Label: catalog "EventQueue_PushEvent" is behavioural.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;                               // +0
    _List_node_base* _M_prev;                               // +4
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                              // +8
};
template <class T> struct _List_iterator {
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_node_base* _M_node;
};
template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T> class list {                             // STLport list: one pointer to the sentinel node
public:
    typedef _List_iterator<T> iterator;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    unsigned int size() const
    {
        unsigned int n = 0;
        for (_List_node_base* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    _List_node<T>* _M_create_node(const T& x)
    {
        _List_node<T>* tmp = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        new (&tmp->_M_data) T(x);
        return tmp;
    }
    iterator insert(iterator position, const T& x)
    {
        _List_node<T>* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
    iterator erase(iterator position)
    {
        _List_node_base* next_node = position._M_node->_M_next;
        _List_node_base* prev_node = position._M_node->_M_prev;
        _List_node<T>* n = (_List_node<T>*)position._M_node;
        prev_node->_M_next = next_node;
        next_node->_M_prev = prev_node;
        _Destroy(&n->_M_data);
        free(n);
        return iterator(next_node);
    }
    void pop_front() { erase(begin()); }
    _List_node_base* _M_node;                               // +0
};

} // namespace std

class CEnginePrimitiveSpriteTrail {
public:
    struct CSpriteTrailEntry {
        C3DVector Position;                                 // +0x00
        float     Width;                                    // +0x0C
        CSpriteTrailEntry(const C3DVector& pos, float width) : Position(pos), Width(width) { }
    };
    unsigned char                     _pad_0x00[0x0C];
    std::list<CSpriteTrailEntry>     Trail;                // +0x0C
};

namespace NParticleEngine {

class CPSCSingleSprite {
public:
    unsigned char               _pad_0x00[0x84];
    float                       TrailWidth;                 // +0x84 (retail; PDB +0x88)
    long                        MaxTrailLength;             // +0x88 (retail; PDB +0x8C)
    unsigned char               _pad_0x8c[0x20];
    CEnginePrimitiveSpriteTrail TrailUpdatePrimitive;       // +0xAC (retail; PDB +0xB0)
protected:
    void AddTrailPosition(const C3DVector& pos, float width);
};

void CPSCSingleSprite::AddTrailPosition(const C3DVector& pos, float width)
{
    TrailUpdatePrimitive.Trail.push_back(CEnginePrimitiveSpriteTrail::CSpriteTrailEntry(pos, width));
    while (TrailUpdatePrimitive.Trail.size() > (unsigned long)MaxTrailLength)
        TrailUpdatePrimitive.Trail.pop_front();
}

} // namespace NParticleEngine