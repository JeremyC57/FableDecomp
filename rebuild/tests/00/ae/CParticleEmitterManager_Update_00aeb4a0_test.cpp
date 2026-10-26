#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#define free test_free
#define malloc test_malloc
// Behaviour test for 00aeb4a0: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// CParticleEmitterManager::Update(bool) -- walk the active emitters: drop (and release) the dead ones, initialise any not yet initialised, and update those that always update (or all when asked) and are visible or update offscreen
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEB4A0, length 176 bytes (`ret 4`).
// Callees: CParticleEmitter::Initialise(CIEngine*) @ 0x00AEE240 (with null), IsVisible @ 0x00AED010, Update @ 0x00AEE9F0;
//          free @ 0x00BFEA14 (list node), operator delete @ 0x00BFE9BC (count block).
// Layout: the retail manager starts with the one-word STLport list of CCountedPointer<CParticleEmitter> (as the
// landed GetParticleStats 0x00AEB000). Node: next +0, prev +4, emitter +8, count block +0xc.
// Shape: erase() is inline -- unlink, the counted pointer's destructor (release, then both words nulled), free;
// the emitter pointer read for the Dead test stays in ecx for the Initialise call, later tests re-read it.
extern "C" void __cdecl free(void* block);           // 0x00BFEA14 (the list node allocator's deallocate)
void __cdecl operator delete(void* block);           // 0x00BFE9BC (count blocks)

class CIEngine;
struct CInterpolationInfoSet {
    float GTPredictedTimeSinceLastRenderFrame;       // +0x00
    float WFInterpolate;                             // +0x04
};
class CInterpolationInfo { public:
    CInterpolationInfoSet Current;                   // +0x00
    CInterpolationInfoSet Last;                      // +0x08
    CInterpolationInfoSet PausedCurrent;             // +0x10
    CInterpolationInfoSet PausedLast;                // +0x18
    CInterpolationInfoSet BulletTimeCurrent;         // +0x20
    CInterpolationInfoSet BulletTimeLast;            // +0x28
    long                  WFServerCurrentTime;       // +0x30
};
class CEnginePrimitivePrepareInfo { public:
    CInterpolationInfo Interpolate;                  // +0x00
    CIEngine*          Engine;                       // +0x34
};

