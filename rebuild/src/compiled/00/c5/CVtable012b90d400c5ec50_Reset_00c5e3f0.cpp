// CVtable_012b90d4_00c5ec50::Reset(void* handle) -- close the owned Win32 handle (unless it is INVALID_HANDLE_VALUE) and take ownership of a new one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5E3F0, length 47 bytes (48 with int3 padding).
// Leaf: Reset
// Callees: CloseHandle @ IAT 0x0143FE30 (kernel32, __stdcall).
// The manifest label "CDynamicObjectHeap<24,853>::Release" is propagated and rejected: the body tests the
// word at +4 against -1, closes it through the kernel32 CloseHandle import and stores the argument there
// (`ret 4`). It is the same handle-owning class as the landed destructor 0x00C5EC50 (vtable 0x012B90D4,
// handle at +4, the same `!= -1 ; CloseHandle ; = -1` close), so the model reuses that class; the caller
// 0x00C5E901 hands it a freshly opened handle for the object at +0xc of its owner.
// Shape: the inline Close() keeps its dead `= -1` store before the assignment (VC7.1 does not drop a store
// that follows an import call), the argument is loaded once per arm, and the two arms end in separate
// `pop esi ; ret 4` exits.
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);   // IAT 0x0143FE30

struct CVtable_012b90d4_00c5ec50 {                     // vtable 0x012B90D4
    virtual void v00();
    void* m_hHandle;                                   // +0x04
    void Close()
    {
        if (m_hHandle != (void*)-1) {
            CloseHandle(m_hHandle);
            m_hHandle = (void*)-1;
        }
    }
    void Reset(void* handle);
};

void CVtable_012b90d4_00c5ec50::Reset(void* handle)
{
    Close();
    m_hHandle = handle;
}