// CViewport::ClearHandle() -- when the viewport's counted handle (+0x28) holds the object's last external reference (count == 1), notify the object (virtual slot 3, with the handle); then release the handle
// Grade: DIFFER(ours v retail) 58v58 -- only the vtable register of the slot-3 call: retail `mov ecx,eax ; mov eax,[ecx] ; push esi ; call [eax+0Ch]`, ours `mov ecx,eax ; mov edx,[eax] ; push esi ; call [edx+0Ch]`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00669390, length 58 bytes. Caller: CThingPhysical::PrepareForRendering (0x00663C40).
// Leaf: ClearHandle
// Callees: the counted object's virtual slots 3 (+0x0c, takes the handle) and 1 (+0x04, destroy).
// Shape: the object read from this (`mov eax,[ecx+2Ch]`) BEFORE the handle address is formed (`lea esi,[ecx+28h]`), so both are
// named locals (object first); the release is the CIVCountedPointer `operator=(0)` shape (wave 24h): a local copy, `dec [p+4] ; jne`,
// the clear inside the test plus a second unconditional clear.
// Tried: member-only form (this kept in esi), handle pointer local only (ecx = object), object local only (this kept), both locals
// (this file: everything but the vtable register). Next: route the slot-3 call through an inline helper on the handle/object so the
// vtable is fetched from ecx, or a lane gap (register choice).
struct CHandle;
struct CCountedObject {
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    virtual void v02();
    virtual void OnLastHandle(CHandle* handle); // slot 3 (+0x0c)
    long m_count;                               // +0x04
};
struct CHandle {
    void*           m_owner;                    // +0x00
    CCountedObject* m_p;                        // +0x04
    void Reset()
    {
        CCountedObject* cur = m_p;
        if (cur) {
            if (--cur->m_count == 0)
                cur->Destroy();
            m_p = 0;
        }
        m_p = 0;
    }
};
struct CViewport {
    unsigned char _pad00[0x28];
    CHandle       m_handle;                     // +0x28
    void ClearHandle();
};
void CViewport::ClearHandle()
{
    CCountedObject* object = m_handle.m_p;
    CHandle* handle = &m_handle;
    if (object && object->m_count == 1)
        object->OnLastHandle(handle);
    handle->Reset();
}
