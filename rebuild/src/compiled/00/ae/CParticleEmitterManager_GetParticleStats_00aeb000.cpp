// CParticleEmitterManager::GetParticleStats(CParticleStats*) -- counts the active emitters, records the visible count, counts the sleeping ones and lets every active emitter add its own statistics
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEB000, 89 bytes (`ret 4`).
// Callees: CParticleEmitter::GetParticleStats @ 0x00AED290.
// Shape: the retail CParticleEmitterManager is ActiveEmitters (CParticleEmitterList: the one-word STLport list +0,
// MaxActiveEmitters +4), PerlinNoise (+8, 0x1000 bytes), MaxVisibleEmitters +0x1008, VisibleEmitterCount +0x100c
// (PDB +0x1014: the list is 8 bytes narrower). EmittersActive is the STLport `size()` -- the inlined distance
// loop (`++first` then `++n`); the Sleeping bit (bit 10 of the emitter's ushort flags at +0x40) is read through
// an inline bool accessor (`return Sleeping;`): the zero-extended word, `shr edx,0Ah ; test dl,1` (testing the
// bitfield directly gives `test byte ptr [ecx+41h],4`); the stats pointer is cached in edi and EmittersSleeping
// bumped in memory (`inc dword ptr [edi+8]`).
struct CIEngine;
struct CParticleStats;

namespace NParticleEngine {
// CParticleEmitter at retail offsets (rebuild/include/engine/CParticleEmitter.h has the PDB layout, 0x0c bytes
// higher from +0x18 on: the retail vector is 12 bytes and CParticleUpdateParameters 0x24).
struct CParticleEmitter {
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
    bool IsSleeping() const { return Sleeping; }
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

struct CParticleStats {
    long EmittersVisible;                            // +0x00
    long EmittersActive;                             // +0x04
    long EmittersSleeping;                           // +0x08
    long SystemsVisible;
    long SystemsActive;
    long SystemsSleeping;
    long ComponentsVisible;
    long ComponentsActive;
    long ComponentsSleeping;
    long TotalNumParticlesVisible;
    long TotalNumParticlesAllocated;
    long TotalNumDecalsVisible;
    long TotalNumDecalsAllocated;
    long TotalNumParticleLightsVisible;
    long TotalNumParticleLightsActive;
};

struct CParticleEmitterList {
    CRetailList<CEmitterPointer> Active;             // +0x00
    long                         MaxActiveEmitters;  // +0x04
    long size() const
    {
        long n = 0;
        for (CRetailList<CEmitterPointer>::iterator first = Active.begin(), last = Active.end(); first != last; ) {
            ++first;
            ++n;
        }
        return n;
    }
};

struct CParticleEmitterManager {
    CParticleEmitterList ActiveEmitters;             // +0x0000
    unsigned char        PerlinNoise[0x1000];        // +0x0008
    long                 MaxVisibleEmitters;         // +0x1008
    long                 VisibleEmitterCount;        // +0x100c
    void GetParticleStats(CParticleStats* stats);
};

void CParticleEmitterManager::GetParticleStats(CParticleStats* stats)
{
    stats->EmittersActive = ActiveEmitters.size();
    stats->EmittersVisible = VisibleEmitterCount;
    for (CRetailList<CEmitterPointer>::iterator it = ActiveEmitters.Active.begin(); it != ActiveEmitters.Active.end(); ++it) {
        if ((*it)->IsSleeping())
            ++stats->EmittersSleeping;
        (*it)->GetParticleStats(stats);
    }
}