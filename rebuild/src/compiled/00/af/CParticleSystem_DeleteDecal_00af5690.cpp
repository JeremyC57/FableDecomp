// CParticleSystem::DeleteDecal(iterator&) -- clears the decal at the iterator, destroys and frees it, unlinks and frees its list node, decrements the live-decal count and returns the iterator to the next decal
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF5690, 78 bytes (`ret 8`: the hidden result pointer and the iterator reference).
// Callees: CParticleSystem::ClearDecal @ 0x00AF5430, NParticleEngine::CDecal::~CDecal @ 0x00AF7C10 (labelled
// "Catch@0047e8cc", propagated), CDecal::operator delete @ 0x00AF8AD0 (a `__fastcall` free-list push of the decal
// pool), free @ 0x00BFEA14 (the node).
// Shape: as DeleteParticle (0x00AF5330) with a non-polymorphic CDecal: `delete decal` is the null test, the
// out-of-line destructor and the class's static `__fastcall operator delete`, the decal held in ebx across both.
// NoActiveDecals (+0x4c, PDB +0x68) is decremented with a load/dec/store after the node is freed.
namespace NParticleEngine {
class CDecal { public:
    ~CDecal();                                                        // 0x00AF7C10
    static void __fastcall operator delete(void* p);                  // 0x00AF8AD0
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
    typedef CRetailList<NParticleEngine::CDecal*>::iterator DecalIterator;
    unsigned char                         m_pad00[0x1c];
    void*                                 Particles;              // +0x1c
    CRetailList<NParticleEngine::CDecal*> Decals;                 // +0x20
    unsigned char                         UpdateParameters[0x24]; // +0x24
    long                                  NoActiveParticles;      // +0x48
    long                                  NoActiveDecals;         // +0x4c
    void ClearDecal(DecalIterator& it);                            // 0x00AF5430
    DecalIterator DeleteDecal(DecalIterator& it);
};

CParticleSystem::DecalIterator CParticleSystem::DeleteDecal(DecalIterator& it)
{
    ClearDecal(it);
    delete *it;
    DecalIterator next = Decals.erase(it);
    --NoActiveDecals;
    return next;
}