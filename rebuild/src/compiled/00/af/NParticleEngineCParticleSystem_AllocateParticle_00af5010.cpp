// NParticleEngine::CParticleSystem::AllocateParticle() -- new particle from the particle pool; when the pool is exhausted, clear the lowest-priority invisible emitter's particles and retry; append it to the particle list and count it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF5010, length 128 bytes.
// Leaf: AllocateParticle
// Callees: CParticle's class operator new @ 0x00AF86C0 (`__fastcall(size)`, the particle pool), CParticle::CParticle @ 0x00AF7A90,
// the particle-emitter manager getter @ 0x00AEAAF0 (manifest "GFGetPreMainMemoryUsed" is propagated),
// CParticleEmitterList::ClearLowestPriorityNonVisibleEmitterParticles(CIEngine*) @ 0x00AEAF20 (on the manager's list at +0 with
// its engine pointer at +0x1014), malloc @ 0x00BFEA0E (the 12-byte list node).
// Layout: Particles (STLport list<CParticle*>, one header word) +0x1c, NoActiveParticles +0x48 (retail CParticleSystem).
// Shape: `new CParticle` twice (pool exhausted -> clear the lowest-priority invisible emitter's particles, retry); the list
// append is STLport insert(end(), x) with end() evaluated before _M_create_node, placement-new of the data under a null test,
// and the old prev read before the node's links are stored (named `prev` local).
// Tried: `return 0` vs an `if (particle)` guard around the append (same code), STLport link order vs the prev local (this file).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CIEngine;
inline void* operator new(unsigned int, void* where) { return where; }
struct CParticleEmitterList {
    void* m_activeHeader;                   // +0x00 (STLport list header)
    void ClearLowestPriorityNonVisibleEmitterParticles(CIEngine* engine);   // 0x00AEAF20
};
struct CParticleEmitterManager {
    CParticleEmitterList ActiveEmitters;    // +0x0000
    unsigned char        _body[0x1010];     // +0x0004
    CIEngine*            PEngine;           // +0x1014
};
CParticleEmitterManager* GetParticleEmitterManager();   // 0x00AEAAF0
namespace NParticleEngine {
class CParticle { public:
    unsigned char _particle[0x9c];
    CParticle();                            // 0x00AF7A90
    static void* __fastcall operator new(unsigned int size);   // 0x00AF86C0 (particle pool)
};
struct _List_node {
    _List_node* _M_next;                    // +0
    _List_node* _M_prev;                    // +4
    CParticle*  _M_data;                    // +8
};
struct _List_iterator {
    _List_node* _M_node;
    _List_iterator(_List_node* n) : _M_node(n) {}
    _List_iterator(const _List_iterator& o) : _M_node(o._M_node) {}
};
struct CParticleList {                      // STLport list<CParticle*> (one header word)
    _List_node* _M_node;
    _List_iterator end() { return _List_iterator(_M_node); }
    _List_node* _M_create_node(CParticle* const& x)
    {
        _List_node* tmp = (_List_node*)malloc(sizeof(_List_node));
        new (&tmp->_M_data) CParticle*(x);
        return tmp;
    }
    void insert(_List_iterator position, CParticle* const& x)
    {
        _List_node* tmp = _M_create_node(x);
        _List_node* n = position._M_node;
        _List_node* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
    }
    void push_back(CParticle* const& x) { insert(end(), x); }
};
class CParticleSystem { public:
    unsigned char _system[0x1c];
    CParticleList Particles;                // +0x1c
    unsigned char _system2[0x28];
    long          NoActiveParticles;        // +0x48
    CParticle* AllocateParticle();
};
CParticle* CParticleSystem::AllocateParticle()
{
    CParticle* particle = new CParticle;
    if (!particle) {
        CParticleEmitterManager* manager = GetParticleEmitterManager();
        manager->ActiveEmitters.ClearLowestPriorityNonVisibleEmitterParticles(manager->PEngine);
        particle = new CParticle;
    }
    if (particle) {
        Particles.push_back(particle);
        ++NoActiveParticles;
    }
    return particle;
}
}