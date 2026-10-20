// NParticleEngine::CParticleEmitterManager::PreparePrimitives(const CEnginePrimitivePrepareInfo& info) -- with particles enabled: start the frame, collect the visible emitters, record their count and let each prepare its primitives (always-update emitters at the real interpolation time, the rest at the bullet-time one); with particles disabled: clear every active emitter's primitives once
// Leaf: ?PreparePrimitives@CParticleEmitterManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEB990, 242 bytes, `ret 4`. PDB: ?PreparePrimitives@CParticleEmitterManager@NParticleEngine@@QAEXABVCEnginePrimitivePrepareInfo@@@Z.
// Callees: CParticleEmitterManager::StartFrame 0x00AEB550, CParticleEmitterList::FindVisible(list&, long, const info&)
//   0x00AEB2D0, CParticleEmitter::PreparePrimitives(float, const info&) 0x00AEE560, CParticleEmitter::ClearAllPrimitives
//   (CIEngine*) 0x00AEE940, list<CCountedPointer<CParticleEmitter> >::clear 0x00AEC840 (catalogued as a propagated
//   remove_if), malloc 0x00BFEA0E / free 0x00BFEA14 (the STLport list head node). The enable flag is the byte global
//   0x0139950C (descriptive name).
// Layout: CEnginePrimitivePrepareInfo (Ego_r.pdb): Interpolate.Current.WFInterpolate +4, Interpolate.BulletTimeCurrent
//   .WFInterpolate +0x24, Engine +0x34. CParticleEmitterManager (retail, 8 bytes below the PDB after the 4-byte STLport list
//   in CParticleEmitterList): ActiveEmitters +0, PerlinNoise +8, MaxVisibleEmitters +0x1008, VisibleEmitterCount +0x100C,
//   Cleared / EnableDecals bits +0x1010. CParticleEmitter: AlwaysUpdate is bit 4 of the byte at retail +0x44 (PDB +0x50).
// Shape: stand-in same-TU callee bodies above (see there); the visible list is a local STLport list (malloc'd self-linked head) homed in the dead `info` argument slot; both
//   times are copied to locals before StartFrame; the time choice is an if/else of two complete PreparePrimitives calls
//   (VC merges them: `push info` hoisted, one push per arm, one call; a ternary argument goes through a frame temporary).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

namespace NParticleEngine {

class CIEngine;
struct CInterpolationInfoSet { float GTPredictedTimeSinceLastRenderFrame; float WFInterpolate; };
struct CInterpolationInfo {
    CInterpolationInfoSet Current, Last, PausedCurrent, PausedLast, BulletTimeCurrent, BulletTimeLast;
    long                  WFServerCurrentTime;
};
struct CEnginePrimitivePrepareInfo {
    CInterpolationInfo Interpolate;                // +0x00
    CIEngine*          Engine;                     // +0x34
};

class CParticleEmitter {
public:
    unsigned char _pad00[0x44];
    unsigned char PreWaterEffect : 1;              // +0x44
    unsigned char WaterEffect : 1;
    unsigned char ZBufferWriteable : 1;
    unsigned char Visible : 1;
    unsigned char AlwaysUpdate : 1;
    void PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info);   // 0x00AEE560
    void ClearAllPrimitives(CIEngine* engine);     // 0x00AEE940
};

struct SCountBlock {
    long  Count;                                   // +0x00
    void (__fastcall* Destroy)(void* object);      // +0x04
    void* Object;                                  // +0x08
};

template <class T> struct CCountedPointer {
    T*           Object;
    SCountBlock* Info;
    T* operator->() const { return Object; }
    void Release()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
        Object = 0;
        Info = 0;
    }
};

