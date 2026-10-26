// CParticleSystem::DeleteParticle(iterator&) -- clears the particle at the iterator, deletes it, unlinks and frees its list node, decrements the live-particle count and returns the iterator to the next particle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF5330, 68 bytes (`ret 8`: the hidden result pointer and the iterator reference).
// Callees: CParticleSystem::ClearParticle @ 0x00AF5090, the particle's virtual scalar deleting destructor (slot 0,
// `push 1 ; call [edx]`), free @ 0x00BFEA14 (the node).
// Shape: the iterator is a 4-byte class (user constructor) returned through the hidden pointer, written last
// (`mov [eax],esi`). The node is re-read from the iterator after the delete (the virtual call may alias it), then
// the STLport `erase(iterator)` is inlined: next/prev loaded, prev->next then next->prev stored, the node freed.
// NoActiveParticles (+0x48, PDB +0x64) is decremented with a load/dec/store after the free.
namespace NParticleEngine {
class CParticle { public:
    virtual ~CParticle();
};
}

extern "C" void __cdecl free(void* memory);         // 0x00BFEA14 (the STLport node allocator's deallocate)

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
    };
    SListNode<T>* _M_node;
    iterator erase(iterator pos)
    {
        SListNode<T>* next = pos._M_node->_M_next;
        SListNode<T>* prev = pos._M_node->_M_prev;
        SListNode<T>* n = pos._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(n);
        return iterator(next);
    }
};

struct CParticleSystem {
    typedef CRetailList<NParticleEngine::CParticle*>::iterator ParticleIterator;
    unsigned char                            m_pad00[0x1c];
    CRetailList<NParticleEngine::CParticle*> Particles;              // +0x1c
    void*                                    Decals;                 // +0x20
    unsigned char                            UpdateParameters[0x24]; // +0x24
    long                                     NoActiveParticles;      // +0x48
    long                                     NoActiveDecals;         // +0x4c
    void ClearParticle(ParticleIterator& it);                         // 0x00AF5090
    ParticleIterator DeleteParticle(ParticleIterator& it);
};

CParticleSystem::ParticleIterator CParticleSystem::DeleteParticle(ParticleIterator& it)
{
    ClearParticle(it);
    delete *it;
    ParticleIterator next = Particles.erase(it);
    --NoActiveParticles;
    return next;
}