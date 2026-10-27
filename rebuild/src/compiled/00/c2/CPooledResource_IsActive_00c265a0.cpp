// CPooledResource::IsActive() -- whether the resource's state word (+0x74) is 1 (active)
// Leaf: ?IsActive@CPooledResource@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C265A0, 11 bytes: xor eax,eax ; cmp word ptr [ecx+74h],1 ; sete al ; ret
// Identity (descriptive): the landed CPooledResource::Release 0x00C265B0 binds ?IsActive@CPooledResource@@QAEJXZ to this
//   address (its state 1/2 setters are 0x00C26560 / 0x00C26580); slots 1 and 4 of vtable 0x012B3FDC (0x00C265E0,
//   0x00C26630) call it before deactivating. The catalog's "CManager::GetKeyPressed" is a propagated label.
// Shape: a long result of `==` gives `xor eax,eax ; cmp ; sete al`.
struct CPooledResource {
    void* __vftable;                            // +0x00 (0x012B3FDC)
    long  m_handle;                             // +0x04
    unsigned char _pad08[0x74 - 0x08];
    short m_state;                              // +0x74
    long IsActive();
};

long CPooledResource::IsActive()
{
    return m_state == 1;
}