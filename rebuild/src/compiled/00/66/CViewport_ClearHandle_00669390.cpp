// CViewport::ClearHandle() -- when the viewport's counted handle (+0x28) holds the object's last external reference (count == 1), notify the object (virtual slot 3, with the handle); then release the handle
// Grade: MATCH -- the handle is modelled like CEnginePrimitiveHandle (landed 0x0041C500): an inline Clear() tests a null-safe
// `long GetRefCount() const { return Object ? Object->m_count : 0; }` against 1 (object loaded into eax, `mov ecx,eax` for the
// slot-3 call) and then assigns 0 through the counted pointer's inline operator= (release + two clears).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00669390, length 58 bytes. Caller: CThingPhysical::PrepareForRendering (0x00663C40).
// Leaf: ClearHandle
// Callees: the counted object's virtual slots 3 (+0x0c, takes the handle) and 1 (+0x04, destroy).
// Shape: CViewport::ClearHandle is `m_handle.Clear();` with the handle's Clear inlined (the same body as the retail
// CEnginePrimitiveHandle::Clear @ 0x0041BE70); the release is the CIVCountedPointer `operator=(0)` shape (wave 24h): a local copy,
// `dec [p+4] ; jne`, the clear inside the test plus a second unconditional clear.
struct CHandle;
struct CCountedObject {
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    virtual void v02();
    virtual void OnLastHandle(CHandle* handle); // slot 3 (+0x0c)
    long m_count;                               // +0x04
};
struct CCountedPointer {
    CCountedObject* Object;
    long GetRefCount() const { return Object ? Object->m_count : 0; }
    __forceinline void Release()
    {
        CCountedObject* object = Object;
        if (object) {
            if (--object->m_count == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CCountedPointer& operator=(CCountedObject* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->m_count;
        return *this;
    }
};
struct CHandle {
    void*           m_owner;                    // +0x00
    CCountedPointer Data;                       // +0x04
    __forceinline void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data.Object->OnLastHandle(this);
        Data = 0;
    }
};
struct CViewport {
    unsigned char _pad00[0x28];
    CHandle       m_handle;                     // +0x28
    void ClearHandle();
};
void CViewport::ClearHandle()
{
    m_handle.Clear();
}