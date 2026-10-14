// CVtable_012b8fc8::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x012B8FC8, no RTTI), the critical section at +4 deleted (DeleteCriticalSection @ IAT 0x0143FDE4), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D5B0, length 41 bytes.
// Leaf: ??_GCVtable_012b8fc8@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x012B8FC8) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);
struct CLockedRoot_00c5d5b0 {
    virtual void v00();
};
struct CVtable_012b8fc8 : CLockedRoot_00c5d5b0 {              // vtable 0x012B8FC8
    unsigned char m_section[24];                   // +0x04 (a CRITICAL_SECTION)
    __forceinline virtual ~CVtable_012b8fc8()
    {
        DeleteCriticalSection(m_section);
    }
};

void* MakeOne_00c5d5b0()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CVtable_012b8fc8;
}
