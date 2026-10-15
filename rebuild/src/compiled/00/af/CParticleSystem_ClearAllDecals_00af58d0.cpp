// CParticleSystem::ClearAllDecals(CEnginePrimitiveHandle*) -- runs ClearDecal on every decal of the system's decal list
// Leaf: ClearAllDecals
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF58D0, 47 bytes (`ret 4`, the handle argument unused).
// Callees: CParticleSystem::ClearDecal(const iterator&) @ 0x00AF5430.
// Label: the decal half of the 0x00AF58A0 / 0x00AF58D0 pair (walks Decals, +0x20, calling ClearDecal).
// Shape: CParticleSystem's STLport lists are one word each in retail (Particles +0x1c, Decals +0x20). The
// iterator handed to ClearDecal lives in the `push ecx` frame slot while the walk itself runs on the node
// pointer in esi: the slot is refreshed after every step (`mov [esp+8],esi`) and the step reads the register
// copy after the call. A `for (it...; ++it) ClearDecal(it)` loop re-reads the escaped iterator from the
// frame after every call (retail was built with whole-program optimisation and knew ClearDecal leaves it
// alone); a temporary copy stores it right before the call. Walking the node and re-pointing the iterator
// object after each step is the source form that gives the retail order.
namespace NParticleEngine { struct CParticle; struct CDecal; }
struct CEnginePrimitiveHandle;

// STLport list (retail: ONE word, the sentinel node pointer): node = next, prev, data.
template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};

template <class T> struct CRetailList {
    struct iterator {
        SListNode<T>* _M_node;
        iterator() {}
        iterator(SListNode<T>* n) : _M_node(n) {}
        T& operator*() const { return _M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    SListNode<T>* _M_node;
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator end() const { return iterator(_M_node); }
};

struct CParticleSystem {
    unsigned char                           m_pad00[0x1c];
    CRetailList<NParticleEngine::CParticle*> Particles;   // +0x1c
    CRetailList<NParticleEngine::CDecal*>    Decals;      // +0x20
    void ClearDecal(const CRetailList<NParticleEngine::CDecal*>::iterator& it);         // 0x00AF5430
    void ClearAllDecals(CEnginePrimitiveHandle* handle);
};

void CParticleSystem::ClearAllDecals(CEnginePrimitiveHandle* handle)
{
    CRetailList<NParticleEngine::CDecal*>::iterator it = Decals.begin();
    SListNode<NParticleEngine::CDecal*>* node = it._M_node;
    while (node != Decals._M_node) {
        ClearDecal(it);
        node = node->_M_next;
        it._M_node = node;
    }
}