// CEnginePrimitiveHandle::`scalar deleting destructor'(unsigned int flags) -- the inlined destructor (Clear: tell the primitive its last handle is going, drop the counted Data pointer; then the member's own release and ~CBaseClass) and, for flag bit 0, operator delete
// Grade: RELOCATION_MATCH -- the inlined Clear tests the handle data through a null-safe `long GetRefCount() const { return Object ? Object->m_refCount : 0; }` compared with 1 (GOTCHAS chunk-137 shape): Data loads into eax, the vtable store goes first and `mov ecx,eax` precedes the slot-3 call
// Leaf: ??_G
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041C500, length 115 bytes (128 with padding).
// Callees: the primitive's virtual slot 3 (+0xC, "last handle released", called with the handle) when its count is exactly 1,
// its virtual destroy (slot 1) at zero count, CBaseClass::~CBaseClass @ 0x0099A2E0 (out of line), operator delete @ 0x00BFE9BC.
// Shape: the vtable store (0x0122F598, CEnginePrimitiveHandle) then the inline Clear (the same body as the retail
// CEnginePrimitiveHandle::Clear @ 0x0041BE70): `Data = 0` through CIVCountedPointer::operator=(pointee*) -- a release of a
// LOCAL copy (`dec [ecx+4] ; jne`), its null store, then the unconditional store of the new (null) value -- followed by the
// member's inline destructor (re-reads Data: release + null store again). Layout as the landed constructor @ 0x005BB32E:
// {vptr, CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +4}. Both the destructor and the counted pointer's members
// are `__forceinline` (GOTCHAS 24i: a `??_G` with an inlined counted release), and a `new` factory pulls in the vtable.
struct CBaseClass {                                // vtable 0x01231710
    virtual ~CBaseClass();                         // 0x0099A2E0
};

struct CEnginePrimitiveHandle;

struct CIVCountedPointeeBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                               // +0x04
    void NotifyIfLastHandle(CEnginePrimitiveHandle* handle)
    {
        if (m_refCount == 1)
            OnLastHandleReleased(handle);
    }
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
    long GetRefCount() const { return Object ? Object->m_refCount : 0; }
    __forceinline void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(CIVCountedPointeeBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->m_refCount;
        return *this;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};

struct CEnginePrimitiveHandle : CBaseClass {       // vtable 0x0122F598
    CIVCountedPointer Data;                        // +0x04
    CEnginePrimitiveHandle() { }
    __forceinline void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data.Object->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};

// VC7.1 emits the class's vtable and scalar deleting destructor in a TU that constructs it; this helper is that construction.
void* MakeOne()
{
    return new CEnginePrimitiveHandle;
}