template <class T> struct list {                   // STLport list: the 4-byte head node pointer
    struct _Node { _Node* _M_next; _Node* _M_prev; T _M_data; };
    struct iterator {
        _Node* _M_node;
        T& operator*() const { return _M_node->_M_data; }
        T* operator->() const { return &_M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    _Node* _M_node;
    list()
    {
        _Node* node = (_Node*)malloc(sizeof(_Node));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~list()
    {
        clear();
        free(_M_node);
    }
    iterator begin() const { iterator it; it._M_node = _M_node->_M_next; return it; }
    iterator end() const { iterator it; it._M_node = _M_node; return it; }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (_Node* first = _M_node->_M_next; first != _M_node; first = first->_M_next)
            ++n;
        return n;
    }
    void clear();                                  // 0x00AEC840 for this T
    void push_back(const T& x)
    {
        _Node* node = (_Node*)malloc(sizeof(_Node));
        node->_M_data = x;
        node->_M_next = _M_node;
        node->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = node;
        _M_node->_M_prev = node;
    }
};
typedef list<CCountedPointer<CParticleEmitter> > CEmitterList;

class CParticleEmitterList {
public:
    CEmitterList Active;                           // +0x00
    long         MaxActiveEmitters;                // +0x04
    void FindVisible(CEmitterList& result, long maxVisible, const CEnginePrimitivePrepareInfo& info);   // 0x00AEB2D0
};

extern bool g_ParticlesEnabled;                    // 0x0139950C

// Same-TU callees compiled ahead of PreparePrimitives (retail keeps the visible list's head node in esi across both calls,
// i.e. VC7.1 knew neither writes the list object itself): list::clear is the real 0x00AEC840 body (release each counted
// pointer, free the node, relink the head); FindVisible 0x00AEB2D0 is a STAND-IN that keeps only its contract (filter the
// active list into `result` with an inline push_back, the visibility test opaque) -- it is not graded.
bool IsEmitterVisible(CParticleEmitter* emitter, long maxVisible, const CEnginePrimitivePrepareInfo& info);   // stand-in
#pragma auto_inline(off)
template <class T> void list<T>::clear()
{
    _Node* cur = _M_node->_M_next;
    while (cur != _M_node) {
        _Node* tmp = cur;
        cur = cur->_M_next;
        tmp->_M_data.Release();
        free(tmp);
    }
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
}
template void list<CCountedPointer<CParticleEmitter> >::clear();
void CParticleEmitterList::FindVisible(CEmitterList& result, long maxVisible, const CEnginePrimitivePrepareInfo& info)
{
    for (CEmitterList::iterator it = Active.begin(); it != Active.end(); ++it)
        if (IsEmitterVisible(it->Object, maxVisible, info))
            result.push_back(*it);
}
#pragma auto_inline(on)

class CParticleEmitterManager {
public:
    CParticleEmitterList ActiveEmitters;           // +0x0000
    unsigned char        PerlinNoise[0x1000];      // +0x0008
    long                 MaxVisibleEmitters;       // +0x1008
    long                 VisibleEmitterCount;      // +0x100c
    bool                 Cleared : 1;              // +0x1010
    bool                 EnableDecals : 1;
    CIEngine*            PEngine;                  // +0x1014
    void StartFrame(const CEnginePrimitivePrepareInfo& info);   // 0x00AEB550
    void PreparePrimitives(const CEnginePrimitivePrepareInfo& info);
};

void CParticleEmitterManager::PreparePrimitives(const CEnginePrimitivePrepareInfo& info)
{
    if (g_ParticlesEnabled) {
        Cleared = false;
        float currentTime = info.Interpolate.Current.WFInterpolate;
        float bulletTime = info.Interpolate.BulletTimeCurrent.WFInterpolate;
        StartFrame(info);
        CEmitterList visible;
        ActiveEmitters.FindVisible(visible, MaxVisibleEmitters, info);
        VisibleEmitterCount = visible.size();
        for (CEmitterList::iterator it = visible.begin(); it != visible.end(); ++it) {
            if ((*it)->AlwaysUpdate)
                (*it)->PreparePrimitives(currentTime, info);
            else
                (*it)->PreparePrimitives(bulletTime, info);
        }
    }
    else if (!Cleared) {
        for (CEmitterList::iterator it = ActiveEmitters.Active.begin(); it != ActiveEmitters.Active.end(); ++it)
            (*it)->ClearAllPrimitives(info.Engine);
        Cleared = true;
    }
}

} // namespace NParticleEngine
