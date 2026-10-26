#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Behaviour test: the model and logic below are a copy of the src/ file; the callees are test doubles.
// CEngineInternalAttachedPrimitive::~CEngineInternalAttachedPrimitive() -- destructor: the attachment info's two dummy names, the output-primitive and parent handles (each tells its primitive when it is the last handle, then releases it), then the root CIVCountedPointeeBase's vtable
// Leaf: ??1CEngineInternalAttachedPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3ACF0, 197 bytes (+ int3 padding); called only from the deleting destructor at 0x00B3ACD3.
// Callees: ~CCharString 0x0099EAE0 (ChildDummyName +0xa0, ParentDummyName +0x98), CBaseClass::~CBaseClass 0x0099A2E0 (each
//   handle's root); the handle's primitive virtual slots 3 (+0xc, OnLastHandleReleased(handle)) and 1 (+4, Destroy).
// Vtables: 0x0122F598 CEnginePrimitiveHandle (stored by each handle's inline dtor), 0x012354D0 CIVCountedPointeeBase (the
//   root, stored last); this class's own 0x012A1A04 and CEngineInternalPrimitiveBase's are never stored here.
// Layout = Ego_r.pdb / landed ctor 0x00B3A910: CEngineInternalPrimitiveBase (0x80 bytes, root CIVCountedPointeeBase
//   {vptr, RefCount}), ParentHandle +0x80, ParentGroupIndex +0x88, OutputPrimitive +0x8c, AttachmentInfo +0x94 {Flags,
//   ParentDummyName +0x98, ParentDummyIndex, ChildDummyName +0xa0, ChildDummyIndex}, ChildInverseTransform +0xa8,
//   ChildInverseTransformValid +0xd8.
// Shape: this class and CEngineInternalPrimitiveBase are `__declspec(novtable)` with empty inline dtors (no vtable store of
//   their own), the root's inline virtual dtor stores its vtable at the end; each handle is the landed
//   NParticleEngine::CParticleEmitter (0x00AF10E0) model: `__forceinline` dtor, `if (Data.GetRefCount() == 1)
//   Data->OnLastHandleReleased(this); Data = 0;` over a CIVCountedPointer member whose own dtor releases again.
class CBaseClass { public:                                    // vtable 0x01231710
    virtual void v00() {}
    CBaseClass();                                      // 0x0099A2D0
    ~CBaseClass();                                     // 0x0099A2E0
};

class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase { public:
    virtual void v00() {}
    virtual void Destroy() = 0;                            // slot 1
    virtual void v02() {}
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle) = 0;   // slot 3
    long m_refCount;                                   // +0x04
};

template <class T> class CIVCountedPointer { public:          // intrusive counted pointer (count at +4 of the object)
    T* m_p;
    CIVCountedPointer() : m_p(0) { }
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_p = 0;
        }
    }
    CIVCountedPointer& operator=(T* p) { Release(); m_p = p; return *this; }
    T* operator->() const { return m_p; }
    long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }
};

class CEnginePrimitiveHandle : public CBaseClass { public:           // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
    virtual void v00() {}
    CEnginePrimitiveHandle() { }
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

class CCharString { public:
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};

class CEnginePrimitiveAttachmentInfo { public:
    unsigned long Flags;                               // +0x00
    CCharString   ParentDummyName;                     // +0x04
    long          ParentDummyIndex;                    // +0x08
    CCharString   ChildDummyName;                      // +0x0c
    long          ChildDummyIndex;                     // +0x10
};

class CMatrix3x4 { public:
    float E[12];
};

class CIVCountedPointeeBase { public:                         // vtable 0x012354D0
    long RefCount;                                     // +0x04
    virtual ~CIVCountedPointeeBase() { }
};

struct __declspec(novtable) CEngineInternalPrimitiveBase : CIVCountedPointeeBase {
    unsigned char _body[0x80 - 8];
    ~CEngineInternalPrimitiveBase() { }
};

struct __declspec(novtable) CEngineInternalAttachedPrimitive : CEngineInternalPrimitiveBase {   // vtable 0x012A1A04
    CEnginePrimitiveHandle         ParentHandle;                // +0x80
    long                           ParentGroupIndex;            // +0x88
    CEnginePrimitiveHandle         OutputPrimitive;             // +0x8c
    CEnginePrimitiveAttachmentInfo AttachmentInfo;              // +0x94
    CMatrix3x4                     ChildInverseTransform;       // +0xa8
    bool                           ChildInverseTransformValid;  // +0xd8
    ~CEngineInternalAttachedPrimitive();
};

CEngineInternalAttachedPrimitive::~CEngineInternalAttachedPrimitive()
{
}

// ---- test doubles and driver ----
#include <new>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { if (g_n < 32) g_log[g_n++] = v; }
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { Log(1); }
CCharString::~CCharString() { Log(PStringData == (void*)0xA0 ? 2 : PStringData == (void*)0x98 ? 3 : 90); }
struct TestPrimitive : CEnginePersistentPrimitiveBase {
    int id; CEnginePrimitiveHandle* lastHandle;
    virtual void Destroy() { Log(10 + id); }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* h) { lastHandle = h; Log(20 + id); }
};
int main()
{
    static TestPrimitive parent, output;
    parent.id = 1; parent.m_refCount = 1; parent.lastHandle = 0;   // last handle: told, then destroyed
    output.id = 2; output.m_refCount = 3; output.lastHandle = 0;   // shared: only decremented
    static double raw[64];
    CEngineInternalAttachedPrimitive* p = new (raw) CEngineInternalAttachedPrimitive;
    p->ParentHandle.Data.m_p = &parent;
    p->OutputPrimitive.Data.m_p = &output;
    p->AttachmentInfo.ChildDummyName.PStringData = (void*)0xA0;
    p->AttachmentInfo.ParentDummyName.PStringData = (void*)0x98;
    p->CEngineInternalAttachedPrimitive::~CEngineInternalAttachedPrimitive();   // qualified: the class is novtable under VC
    // child name, parent name, output handle (refs 3 -> 2, base dtor), parent handle (last: told, destroyed, base dtor)
    static const int want[] = { 2, 3, 1, 21, 11, 1 };
    bool ok = g_n == 6 && output.m_refCount == 2 && parent.m_refCount == 0 && parent.lastHandle == &p->ParentHandle
        && output.lastHandle == 0 && p->ParentHandle.Data.m_p == 0 && p->OutputPrimitive.Data.m_p == 0;
    for (int i = 0; ok && i < 6; ++i) ok = g_log[i] == want[i];
    if (!ok) { std::printf("00b3acf0_TEST FAIL\n"); return 1; }
    std::printf("00b3acf0_TEST PASS\n");
    return 0;
}