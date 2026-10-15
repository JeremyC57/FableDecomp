// Behaviour of CParticleEmitterManager::GetParticleStats (0x00AEB000): active count = list size, visible count copied,
// sleeping emitters counted, and every active emitter adds its own statistics.
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
struct CParticleStats { long EmittersVisible; long EmittersActive; long EmittersSleeping; long rest[12]; };
struct CParticleEmitterList {
    CRetailList<CEmitterPointer> Active;
    long MaxActiveEmitters;
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
    CParticleEmitterList ActiveEmitters;
    unsigned char PerlinNoise[0x1000];
    long MaxVisibleEmitters;
    long VisibleEmitterCount;
    void GetParticleStats(CParticleStats* stats)
    {
        stats->EmittersActive = ActiveEmitters.size();
        stats->EmittersVisible = VisibleEmitterCount;
        for (CRetailList<CEmitterPointer>::iterator it = ActiveEmitters.Active.begin(); it != ActiveEmitters.Active.end(); ++it) {
            if ((*it)->IsSleeping())
                ++stats->EmittersSleeping;
            (*it)->GetParticleStats(stats);
        }
    }
};
static CParticleEmitterManager g_m;
int main()
{
    Link();
    for (int i = 0; i < 3; ++i) g_em[i].Reset(true, false, false, false);
    g_em[0].Sleeping = 1; g_em[2].Sleeping = 1;
    g_m.ActiveEmitters.Active._M_node = &g_head; g_m.VisibleEmitterCount = 7;
    CParticleStats st; st.EmittersVisible = 0; st.EmittersActive = 0; st.EmittersSleeping = 1;
    g_m.GetParticleStats(&st);
    if (st.EmittersActive != 3 || st.EmittersVisible != 7 || st.EmittersSleeping != 3 || g_em[0].m_stats != 1 || g_em[1].m_stats != 1 || g_em[2].m_stats != 1) { std::printf("00aeb000_TEST FAIL\n"); return 1; }
    std::printf("00aeb000_TEST PASS\n");
    return 0;
}