namespace NParticleEngine {
// CParticleEmitter at retail offsets (as the landed GetParticleStats 0x00AEB000): the ushort flag word at +0x40,
// the uchar flag byte at +0x44 (PDB +0x4c / +0x50).
class CParticleEmitter { public:
    void*          m_vtable;                         // +0x00
    unsigned char  m_base04[0x3c];                   // +0x04 .. +0x3f
    unsigned short Priority : 3;                     // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;                         // bit 5
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;                  // bit 8 (+0x41 bit 0)
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    unsigned short Emitter2DLayer : 7;               // +0x42
    unsigned char  PreWaterEffect : 1;               // +0x44
    unsigned char  WaterEffect : 1;
    unsigned char  ZBufferWriteable : 1;
    unsigned char  Visible : 1;
    unsigned char  AlwaysUpdate : 1;                 // 0x10
    unsigned char  DieIfOffsceen : 1;
    unsigned char  OffscreenUpdate : 1;              // 0x40
    unsigned char  ClipEffectToWeatherMask : 1;
    bool IsDead() const { return Dead; }
    bool IsInitialised() const { return Initialised; }
    bool Initialise(CIEngine* engine);                                          // 0x00AEE240
    bool IsVisible() const;                                                     // 0x00AED010 (labelled "CTCPhysicsStandard::CanCollideWithLandscape")
    void Update();                                                              // 0x00AEE9F0
    void StartFrame(float interpolate, CEnginePrimitivePrepareInfo* info);     // 0x00AEEBA0
};

struct CCountedItem { };
struct CCPPointerInfo {                              // the shared count block
    long m_count;                                    // +0x00
    void (CCountedItem::*m_pfnDestroy)();            // +0x04 (called with the item as `this`)
    CCountedItem* m_pItem;                           // +0x08
};
template <class T> struct CCountedPointer {
    T*              Object;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            if (--Info->m_count == 0) {
                (Info->m_pItem->*Info->m_pfnDestroy)();
                operator delete(Info);
            }
        }
        Object = 0;
        Info = 0;
    }
    ~CCountedPointer() { Release(); }
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
        T* operator->() const { return &_M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    SListNode<T>* _M_node;
    iterator begin() const { return iterator(_M_node->_M_next); }
    iterator end() const { return iterator(_M_node); }
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

typedef NParticleEngine::CCountedPointer<NParticleEngine::CParticleEmitter> CEmitterPointer;

struct CParticleEmitterList {
    CRetailList<CEmitterPointer> Active;             // +0x00
    long                         MaxActiveEmitters;  // +0x04
};

struct CParticleEmitterManager {
    CParticleEmitterList ActiveEmitters;             // +0x0000
    void Update(bool updateAll);
};

void CParticleEmitterManager::Update(bool updateAll)
{
    CRetailList<CEmitterPointer>::iterator it = ActiveEmitters.Active.begin();
    while (it != ActiveEmitters.Active.end()) {
        NParticleEngine::CParticleEmitter* emitter = it->Object;
        if (emitter->IsDead()) {
            it = ActiveEmitters.Active.erase(it);
            continue;
        }
        if (!emitter->IsInitialised())
            emitter->Initialise(0);
        if (updateAll || (*it)->AlwaysUpdate) {
            if ((*it)->IsVisible() || (*it)->OffscreenUpdate)
                (*it)->Update();
        }
        ++it;
    }
}

// ---- stubs and driver ----
static char g_log[512];
static char g_num[64];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
static bool g_armed = false;
static int g_frees = 0, g_deletes = 0, g_destroys = 0;
extern "C" void test_free(void* p) { if (g_armed) ++g_frees; }
void __cdecl operator delete(void* p) { if (g_armed) ++g_deletes; }
using NParticleEngine::CParticleEmitter;
struct CItem : NParticleEngine::CCountedItem { void Destroy() { ++g_destroys; } };
typedef SListNode<CEmitterPointer> Node;
static Node g_head, g_nodes[4];
static CParticleEmitter g_em[4];
static NParticleEngine::CCPPointerInfo g_blocks[4];
static CItem g_items[4];
static void Build(CRetailList<CEmitterPointer>& list, int n)
{
    list._M_node = &g_head;
    Node* prev = &g_head;
    for (int i = 0; i < n; ++i) {
        std::memset((void*)&g_em[i], 0, sizeof(g_em[i]));
        g_blocks[i].m_count = 1;
        g_blocks[i].m_pfnDestroy = static_cast<void (NParticleEngine::CCountedItem::*)()>(&CItem::Destroy);
        g_blocks[i].m_pItem = &g_items[i];
        g_nodes[i]._M_data.Object = &g_em[i];
        g_nodes[i]._M_data.Info = &g_blocks[i];
        prev->_M_next = &g_nodes[i]; g_nodes[i]._M_prev = prev; prev = &g_nodes[i];
    }
    prev->_M_next = &g_head; g_head._M_prev = prev;
}
static int Index(CParticleEmitter* e) { return (int)(e - g_em); }
static int Count(CRetailList<CEmitterPointer>& list) { int n = 0; for (Node* p = list._M_node->_M_next; p != list._M_node; p = p->_M_next) ++n; return n; }
bool CParticleEmitter::Initialise(CIEngine* e) { std::sprintf(g_num, "I%d%s", Index(this), e ? "x" : "0"); Log(g_num); return true; }
static bool g_visible[4];
bool CParticleEmitter::IsVisible() const { return g_visible[(int)(this - g_em)]; }
void CParticleEmitter::Update() { std::sprintf(g_num, "U%d", Index(this)); Log(g_num); }
int main()
{
    CParticleEmitterManager m;
    Build(m.ActiveEmitters.Active, 4);
    g_em[0].Initialised = 1; g_em[0].AlwaysUpdate = 1; g_visible[0] = true;    // updated
    g_em[1].Dead = 1;                                                           // removed
    g_em[2].Initialised = 0; g_em[2].AlwaysUpdate = 1; g_em[2].OffscreenUpdate = 1;   // initialised + updated offscreen
    g_em[3].Initialised = 1;                                                    // not always-update: skipped unless updateAll
    g_armed = true;
    m.Update(false);
    g_armed = false;
    bool ok = std::strcmp(g_log, "U0,I20,U2,") == 0 && Count(m.ActiveEmitters.Active) == 3 && g_frees == 1 &&
              g_deletes == 1 && g_destroys == 1 && g_nodes[1]._M_data.Object == 0 && g_nodes[1]._M_data.Info == 0;
    g_log[0] = 0; g_visible[3] = true;
    m.Update(true);
    ok = ok && std::strcmp(g_log, "U0,I20,U2,U3,") == 0;
    if (!ok) { std::printf("00aeb4a0_TEST FAIL\n"); return 1; }
    std::printf("00aeb4a0_TEST PASS\n");
    return 0;
}