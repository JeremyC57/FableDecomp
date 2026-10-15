// CPooledResource::Release(CPooledResourceManager*) -- if the entry is active it is first deactivated with its manager, then its pool allocation (when it holds one) is freed
// Leaf: ?Release@CPooledResource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C265B0, 47 bytes (+1 int3). Callers: 0x00C130C7, 0x00C269E8.
// Callees: the entry's "is active" test @ 0x00C265A0 (`xor eax,eax ; cmp word ptr [ecx+74h],1 ; sete al`), its deactivate
//          @ 0x00C26580 (state 2, then the manager's +0x2C list slot 8 with the entry; its twin 0x00C26560 sets state 1
//          and uses slot 9), CHandleBasedMemoryPool::FreeAllocation @ 0x00C28690 on the manager's pool at +0x360.
//
// LABEL CORRECTION: "CTCInventoryWeapons::ReturnAllConfiscatedItems" is propagated; nothing here touches the weapons
// screen. The class names are DESCRIPTIVE (an unnamed late module: a handle-pool resource entry with a short state at
// +0x74 and its pool handle at +4; the manager keeps two intrusive lists at +0x2B4/+0x2E4 and the pool at +0x360).
//
// Shape: `call IsActive ; test eax,eax ; mov edi,[esp+0Ch] ; je ; push edi ; call Deactivate` -- ecx is NOT reloaded
// for the second call: VC7.1 knows that a function compiled earlier in the same translation unit (0x00C265A0, eax
// only) leaves ecx = this. IsActive is therefore defined above with `#pragma auto_inline(off)` and returns a long
// (`xor eax,eax ; sete al`, tested with `test eax,eax`). The handle is a named local (esi is reused for it after the
// last use of `this`), tested signed (`jl`).
struct CPooledResource;
struct CHandleBasedMemoryPool {
    void FreeAllocation(long handle);         // 0x00C28690
};
struct CPooledResourceManager {
    unsigned char _pad_0x00[0x360];
    CHandleBasedMemoryPool m_pool;            // +0x360
};
struct CPooledResource {
    void* __vftable;
    long  m_handle;                           // +0x04
    unsigned char _pad_0x08[0x74 - 8];
    short m_state;                            // +0x74
    long IsActive();
    void Deactivate(CPooledResourceManager* manager);   // 0x00C26580
    void Release(CPooledResourceManager* manager);
};
#pragma auto_inline(off)
long CPooledResource::IsActive()
{
    return m_state == 1;
}
#pragma auto_inline(on)
void CPooledResource::Release(CPooledResourceManager* manager)
{
    if (IsActive())
        Deactivate(manager);
    long handle = m_handle;
    if (handle >= 0)
        manager->m_pool.FreeAllocation(handle);
}
