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
class CArray { public:                                    // retail CArray: first / last / end of storage (12 bytes)
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

class CEngineCamera;
extern CEngineCamera* g_pEngineCamera_01436ea0;    // 0x01436EA0

class CEngineComponentParams { public:
    float m_frameTime;                             // +0x00
};

struct CEngineRoot_0099a2f0 {
    virtual void v00();
};

class CEngineComponent { public:                          // secondary base at +4
    virtual void v00();
    virtual void FrameStart(const CEngineComponentParams& params);   // slot 1
    long GlobalPointerAddress;                                      // +0x04
    bool m_enabled;                                // +0x08
    bool KeepAlive;                                      // +0x09
};

class CEnginePrimitiveAttachmentManager : public CEngineRoot_0099a2f0, public CEngineComponent { public:
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