// Behaviour test for 00af5010: src/00af5010.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NParticleEngine::CParticleSystem::AllocateParticle()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
struct CIEngine;
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
struct CParticle {
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
struct CParticleSystem {
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

// ---- stubs and driver ----

#include <stdlib.h>
static int g_news = 0, g_failFirst = 0, g_failAll = 0, g_ctors = 0;
static unsigned char g_pool[3][0x9c];
void* __fastcall NParticleEngine::CParticle::operator new(unsigned int size)
{
    CHECK(size == 0x9c);
    int i = g_news++;
    if (g_failAll || (g_failFirst && i == 0)) return 0;
    return g_pool[i % 3];
}
NParticleEngine::CParticle::CParticle() { ++g_ctors; _particle[0] = 0x5a; }
static int g_clears = 0; static CIEngine* g_engine;
void CParticleEmitterList::ClearLowestPriorityNonVisibleEmitterParticles(CIEngine* engine) { ++g_clears; g_engine = engine; }
static CParticleEmitterManager g_mgr;
CParticleEmitterManager* GetParticleEmitterManager() { return &g_mgr; }
using namespace NParticleEngine;
int main()
{
    g_mgr.PEngine = (CIEngine*)0x9090;
    _List_node head; head._M_next = &head; head._M_prev = &head;
    CParticleSystem sys; memset(&sys, 0, sizeof sys); sys.Particles._M_node = &head; sys.NoActiveParticles = 1;
    CParticle* p = sys.AllocateParticle();
    CHECK(p == (CParticle*)g_pool[0]); CHECK(g_ctors == 1); CHECK(p->_particle[0] == 0x5a); CHECK(g_clears == 0);
    CHECK(head._M_next->_M_data == p); CHECK(head._M_prev == head._M_next); CHECK(sys.NoActiveParticles == 2);
    g_news = 0; g_failFirst = 1;
    CParticle* q = sys.AllocateParticle();
    CHECK(q == (CParticle*)g_pool[1]); CHECK(g_clears == 1); CHECK(g_engine == (CIEngine*)0x9090); CHECK(head._M_prev->_M_data == q);
    CHECK(head._M_next->_M_next == head._M_prev); CHECK(sys.NoActiveParticles == 3);
    g_news = 0; g_failFirst = 0; g_failAll = 1;
    CHECK(sys.AllocateParticle() == 0); CHECK(g_clears == 2); CHECK(sys.NoActiveParticles == 3); CHECK(g_ctors == 2);
    if (fails == 0) printf("00af5010_TEST PASS\n"); else printf("00af5010_TEST FAIL\n");
    return fails;
}