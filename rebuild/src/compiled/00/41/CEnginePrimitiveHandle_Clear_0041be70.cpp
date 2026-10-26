// CEnginePrimitiveHandle::Clear() -- tell the primitive its last handle is going (virtual slot 3, with the handle) when its
// count is exactly 1, then drop the counted Data pointer (`Data = 0`: release a local copy, null store, store the new null)
// Retry 10b: plain compiler-emitted C++ (the catalog copy of this function is an asm_bake); the same model as the inlined
// Clear of the deleting destructor 0x0041C500.
// Leaf: ?Clear@CEnginePrimitiveHandle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041BE70, length 57 bytes.
// Callees: the primitive's virtual slot 3 (+0xC) and, at zero count, its virtual destroy (slot 1, +4).
// Layout as the landed constructor @ 0x005BB32E: {vptr, CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +4}.
class CBaseClass { public:                                // vtable 0x01231710
    virtual ~CBaseClass();                         // 0x0099A2E0
};

class CEnginePrimitiveHandle;

class CIVCountedPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3
    long m_refCount;                               // +0x04
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
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

class CEnginePrimitiveHandle : public CBaseClass { public:       // vtable 0x0122F598
    CIVCountedPointer Data;                        // +0x04
    void Clear();
};

void CEnginePrimitiveHandle::Clear()
{
    if (Data.Object && Data.Object->m_refCount == 1)
        Data.Object->OnLastHandleReleased(this);
    Data = 0;
}