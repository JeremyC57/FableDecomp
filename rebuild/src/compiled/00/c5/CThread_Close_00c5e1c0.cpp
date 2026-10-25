// CThread::Close() -- close the wrapped Win32 thread handle (+4) with CloseHandle when one is open, and forget it
// Leaf: ?Close@CThread@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5E1C0, 26 bytes (+ int3 padding), thiscall, plain `ret`.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je done ; push eax ; call [CloseHandle] ;
//   mov dword ptr [esi+4],0 ; done: pop esi ; ret
// Identity (sweep1, 2026-10-25; FINDINGS chunk 441 "0xC5E1C0 "CHandleOwner::CloseHandle" = CThread::Close"):
//   - agent441's landed CThread::Create 0x00C5E180 is the same wrapper: it stores CreateThread's handle at +4 and the
//     thread id at +8 (`m_thread.Create(0x00C5E330, this, 0, 0, 0)` on the device worker's +0x34 member, 0x00C5EA5D);
//   - the next function 0x00C5E1E0 passes the same +4 handle to the thread-priority import (IAT 0x0143FDD0).
//   The landed CHandleOwner_00c5e1c0::CloseHandle_00c5e1c0 is a placeholder name; descriptive names (GameCODA: no PDB).
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);   // IAT 0x0143FE30

class CThread {
public:
    unsigned long m_0;                                  // +0x00 (not touched here)
    void*         m_handle;                             // +0x04 thread handle
    unsigned long m_threadId;                           // +0x08
    void Close();
};

void CThread::Close()
{
    if (m_handle) {
        CloseHandle(m_handle);
        m_handle = 0;
    }
}
