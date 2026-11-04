// NParticleEngine::CParticleSystem::DeleteAllParticles() -- for every particle: clear it, delete it and erase its list node (counting down); then clear the list and zero the count
// Leaf: ?DeleteAllParticles@CParticleSystem@NParticleEngine@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF56E0, 138 bytes (padded to 144). Ego_r.pdb / FableWin.pdb
//   ?DeleteAllParticles@CParticleSystem@NParticleEngine@@QAEXXZ.
// Callees: CParticleSystem::ClearParticle(iterator&) @ 0x00AF5090 (the landed DeleteParticle 0x00AF5330 spelling), the
//   particle's virtual scalar deleting destructor (slot 0, `push 1`), free @ 0x00BFEA14 (each list node).
// Layout: as the landed same-TU sibling DeleteParticle 0x00AF5330: Particles (STLport list<CParticle*>, one header word) +0x1c,
//   NoActiveParticles +0x48.
// Shape: one loop iterator handed to ClearParticle by reference. ClearParticle (0x00AF5090) sits in this TU right above the
//   row; VC7.1 sees that its body only reads the iterator, so `it` stays in edi across the call (its frame home is refreshed
//   from edi after `it = erase(it)`: `mov edi,ebx ; mov [esp+0Ch],edi`). The out-of-line callee is therefore given a
//   same-TU auto_inline(off) stand-in that reads the iterator and writes only the system (EXTRA_RULES chunk 422; the
//   landed CPSCEmitterGeneric::WriteBinary 0x00B12EF0 stand-in). Without it VC7.1 treats the iterator as escaped and the
//   loop needs a second frame iterator, which stores the erase result from ebx.
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

namespace std {
template <class T> class allocator;
template <class T, class A> class _List_val { public:   // the PDB's list base (only its arguments matter here)
    typedef T value_type;
};
template <class V> class _List_iterator { public:      // 4 bytes: the node pointer
    typedef typename V::value_type T;
    SListNode<T>* _M_node;
    _List_iterator() {}
    _List_iterator(SListNode<T>* n) : _M_node(n) {}
    T& operator*() const { return _M_node->_M_data; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
}

template <class T> struct CRetailList {
    typedef std::_List_iterator<std::_List_val<T, std::allocator<T> > > iterator;
    SListNode<T>* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
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
    void clear()
    {
        SListNode<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SListNode<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};

namespace NParticleEngine {
class CParticleSystem { public:
    typedef CRetailList<CParticle*>::iterator ParticleIterator;
    unsigned char                            m_pad00[0x1c];
    CRetailList<CParticle*>                  Particles;              // +0x1c
    void*                                    Decals;                 // +0x20
    unsigned char                            UpdateParameters[0x24]; // +0x24
    long                                     NoActiveParticles;      // +0x48
    long                                     NoActiveDecals;         // +0x4c
    void ClearParticle(ParticleIterator& it);                         // 0x00AF5090
    void DeleteAllParticles();
};
}

#pragma auto_inline(off)
// Same-TU stand-in for 0x00AF5090 (retail: release the particle's primitives): reads the iterator, writes only the system.
void NParticleEngine::CParticleSystem::ClearParticle(ParticleIterator& it)
{
    if (*it == 0) ++NoActiveDecals;
}
#pragma auto_inline(on)

void NParticleEngine::CParticleSystem::DeleteAllParticles()
{
    ParticleIterator it = Particles.begin();
    while (it != Particles.end()) {
        ClearParticle(it);
        delete *it;
        it = Particles.erase(it);
        --NoActiveParticles;
    }
    Particles.clear();
    NoActiveParticles = 0;
}