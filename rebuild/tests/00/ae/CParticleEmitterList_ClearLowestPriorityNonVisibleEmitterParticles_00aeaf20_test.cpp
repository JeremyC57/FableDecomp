// Behaviour of CParticleEmitterList::ClearLowestPriorityNonVisibleEmitterParticles (0x00AEAF20): from the back of the active
// list, the first initialised, non-visible emitter that has Particles allocated is cleared; nothing else is.
#include <cstdio>
struct CIEngine { int id; };
struct CParticleStats;
struct CParticleEmitter {
    unsigned short Priority : 3; unsigned short IsScreenDisplacement : 1; unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1; unsigned short ParticleCreationDisabled : 1; unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1; unsigned short WaitingToSleep : 1; unsigned short Sleeping : 1; unsigned short Emitter2D : 1;
    bool m_visible; bool m_hasParticles; bool m_hasDecals; int m_clearedParticles; int m_clearedDecals; CIEngine* m_engine; int m_stats;
    bool IsSleeping() const { return Sleeping; }
    bool IsVisible() const { return m_visible; }
    bool HasAllocatedParticles() const { return m_hasParticles; }
    bool HasAllocatedDecals() const { return m_hasDecals; }
    void ClearAllParticles(CIEngine* engine) { ++m_clearedParticles; m_engine = engine; }
    void ClearAllDecals(CIEngine* engine) { ++m_clearedDecals; m_engine = engine; }
    void GetParticleStats(CParticleStats*) { ++m_stats; }
    void Reset(bool init, bool vis, bool parts, bool decals)
    {
        Priority = 0; IsScreenDisplacement = 0; ReadZBuffer = 0; Dead = 0; ParticleCreationDisabled = 0; ContinuousEmitter = 0;
        Initialised = init; WaitingToSleep = 0; Sleeping = 0; Emitter2D = 0;
        m_visible = vis; m_hasParticles = parts; m_hasDecals = decals; m_clearedParticles = 0; m_clearedDecals = 0; m_engine = 0; m_stats = 0;
    }
};
struct SCountBlock;
struct CEmitterPointer { CParticleEmitter* Object; SCountBlock* Block; CParticleEmitter* operator->() const { return Object; } };
template <class T> struct SListNode { SListNode* _M_next; SListNode* _M_prev; T _M_data; };
template <class T> struct CRetailList {
    struct iterator {
        SListNode<T>* _M_node;
        iterator(SListNode<T>* n) : _M_node(n) {}
        T& operator*() const { return _M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    struct reverse_iterator {
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
static CParticleEmitter g_em[3];
static SListNode<CEmitterPointer> g_head, g_n[3];
static void Link()
{
    SListNode<CEmitterPointer>* prev = &g_head;
    for (int i = 0; i < 3; ++i) { prev->_M_next = &g_n[i]; g_n[i]._M_prev = prev; g_n[i]._M_data.Object = &g_em[i]; g_n[i]._M_data.Block = 0; prev = &g_n[i]; }
    prev->_M_next = &g_head; g_head._M_prev = prev;
}
struct CParticleEmitterList {
    CRetailList<CEmitterPointer> Active;
    long MaxActiveEmitters;
    void ClearLowestPriorityNonVisibleEmitterParticles(CIEngine* engine)
    {
        for (CRetailList<CEmitterPointer>::reverse_iterator it = Active.rbegin(); it != Active.rend(); ++it) {
            if ((*it)->Initialised && !(*it)->IsVisible() && (*it)->HasAllocatedParticles()) {
                (*it)->ClearAllParticles(engine);
                return;
            }
        }
    }
};
int main()
{
    Link(); CIEngine e;
    g_em[0].Reset(true, false, true, true);    // eligible (front)
    g_em[1].Reset(true, false, true, true);    // eligible: the last eligible from the back
    g_em[2].Reset(false, false, true, true);   // not initialised
    CParticleEmitterList l; l.Active._M_node = &g_head;
    l.ClearLowestPriorityNonVisibleEmitterParticles(&e);
    if (g_em[1].m_clearedParticles != 1 || g_em[1].m_engine != &e || g_em[0].m_clearedParticles != 0 || g_em[2].m_clearedParticles != 0 || g_em[1].m_clearedDecals != 0) { std::printf("00aeaf20_TEST FAIL\n"); return 1; }
    g_em[1].Reset(true, true, true, true);     // visible now
    l.ClearLowestPriorityNonVisibleEmitterParticles(&e);
    if (g_em[0].m_clearedParticles != 1 || g_em[1].m_clearedParticles != 0) { std::printf("00aeaf20_TEST FAIL\n"); return 1; }
    g_em[0].Reset(true, false, false, false);  // nothing allocated
    l.ClearLowestPriorityNonVisibleEmitterParticles(&e);
    if (g_em[0].m_clearedParticles != 0 || g_em[1].m_clearedParticles != 0 || g_em[2].m_clearedParticles != 0) { std::printf("00aeaf20_TEST FAIL\n"); return 1; }
    std::printf("00aeaf20_TEST PASS\n");
    return 0;
}