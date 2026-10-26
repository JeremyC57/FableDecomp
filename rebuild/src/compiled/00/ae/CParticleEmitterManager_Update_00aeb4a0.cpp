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