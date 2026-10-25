// CVtable_012b90d4_00c5ec50::`scalar deleting destructor'(unsigned int) -- this class's vtable (0x012B90D4, no RTTI), close the Win32 handle at +4 unless it is -1 and set it to -1, flag-tested delete
// Leaf: ??_GCVtable_012b90d4_00c5ec50@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5EC00, 53 bytes, thiscall, `ret 4`.
// Identity: the only slot of the no-RTTI vtable 0x012B90D4 (the next words are GUID data); the class is the landed
//   handle owner CVtable_012b90d4_00c5ec50 (destructor 0x00C5EC50 right after this body, Reset 0x00C5E3F0; constructor
//   0x00C5DFD0 stores the vtable and -1). Named by the vtable as those rows are; the catalog label
//   CWaterPatchMesh::`scalar_deleting_destructor' is propagated.
// Callees: CloseHandle (kernel32 import through 0x0143FE30), operator delete 0x00BFE9BC.
// Shape: the landed destructor body (handle loaded and compared before the vtable store) inlined into the deleting
//   destructor; the destructor is virtual here (slot 0 is this body) and defined out of line in the TU, which emits the
//   vtable and the deleting destructor.
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);   // IAT 0x0143FE30

struct CVtable_012b90d4_00c5ec50 {                     // vtable 0x012B90D4
    virtual ~CVtable_012b90d4_00c5ec50();
    void* m_hHandle;                                   // +0x04
};

CVtable_012b90d4_00c5ec50::~CVtable_012b90d4_00c5ec50()
{
    if (m_hHandle != (void*)-1) {
        CloseHandle(m_hHandle);
        m_hHandle = (void*)-1;
    }
}