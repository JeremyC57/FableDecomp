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
// ---- the candidate model (copied from src/00b0c910.cpp) ----
// NParticleEngine::CPSCUpdateNormal::PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system, const CParticleUpdateParameters& params, CEnginePrimitiveHandle& handle) -- vtable slot 13: drop dead decal emitters from the list, initialise any new ones against the render engine, then start the frame and prepare the primitives of each live one
// Leaf: ?PreparePrimitives@CPSCUpdateNormal@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0C910, 174 bytes (+2 int3 padding).
// Name: FableWin.pdb ?PreparePrimitives@CPSCUpdateNormal@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z;
// slot 13 of the vtable 0x0129FC1C (rtti-vtables.tsv; five stack arguments, `ret 14h`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the intelligent pointer's virtual destructor (slot 0, flag 0),
// free @ 0x00BFEA14 (the list node), CParticleEmitter::Initialise(CIEngine*) @ 0x00AEE240,
// CParticleEmitter::StartFrame(float, CEnginePrimitivePrepareInfo*) @ 0x00AEEBA0,
// CParticleEmitter::PreparePrimitives(float, const CEnginePrimitivePrepareInfo*) @ 0x00AEE560 (all three landed with
// pointer parameters; StartFrame's is non-const, so the info is passed through a const_cast).
// Layout: the landed KillAllDecalEmitters 0x00B0C730: DecalEmitterList (STLport list head node) +0x9C, node {next,
// prev, CIntelligentPointer}; the emitter flags Dead (+0x40 bit 5) and Initialised (+0x41 bit 0) read through the
// PDB's inline IsDead() / IsInitialised(); CEnginePrimitivePrepareInfo::Engine at +0x34 (after the 0x34-byte
// CInterpolationInfo, as the landed CView3DBase::Render model).
// Shape: an erase loop `it = dead ? DecalEmitterList.erase(it) : (work, ++it)` with end() re-read every pass
// (`this` homed in the `push ecx` slot); the inlined STLport erase unlinks the node (next in ebp), runs the
// element's virtual destructor and frees the node. The distance float is forwarded as a dword (ebp). `ret 14h`.

class CIEngine;
class CEnginePrimitiveHandle;
class CEnginePrimitivePrepareInfo { public:
    unsigned char _interpolate[0x34];       // CInterpolationInfo
    CIEngine*     Engine;                   // +0x34
};

namespace NParticleEngine {
template <class A, class B> class CParticleUpdateParameters;
class CParticleSystem;
class CParticleEmitter { public:
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
    bool Initialise(CIEngine* engine);                                                   // 0x00AEE240
    void StartFrame(float interpolate, CEnginePrimitivePrepareInfo* info);               // 0x00AEEBA0
    void PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo* info);     // 0x00AEE560
};
}

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    void* m_pItem;                          // +0x04
    void* GetPItem() const;                 // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
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
    iterator erase(iterator position)
    {
        SListNode<T>* next = position._M_node->_M_next;
        SListNode<T>* prev = position._M_node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        position._M_node->_M_data.~T();
        free(position._M_node);
        return iterator(next);
    }
};
typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> CEmitterPointer;

namespace NParticleEngine {
class CPSCUpdateNormal { public:
    virtual void v00();
    unsigned char                _pad04[0x9c - 4];
    CRetailList<CEmitterPointer> DecalEmitterList;     // +0x9c
    virtual void PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                   CEnginePrimitiveHandle& handle);
};

void CPSCUpdateNormal::PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                         const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                         CEnginePrimitiveHandle& handle)
{
    CRetailList<CEmitterPointer>::iterator it = DecalEmitterList.begin();
    while (it != DecalEmitterList.end()) {
        if (it->Get()->IsDead()) {
            it = DecalEmitterList.erase(it);
        } else {
            if (!it->Get()->IsInitialised())
                it->Get()->Initialise(info.Engine);
            it->Get()->StartFrame(distance, const_cast<CEnginePrimitivePrepareInfo*>(&info));
            it->Get()->PreparePrimitives(distance, &info);
            ++it;
        }
    }
}
} // namespace NParticleEngine

// ---- stubs for the external callees ----

CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
template <class T> CIntelligentPointer<T>::~CIntelligentPointer() {}
void* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void NParticleEngine::CPSCUpdateNormal::v00() {}
static NParticleEngine::CParticleEmitter g_pool[3];
static int g_initialised[3], g_started[3], g_prepared[3];
static CIEngine* g_engine = 0;
static int g_order = 0;
bool NParticleEngine::CParticleEmitter::Initialise(CIEngine* engine) { g_engine = engine; g_initialised[this - g_pool] = ++g_order; Initialised = 1; return true; }
void NParticleEngine::CParticleEmitter::StartFrame(float, CEnginePrimitivePrepareInfo*) { g_started[this - g_pool] = ++g_order; }
void NParticleEngine::CParticleEmitter::PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo*) { if (distance == 2.5f) g_prepared[this - g_pool] = ++g_order; }
static SListNode<CEmitterPointer>* NewNode(void* item)
{
    SListNode<CEmitterPointer>* n = (SListNode<CEmitterPointer>*)malloc(sizeof(SListNode<CEmitterPointer>));
    new (&n->_M_data) CEmitterPointer();
    n->_M_data.m_pItem = item;
    return n;
}

static void run()
{

    // emitter 0 live, 1 dead (erased), 2 live but not yet initialised
    g_pool[0].Initialised = 1;
    g_pool[1].Initialised = 1; g_pool[1].Dead = 1;
    SListNode<CEmitterPointer> head;
    head._M_data.m_pItem = 0;
    SListNode<CEmitterPointer>* n0 = NewNode(&g_pool[0]);
    SListNode<CEmitterPointer>* n1 = NewNode(&g_pool[1]);
    SListNode<CEmitterPointer>* n2 = NewNode(&g_pool[2]);
    head._M_next = n0; n0->_M_prev = &head;
    n0->_M_next = n1; n1->_M_prev = n0;
    n1->_M_next = n2; n2->_M_prev = n1;
    n2->_M_next = &head; head._M_prev = n2;
    int engineStorage = 0;
    CEnginePrimitivePrepareInfo info;
    info.Engine = reinterpret_cast<CIEngine*>(&engineStorage);
    int storage = 0;
    NParticleEngine::CParticleSystem& system = *reinterpret_cast<NParticleEngine::CParticleSystem*>(&storage);
    const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>& params =
        *reinterpret_cast<const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>*>(&storage);
    CEnginePrimitiveHandle& handle = *reinterpret_cast<CEnginePrimitiveHandle*>(&storage);
    NParticleEngine::CPSCUpdateNormal u;
    u.DecalEmitterList._M_data = &head;
    u.PreparePrimitives(2.5f, info, system, params, handle);
    CHECK(g_started[0] == 1 && g_prepared[0] == 2 && g_initialised[0] == 0);
    CHECK(g_started[1] == 0 && g_prepared[1] == 0);
    CHECK(g_initialised[2] == 3 && g_engine == info.Engine && g_started[2] == 4 && g_prepared[2] == 5);
    CHECK(head._M_next == n0 && n0->_M_next == n2 && n2->_M_prev == n0 && n2->_M_next == &head && head._M_prev == n2);
    n0->_M_data.~CEmitterPointer(); free(n0);
    n2->_M_data.~CEmitterPointer(); free(n2);
}

int main()
{
    run();
    if (!g_ok) { std::printf("00b0c910_TEST FAIL\n"); return 1; }
    std::printf("00b0c910_TEST PASS\n");
    return 0;
}