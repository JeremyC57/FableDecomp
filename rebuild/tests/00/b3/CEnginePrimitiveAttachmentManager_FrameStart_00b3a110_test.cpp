// Behaviour test for 00b3a110: src/00b3a110.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CEnginePrimitiveAttachmentManager::FrameStart(const CEngineComponentParams& params)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEnginePrimitiveAttachmentManager::FrameStart(const CEngineComponentParams& params) -- updates the attached primitives for this frame against a one-element camera array holding the engine camera
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3A110, length 99 bytes (112 with padding); CEngineComponent-side vtable
// 0x012A1A78 slot 1 of CEnginePrimitiveAttachmentManager (rtti-vtables.tsv).
// Leaf: ?FrameStart@CEnginePrimitiveAttachmentManager
// Callees: the camera array's reallocating insert @ 0x00B3ABC0 (manifest "Vector_InsertMultipleFill"),
// CEnginePrimitiveAttachmentManager::UpdatePrimitives(const CArray<CEngineCamera*>&, float) @ 0x00B39FB0, free @ 0x00BFEA14.
// Globals: the engine camera pointer @ 0x01436EA0 (its ADDRESS is pushed: push_back takes it by const reference).
// Shape: FrameStart is first declared by the secondary base CEngineComponent (at +4, see the landed constructor
// @ 0x00B39F30), so the override runs with `this` at that subobject and reaches the full object with `lea ecx,[esi-4]`.
// The local array is the retail 12-byte CArray (first/last/end zeroed by its inline constructor); its inline push_back
// folds the `last != end` test of an empty array into the InsertGrow(last = 0, value, SScalarTag(), 1, true) call (the
// empty tag homed in the dead argument slot); the params' first word (the frame time) is passed by value; the array's
// inline destructor frees the storage.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };           // user constructor: no zero-initialisation of the 1-byte temporary

template <class T>
struct CArray {                                    // retail CArray: first / last / end of storage (12 bytes)
    T* first;
    T* last;
    T* end;
    CArray() : first(0), last(0), end(0) { }
    ~CArray()
    {
        if (first)
            free(first);
    }
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x00B3ABC0
    void push_back(const T& value)
    {
        if (last != end) {
            *last = value;
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CArray<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)

struct CEngineCamera;
extern CEngineCamera* g_pEngineCamera_01436ea0;    // 0x01436EA0

struct CEngineComponentParams {
    float m_frameTime;                             // +0x00
};

struct CEngineRoot_0099a2f0 {
    virtual void v00();
};

struct CEngineComponent {                          // secondary base at +4
    virtual void v00();
    virtual void FrameStart(const CEngineComponentParams& params);   // slot 1
    long m_4;                                      // +0x04
    bool m_enabled;                                // +0x08
    bool m_9;                                      // +0x09
};

struct CEnginePrimitiveAttachmentManager : CEngineRoot_0099a2f0, CEngineComponent {
    void* AttachedPrimitives;                      // +0x10 (retail STLport list: sentinel pointer)
    void UpdatePrimitives(const CArray<CEngineCamera*>& cameras, float frameTime);   // 0x00B39FB0
    virtual void FrameStart(const CEngineComponentParams& params);
};

void CEnginePrimitiveAttachmentManager::FrameStart(const CEngineComponentParams& params)
{
    CArray<CEngineCamera*> cameras;
    cameras.push_back(g_pEngineCamera_01436ea0);
    UpdatePrimitives(cameras, params.m_frameTime);
}

// ---- stubs and driver ----
extern "C" void* __cdecl malloc(unsigned int size);
static int g_grow = 0, g_upd = 0; static float g_time = 0; static long g_count = -1; static CEngineCamera* g_cam0 = 0;
CEngineCamera* g_pEngineCamera_01436ea0 = 0;
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{
    ++g_grow; CArray<CEngineCamera*>* v = (CArray<CEngineCamera*>*)storage; long n = v->last - v->first;
    CEngineCamera** s = (CEngineCamera**)malloc((n + count) * sizeof(CEngineCamera*)); if (n) memcpy(s, v->first, n * sizeof(CEngineCamera*));
    v->first = s; v->last = s + n + count; v->end = v->last; return s + n;
}
void CEngineRoot_0099a2f0::v00() {}
void CEngineComponent::v00() {}
void CEngineComponent::FrameStart(const CEngineComponentParams&) {}
void CEnginePrimitiveAttachmentManager::UpdatePrimitives(const CArray<CEngineCamera*>& cameras, float frameTime)
{ ++g_upd; g_time = frameTime; g_count = cameras.last - cameras.first; g_cam0 = g_count > 0 ? cameras.first[0] : 0; CHECK(this != 0); }
int main()
{
    CEnginePrimitiveAttachmentManager m; CEngineComponentParams p; p.m_frameTime = 0.25f;
    g_pEngineCamera_01436ea0 = (CEngineCamera*)0x1234;
    m.FrameStart(p);
    CHECK(g_upd == 1 && g_time == 0.25f && g_count == 1 && g_cam0 == (CEngineCamera*)0x1234 && g_grow == 1);
    g_pEngineCamera_01436ea0 = 0; p.m_frameTime = 1.5f; m.FrameStart(p);
    CHECK(g_upd == 2 && g_time == 1.5f && g_count == 1 && g_cam0 == 0 && g_grow == 2);
    if (fails == 0) printf("00b3a110_TEST PASS\n"); else printf("00b3a110_TEST FAIL\n");
    return fails;
}