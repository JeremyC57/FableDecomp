// Behaviour test for 00af5380: src/00af5380.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NParticleEngine::CParticleSystem::AllocateDecal()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NParticleEngine::CParticleSystem::AllocateDecal() -- take a decal from the particle memory (reclaiming the decals of the lowest-priority invisible emitters once if it is exhausted), construct it, append it to the system's decal list and count it; null when no memory can be found
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF5380, length 176 bytes (manifest label correct).
// Callees: the particle memory allocator @ 0x00AF8B00 (__fastcall size; label "operator_new" -- CDecal's class operator
//          new), CParticleEmitterManager::Get @ 0x00AEAAF0, CParticleEmitterList::ClearLowestPriorityNonVisibleEmitterDecals
//          (CIEngine*) @ 0x00AEAF80 (the manager's list at +0, its engine word at +0x1014), malloc @ 0x00BFEA0E (list node).
// Layout: Ego_r.pdb NParticleEngine::CDecal (0x7c; retail 0x74 with 12-byte CArrays), vtable 0x0129F4FC: SizeFactor[2]
// +0x38, AlphaFactor[2] +0x40, GraphicFrame +0x48, UpdateTimer +0x4c, MinLife +0x50, flag byte +0x58 (bit 1
// SizeCollisionThisFrame, bit 2 AlphaCollisionThisFrame), PrimIDList +0x5c, ParentHandleList +0x68. The retail
// CParticleSystem keeps its STLport decal list head at +0x20 and NoActiveDecals at +0x4c.
// Shape: the raw block is fetched (retrying once after the reclaim) and the decal is built with placement new, whose
// null test folds into the allocation tests; the list append is the inline STLport push_back on a malloc'd node.
#include <new>

extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E

class CIEngine;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

template<class T> class CArray { public:                  // retail CArray<T>: three pointers
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    CArray() : m_pData(0), m_pLast(0), m_pEnd(0) {}
};
class CEnginePrimitiveHandle;

class CParticleEmitterList {
public:
    void ClearLowestPriorityNonVisibleEmitterDecals(CIEngine* engine);   // 0x00AEAF80
};
class CParticleEmitterManager {
public:
    CParticleEmitterList ActiveEmitters;           // +0x0000
    unsigned char        _pad[0x1010];
    CIEngine*            PEngine;                  // +0x1014
    static CParticleEmitterManager* Get();         // 0x00AEAAF0
};

void* __fastcall AllocateParticleMemory_00af8b00(unsigned int size);   // 0x00AF8B00

namespace NParticleEngine {
class CDecal {                                     // vtable 0x0129F4FC
public:
    C3DVector         Position;                    // +0x04
    CRightHandedSet   RHSet;                       // +0x10
    long              Lifetimer;                   // +0x28
    float             CreationTime;                // +0x2c
    long              MaxLife;                     // +0x30
    unsigned char     FlickerIndex;                // +0x34
    unsigned char     FlickerTimeOffset;           // +0x35
    float             SizeFactor[2];               // +0x38
    float             AlphaFactor[2];              // +0x40
    float             GraphicFrame;                // +0x48
    long              UpdateTimer;                 // +0x4c
    float             MinLife;                     // +0x50
    float             MinSize;                     // +0x54
    bool              Active : 1;                  // +0x58
    bool              SizeCollisionThisFrame : 1;
    bool              AlphaCollisionThisFrame : 1;
    bool              LandscapeOnlyDecal : 1;
    CArray<CArray<long> >                  PrimIDList;         // +0x5c
    CArray<CEnginePrimitiveHandle>         ParentHandleList;   // +0x68
    CDecal()
        : GraphicFrame(0)
        , UpdateTimer(0)
    {
        SizeCollisionThisFrame = false;
        AlphaCollisionThisFrame = false;
        MinLife = 0;
        SizeFactor[0] = 1.0f;
        SizeFactor[1] = 1.0f;
        AlphaFactor[0] = 1.0f;
        AlphaFactor[1] = 1.0f;
    }
    static void* operator new(unsigned int size) { return AllocateParticleMemory_00af8b00(size); }
    virtual void v00();
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};

template <class T> struct CRetailList {            // STLport list: one word, the sentinel node
    struct iterator {
        SListNode<T>* _M_node;
        iterator(SListNode<T>* n) : _M_node(n) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
    };
    SListNode<T>* _M_node;
    iterator end() { return iterator(_M_node); }
    SListNode<T>* _M_create_node(const T& x)
    {
        SListNode<T>* node = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        new (&node->_M_data) T(x);
        return node;
    }
    void insert(iterator position, const T& x)
    {
        SListNode<T>* node = _M_create_node(x);
        SListNode<T>* n = position._M_node;
        SListNode<T>* previous = n->_M_prev;
        node->_M_next = n;
        node->_M_prev = previous;
        previous->_M_next = node;
        n->_M_prev = node;
    }
    void push_back(const T& x) { insert(end(), x); }
};

class CParticleSystem {
public:
    void*                m_vtable;                 // +0x00
    void*                PParentEmitter;           // +0x04
    CIEngine*            PEngine;                  // +0x08
    unsigned char        Components[0x10];         // +0x0c
    CRetailList<void*>   Particles;                // +0x1c
    CRetailList<CDecal*> Decals;                   // +0x20
    unsigned char        UpdateParameters[0x24];   // +0x24
    long                 NoActiveParticles;        // +0x48
    long                 NoActiveDecals;           // +0x4c
    CDecal* AllocateDecal();
};

CDecal* CParticleSystem::AllocateDecal()
{
    CDecal* decal = new CDecal;
    if (decal == 0) {
        CParticleEmitterManager* manager = CParticleEmitterManager::Get();
        manager->ActiveEmitters.ClearLowestPriorityNonVisibleEmitterDecals(manager->PEngine);
        decal = new CDecal;
    }
    if (decal) {
        Decals.push_back(decal);
        ++NoActiveDecals;
    }
    return decal;
}
}

