// CVtable_012b90d4_00c5ec50::~CVtable_012b90d4_00c5ec50() -- this class's vtable (0x012B90D4, no RTTI), then close the Win32 handle at +4 (CloseHandle @ IAT 0x0143FE30) unless it is -1 and set it to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5EC50, length 48 bytes.
// Leaf: ??1CVtable_012b90d4_00c5ec50@@QAE@XZ
// The chunk label "ClearTextures" is propagated; named by the vtable (one of two byte-identical bodies). Shape: the handle loaded and compared before the vtable store, `ret`.
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);   // IAT 0x0143FE30
struct CVtable_012b90d4_00c5ec50 {                     // vtable 0x012B90D4
    virtual void v00();
    void* m_hHandle;                               // +0x04
    ~CVtable_012b90d4_00c5ec50();
};

CVtable_012b90d4_00c5ec50::~CVtable_012b90d4_00c5ec50()
{
    if (m_hHandle != (void*)-1) {
        CloseHandle(m_hHandle);
        m_hHandle = (void*)-1;
    }
}