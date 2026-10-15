// NParticleEngine::CParticleSystem::DeleteAllParticles() -- for every particle: notify, delete it and erase its list node (counting down); then clear the list and zero the count
// Grade: DIFFER(138v138) -- one operand: at the loop bottom retail stores the frame iterator from edi after `mov edi,ebx` (`mov [esp+0Ch],edi`), the lane stores it from ebx (`mov [esp+0Ch],ebx`); every other byte matches (register-choice artefact)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF56E0, length 138 bytes (padded to 144).
// Leaf: DeleteAllParticles
// Callees: CParticleSystem::ClearParticle(const list<CParticle*>::iterator&) @ 0x00AF5090, the particle's virtual scalar
// deleting destructor (slot 0, `push 1`), free @ 0x00BFEA14 (each list node).
// Layout: Particles (STLport list<CParticle*>, one header word) +0x1c, NoActiveParticles +0x48 (retail CParticleSystem).
// Shape: the walking iterator lives in a register while a SEPARATE frame iterator (`current`, assigned before the loop and at
// its bottom) is what ClearParticle receives by reference -- retail reads the node from edi after the call; passing the
// loop iterator itself re-reads it from the frame. Then `delete *it`, STLport erase (next/prev unlinked, node freed),
// --NoActiveParticles; afterwards STLport clear() (cursor advanced before each free, header re-read) and the count zeroed.
// Tried: non-const / const reference to the loop iterator (frame re-read), a by-value forwarding wrapper (copy stored before
// the call), the separate `current` iterator (this file).
extern "C" void __cdecl free(void* block);  // 0x00BFEA14
namespace NParticleEngine {
struct CParticle {
    virtual ~CParticle();                   // slot 0 (scalar deleting destructor)
};
struct _List_node {
    _List_node* _M_next;                    // +0
    _List_node* _M_prev;                    // +4
    CParticle*  _M_data;                    // +8
};
struct _List_iterator {
    _List_node* _M_node;
    _List_iterator() {}
    _List_iterator(_List_node* n) : _M_node(n) {}
    CParticle*& operator*() const { return _M_node->_M_data; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
struct CParticleList {                      // STLport list<CParticle*> (one header word)
    _List_node* _M_node;
    _List_iterator begin() { return _List_iterator(_M_node->_M_next); }
    _List_iterator end() { return _List_iterator(_M_node); }
    _List_iterator erase(_List_iterator position)
    {
        _List_node* next = position._M_node->_M_next;
        _List_node* prev = position._M_node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(position._M_node);
        return _List_iterator(next);
    }
    void clear()
    {
        _List_node* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
struct CParticleSystem {
    unsigned char _system[0x1c];
    CParticleList Particles;                // +0x1c
    unsigned char _system2[0x28];
    long          NoActiveParticles;        // +0x48
    void ClearParticle(const _List_iterator& it);   // 0x00AF5090
    void DeleteAllParticles();
};
void CParticleSystem::DeleteAllParticles()
{
    _List_iterator it = Particles.begin();
    _List_iterator current = it;
    while (it != Particles.end()) {
        ClearParticle(current);
        delete *it;
        it = Particles.erase(it);
        --NoActiveParticles;
        current = it;
    }
    Particles.clear();
    NoActiveParticles = 0;
}
}