// ---- stubs and driver ----
static int g_allocs = 0, g_failFirst = 0, g_failAll = 0;
static unsigned char g_mem[4][0x80];
void* __fastcall AllocateParticleMemory_00af8b00(unsigned int size)
{
    CHECK(size == sizeof(NParticleEngine::CDecal));
    int i = g_allocs++;
    if (g_failAll) return 0;
    if (g_failFirst && i == 0) return 0;
    return g_mem[i & 3];
}
static int g_clears = 0; static CIEngine* g_clearEngine;
void CParticleEmitterList::ClearLowestPriorityNonVisibleEmitterDecals(CIEngine* engine) { ++g_clears; g_clearEngine = engine; }
static CParticleEmitterManager* g_manager;
CParticleEmitterManager* CParticleEmitterManager::Get() { return g_manager; }
void NParticleEngine::CDecal::v00() {}
int main()
{
    static CParticleEmitterManager mgr; mgr.PEngine = (CIEngine*)0x4444; g_manager = &mgr;
    static NParticleEngine::SListNode<NParticleEngine::CDecal*> head; head._M_next = &head; head._M_prev = &head;
    NParticleEngine::CParticleSystem sys; memset(&sys, 0, sizeof sys); sys.Decals._M_node = &head; sys.NoActiveDecals = 5;
    NParticleEngine::CDecal* d = sys.AllocateDecal();
    CHECK(d == (NParticleEngine::CDecal*)g_mem[0]); CHECK(g_clears == 0); CHECK(sys.NoActiveDecals == 6);
    CHECK(head._M_next != &head); CHECK(head._M_next->_M_data == d); CHECK(head._M_prev == head._M_next);
    CHECK(d->GraphicFrame == 0.0f); CHECK(d->UpdateTimer == 0); CHECK(d->SizeFactor[0] == 1.0f); CHECK(d->AlphaFactor[1] == 1.0f); CHECK(d->MinLife == 0.0f);
    CHECK(d->SizeCollisionThisFrame == false); CHECK(d->PrimIDList.m_pData == 0);
    g_allocs = 0; g_failFirst = 1;
    NParticleEngine::CDecal* d2 = sys.AllocateDecal();
    CHECK(d2 == (NParticleEngine::CDecal*)g_mem[1]); CHECK(g_clears == 1); CHECK(g_clearEngine == (CIEngine*)0x4444); CHECK(sys.NoActiveDecals == 7);
    CHECK(head._M_prev->_M_data == d2);
    g_allocs = 0; g_failFirst = 0; g_failAll = 1;
    CHECK(sys.AllocateDecal() == 0); CHECK(g_clears == 2); CHECK(sys.NoActiveDecals == 7); CHECK(g_allocs == 2);
    if (fails == 0) printf("00af5380_TEST PASS\n"); else printf("00af5380_TEST FAIL\n");
    return fails;
}