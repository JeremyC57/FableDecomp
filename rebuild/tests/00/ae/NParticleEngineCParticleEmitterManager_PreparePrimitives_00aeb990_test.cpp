#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
namespace NParticleEngine { class CIEngine; }
static NParticleEngine::CIEngine* g_engine = (NParticleEngine::CIEngine*)0x1234;
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
    float preparedAt; int clears;
    void PreparePrimitives(float time, const CEnginePrimitivePrepareInfo& info) { preparedAt = time; (void)info; }
    void ClearAllPrimitives(CIEngine* engine) { if (engine == g_engine) ++clears; }
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
                ::operator delete(Info);
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

bool g_ParticlesEnabled;

// Same-TU callees compiled ahead of PreparePrimitives (retail keeps the visible list's head node in esi across both calls,
// i.e. VC7.1 knew neither writes the list object itself): list::clear is the real 0x00AEC840 body (release each counted
// pointer, free the node, relink the head); FindVisible 0x00AEB2D0 is a STAND-IN that keeps only its contract (filter the
// active list into `result` with an inline push_back, the visibility test opaque) -- it is not graded.
bool IsEmitterVisible(CParticleEmitter* emitter, long maxVisible, const CEnginePrimitivePrepareInfo& info) { (void)maxVisible; (void)info; return emitter->Visible != 0; }
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

class CParticleEmitterManager {
public:
    CParticleEmitterList ActiveEmitters;           // +0x0000
    unsigned char        PerlinNoise[0x1000];      // +0x0008
    long                 MaxVisibleEmitters;       // +0x1008
    long                 VisibleEmitterCount;      // +0x100c
    bool                 Cleared : 1;              // +0x1010
    bool                 EnableDecals : 1;
    CIEngine*            PEngine;                  // +0x1014
    int frames;
    void StartFrame(const CEnginePrimitivePrepareInfo& info) { ++frames; (void)info; }
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

using namespace NParticleEngine;
int main()
{
    static CParticleEmitter e[3];
    for (int i = 0; i < 3; ++i) { std::memset(&e[i], 0, sizeof(e[i])); e[i].preparedAt = -1.0f; }
    e[0].Visible = 1; e[0].AlwaysUpdate = 1; e[1].Visible = 0; e[2].Visible = 1; e[2].AlwaysUpdate = 0;
    static unsigned char raw[sizeof(CParticleEmitterManager)];
    CParticleEmitterManager* m = new (raw) CParticleEmitterManager;
    m->frames = 0; m->MaxVisibleEmitters = 8; m->VisibleEmitterCount = -1; m->Cleared = true; m->EnableDecals = true;
    for (int i = 0; i < 3; ++i) { CCountedPointer<CParticleEmitter> p; p.Object = &e[i]; p.Info = 0; m->ActiveEmitters.Active.push_back(p); }
    CEnginePrimitivePrepareInfo info; std::memset(&info, 0, sizeof(info));
    info.Interpolate.Current.WFInterpolate = 0.25f; info.Interpolate.BulletTimeCurrent.WFInterpolate = 0.75f; info.Engine = g_engine;
    g_ParticlesEnabled = true;
    m->PreparePrimitives(info);
    if (m->frames != 1 || m->VisibleEmitterCount != 2 || m->Cleared != false || m->EnableDecals != true) { std::printf("00aeb990_TEST FAIL\n"); return 1; }
    if (e[0].preparedAt != 0.25f || e[1].preparedAt != -1.0f || e[2].preparedAt != 0.75f) { std::printf("00aeb990_TEST FAIL\n"); return 1; }
    g_ParticlesEnabled = false;
    m->PreparePrimitives(info);
    m->PreparePrimitives(info);
    if (m->frames != 1 || m->Cleared != true || e[0].clears != 1 || e[1].clears != 1 || e[2].clears != 1) { std::printf("00aeb990_TEST FAIL\n"); return 1; }
    std::printf("00aeb990_TEST PASS\n");
    return 0;
}