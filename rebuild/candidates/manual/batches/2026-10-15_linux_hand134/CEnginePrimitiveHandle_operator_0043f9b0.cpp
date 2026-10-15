// CEnginePrimitiveHandle::operator=(const CEnginePrimitiveHandle& other) -- handle assignment: release this handle (last-handle notification + counted release), then share other's primitive (counted copy)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043F9B0, length 109 bytes (`ret 4`, returns this).
// Callees: the primitive's virtual slots 3 (+0x0C, OnLastHandleReleased(handle)) and 1 (+0x04, Destroy at zero count).
// Label: the manifest's "CEngineInternalPrimitiveParticleGroup::SetChildPrimitive" is propagated and wrong -- the body is the
// out-of-line copy of the handle assignment that CEngineInternalPrimitiveParticleGroup::SetChildPrimitive (0x00BBC310) and
// GetChildPrimitiveHandle (0x00BBC3A0) inline; `this` is the 8-byte CEnginePrimitiveHandle (vptr 0x0122F598 + CIVCountedPointer Data).
// Shape: Clear() is the agent-61 handle release (IsUnique through operator->, `Data = 0` = release + second zero store); the counted
// copy reads the old pointer into a local (`mov ecx,[esi+4] ; cmp ecx,[edi+4]`) and the release lives on the pointee
// (`old->Release()` keeps the pointer in ecx for `dec [ecx+4] ; jne ; call [vt+4]`), other's pointer is re-read after the release.
// Leaf: ??4CEnginePrimitiveHandle
class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CIVCountedPointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CEnginePersistentPrimitiveBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

class CEnginePrimitiveHandle {                     // 8 bytes: vptr (0x0122F598) + counted Data
public:
    virtual void v000();
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other);
};

CEnginePrimitiveHandle& CEnginePrimitiveHandle::operator=(const CEnginePrimitiveHandle& other)
{
    Clear();
    Data = other.Data;
    return *this;
}
