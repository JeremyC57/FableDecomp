// CEngineInternalAttachedPrimitive::~CEngineInternalAttachedPrimitive() -- destructor: the attachment info's two dummy names, the output-primitive and parent handles (each tells its primitive when it is the last handle, then releases it), then the root CIVCountedPointeeBase's vtable
// Leaf: ??1CEngineInternalAttachedPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3ACF0, 203 bytes (+ int3 padding); called only from the deleting destructor at 0x00B3ACD3.
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
struct CBaseClass {                                    // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                      // 0x0099A2D0
    ~CBaseClass();                                     // 0x0099A2E0
};

struct CEnginePrimitiveHandle;
struct CEnginePersistentPrimitiveBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                                   // +0x04
};

template <class T> struct CIVCountedPointer {          // intrusive counted pointer (count at +4 of the object)
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

struct CEnginePrimitiveHandle : CBaseClass {           // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
    virtual void v00();
    CEnginePrimitiveHandle() { }
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct CCharString {
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};

struct CEnginePrimitiveAttachmentInfo {
    unsigned long Flags;                               // +0x00
    CCharString   ParentDummyName;                     // +0x04
    long          ParentDummyIndex;                    // +0x08
    CCharString   ChildDummyName;                      // +0x0c
    long          ChildDummyIndex;                     // +0x10
};

struct CMatrix3x4 {
    float E[12];
};

struct CIVCountedPointeeBase {                         // vtable 0x012354D0
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