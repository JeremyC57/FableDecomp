#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/00b0c830.cpp) ----
// NParticleEngine::CPSCUpdateNormal::GetParticleCount() -- vtable slot 26: the number of live particles in the decal emitters this component spawned (dead or uninitialised emitters skipped)
// Leaf: ?GetParticleCount@CPSCUpdateNormal@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0C830, 97 bytes (+15 int3 padding).
// Name: FableWin.pdb ?GetParticleCount@CPSCUpdateNormal@NParticleEngine@@UAEJXZ; slot 26 of the vtable 0x0129FC1C (rtti-vtables.tsv;
// CPSCBase declares GetParticleCount then GetDecalCount, slots 26/27; the twin rows 0x00B0C830/0x00B0C8A0 differ only
// in the per-emitter callee).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (four times per element), CParticleEmitter::GetNoParticles
// @ 0x00AEDCA0 (landed under the global class name ::CParticleEmitter; kept so the address binds one symbol).
// Layout: the landed KillAllDecalEmitters 0x00B0C730: DecalEmitterList (STLport list head node) at +0x9C, node
// {next, prev, CIntelligentPointer (vptr, item)}; the emitter flags Dead (+0x40 bit 5), Initialised (+0x41 bit 0).
// Shape: `for (it = begin(); it != end(); ++it)` with end() re-read each pass (`cmp edi,[ebp+9Ch]`); each
// test re-fetches the item through the out-of-line GetPItem; the flags are read through the PDB's inline
// `bool IsDead() const` / `IsInitialised() const` (the bool return gives `mov al ; shr al,5 ; test al,1`; a direct
// bit-field test is `test byte ptr [eax+40h],20h`); the sum lives in ebx. Plain `ret`.
struct CParticleEmitter {                   // NParticleEngine::CParticleEmitter (landed callee spelling)
    unsigned char  _pad00[0x40];
    unsigned short Priority : 3;            // +0x40 bits 0-2
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;                // bit 5
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;         // +0x41 bit 0
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    bool IsDead() const { return Dead; }
    bool IsInitialised() const { return Initialised; }
    long GetNoParticles();                         // 0x00AEDCA0
};

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    void* m_pItem;                          // +0x04
    void* GetPItem() const;                 // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual ~CIntelligentPointer();
    T* Get() const { return (T*)GetPItem(); }
};
template <class T> struct SListNode {       // STLport _List_node
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CRetailList {     // STLport list (4 bytes: the head node)
    SListNode<T>* _M_data;
    struct iterator {
        SListNode<T>* _M_node;
        iterator(SListNode<T>* n) : _M_node(n) {}
        T* operator->() const { return &_M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    iterator begin() const { return iterator(_M_data->_M_next); }
    iterator end() const { return iterator(_M_data); }
};
typedef CIntelligentPointer<CParticleEmitter> CEmitterPointer;

namespace NParticleEngine {
struct CPSCUpdateNormal {
    virtual void v00();
    unsigned char                _pad04[0x9c - 4];
    CRetailList<CEmitterPointer> DecalEmitterList;     // +0x9c
    virtual long GetParticleCount();
};

long CPSCUpdateNormal::GetParticleCount()
{
    long count = 0;
    for (CRetailList<CEmitterPointer>::iterator it = DecalEmitterList.begin(); it != DecalEmitterList.end(); ++it)
    {
        if (it->Get() && !it->Get()->IsDead() && it->Get()->IsInitialised())
            count += it->Get()->GetNoParticles();
    }
    return count;
}
} // namespace NParticleEngine

// ---- stubs for the external callees ----

CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
template <class T> CIntelligentPointer<T>::~CIntelligentPointer() {}
void* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void NParticleEngine::CPSCUpdateNormal::v00() {}
static CParticleEmitter g_pool[4];
long CParticleEmitter::GetNoParticles() { return (long)(this - g_pool) + 1; }

static void run()
{

    // emitters 0 and 3 are live and initialised (counts 1 and 4); 1 is dead, 2 not initialised; one node is empty
    g_pool[0].Initialised = 1;
    g_pool[1].Initialised = 1; g_pool[1].Dead = 1;
    g_pool[3].Initialised = 1;
    SListNode<CEmitterPointer> head;
    SListNode<CEmitterPointer> nodes[5];
    void* items[5];
    items[0] = &g_pool[0]; items[1] = &g_pool[1]; items[2] = 0; items[3] = &g_pool[2]; items[4] = &g_pool[3];
    head._M_next = &nodes[0]; head._M_prev = &nodes[4]; head._M_data.m_pItem = 0;
    int i = 0;
    while (i < 5) {
        nodes[i]._M_next = i < 4 ? &nodes[i + 1] : &head;
        nodes[i]._M_prev = i > 0 ? &nodes[i - 1] : &head;
        nodes[i]._M_data.m_pItem = items[i];
        ++i;
    }
    NParticleEngine::CPSCUpdateNormal u;
    u.DecalEmitterList._M_data = &head;
    CHECK(u.GetParticleCount() == 5);
    SListNode<CEmitterPointer> empty;
    empty._M_next = &empty; empty._M_prev = &empty; empty._M_data.m_pItem = 0;
    u.DecalEmitterList._M_data = &empty;
    CHECK(u.GetParticleCount() == 0);
}

int main()
{
    run();
    if (!g_ok) { std::printf("00b0c830_TEST FAIL\n"); return 1; }
    std::printf("00b0c830_TEST PASS\n");
    return 0;
}