// CParticleEmitterList::ClearLowestPriorityNonVisibleEmitterParticles(CIEngine*) -- walking the active list from the back (lowest priority first), clears the particles of the first initialised, non-visible emitter that has any allocated
// Leaf: ClearLowestPriorityNonVisibleEmitterParticles
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEAF20, 87 bytes (`ret 4`). 0x00AEAF80 is the decal twin.
// Callees: CParticleEmitter::IsVisible @ 0x00AED010, CParticleEmitter::HasAllocatedParticles @ 0x00AED1B0,
// CParticleEmitter::ClearAllParticles @ 0x00AED140.
// Label: the chunk names both twins "ClearLowestPriorityNonVisibleEmitterDecals"; the callees and the callers
// (CParticleSystem::AllocateParticle 0x00AF5010 / AllocateDecal 0x00AF5380) make this the particles one.
// Shape: Active (list<CCountedPointer<CParticleEmitter>>) is the list at +0 (one STLport word); the loop is a
// reverse_iterator walk: rbegin holds the sentinel, `*rit` reads current->prev's data (`mov eax,[esi+4] ; mov
// ecx,[eax+8]`, re-read before every call), rend is begin() re-read from the sentinel each pass. Initialised is
// bit 8 of the emitter's ushort flags at +0x40 (`test byte ptr [ecx+41h],1`).
class CIEngine;
struct CParticleStats;

namespace NParticleEngine {
// CParticleEmitter at retail offsets (rebuild/include/engine/CParticleEmitter.h has the PDB layout, 0x0c bytes
// higher from +0x18 on: the retail vector is 12 bytes and CParticleUpdateParameters 0x24).
class CParticleEmitter { public:
    void*          m_vtable;                         // +0x00
    unsigned char  m_base04[0x04];
    unsigned char  ParticleSystems[0x0c];            // +0x08 vector<CCountedPointer<CParticleSystem>>
    unsigned char  UpdateParameters[0x24];           // +0x14
    unsigned char  PrimitiveGroupHandle[0x08];       // +0x38
    unsigned short Priority : 3;                     // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;                  // bit 8 (+0x41 bit 0)
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;                     // bit 10
    unsigned short Emitter2D : 1;
    bool IsVisible() const;                          // 0x00AED010 (labelled "CTCPhysicsStandard::CanCollideWithLandscape")
    bool HasAllocatedParticles() const;              // 0x00AED1B0
    bool HasAllocatedDecals() const;                 // 0x00AED220
    void ClearAllParticles(CIEngine* engine);        // 0x00AED140
    void ClearAllDecals(CIEngine* engine);           // 0x00AED1E0
    void GetParticleStats(CParticleStats* stats);    // 0x00AED290
};

struct SCountBlock;
template <class T> struct CCountedPointer {
    T*           Object;
    SCountBlock* Block;
    T* operator->() const { return Object; }
};
}

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};

template <class T> struct CRetailList {              // STLport list: one word, the sentinel node
    struct iterator {
        SListNode<T>* _M_node;
        iterator(SListNode<T>* n) : _M_node(n) {}
        T& operator*() const { return _M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    struct reverse_iterator {                        // STLport reverse_iterator: holds the base iterator
        iterator current;
        reverse_iterator(const iterator& it) : current(it) {}
        T& operator*() const { iterator tmp = current; return *--tmp; }
        reverse_iterator& operator++() { --current; return *this; }
        bool operator!=(const reverse_iterator& o) const { return current != o.current; }
    };
    SListNode<T>* _M_node;
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator end() const { return iterator(_M_node); }
    reverse_iterator rbegin() const { return reverse_iterator(end()); }
    reverse_iterator rend() const { return reverse_iterator(begin()); }
};

typedef NParticleEngine::CCountedPointer<NParticleEngine::CParticleEmitter> CEmitterPointer;

struct CParticleEmitterList {
    CRetailList<CEmitterPointer> Active;             // +0x00
    long                         MaxActiveEmitters;  // +0x04
    void ClearLowestPriorityNonVisibleEmitterParticles(CIEngine* engine);
};

void CParticleEmitterList::ClearLowestPriorityNonVisibleEmitterParticles(CIEngine* engine)
{
    for (CRetailList<CEmitterPointer>::reverse_iterator it = Active.rbegin(); it != Active.rend(); ++it) {
        if ((*it)->Initialised && !(*it)->IsVisible() && (*it)->HasAllocatedParticles()) {
            (*it)->ClearAllParticles(engine);
            return;
        }
    }
}