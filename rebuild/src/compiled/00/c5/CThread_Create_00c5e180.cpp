// CThread::Create(unsigned long (__stdcall* start)(void*), void* param, void* security, unsigned long flags, unsigned long stackSize) -- start a Win32 thread with CreateThread, keeping its handle (+4) and id (+8); true when a handle came back
// Leaf: ?Create@CThread@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5E180, length 52 bytes (+ int3 padding); `ret 14h`, returns bool.
// Callee: CreateThread (IAT 0x0143FE68). Caller: 0x00C5EA5D (the device worker: `m_thread.Create(0x00C5E330, this,
// 0, 0, 0)` on its member at +0x34, then a thread-priority call on the handle).
// The landed 0x00C5E1C0 (`CHandleOwner_00c5e1c0::CloseHandle_00c5e1c0`) is the same wrapper's Close (handle +4).
// Label: the manifest's "CThread_Create" is close; GameCODA has no PDB, the parameter order is from the push order:
// CreateThread(security, stackSize, start, param, flags, &m_threadId).
// Shape: `m_handle != 0` returned as bool: `xor ecx,ecx ; test eax,eax ; mov [esi+4],eax ; setne cl ; mov al,cl`.
extern "C" __declspec(dllimport) void* __stdcall CreateThread(void* security, unsigned long stackSize,
                                                              unsigned long (__stdcall* start)(void*), void* param,
                                                              unsigned long flags, unsigned long* threadId);   // IAT 0x0143FE68

class CThread {
public:
    unsigned long m_0;                  // +0x00 (not touched here)
    void*         m_handle;             // +0x04
    unsigned long m_threadId;           // +0x08
    bool Create(unsigned long (__stdcall* start)(void*), void* param, void* security, unsigned long flags,
                unsigned long stackSize);
};

bool CThread::Create(unsigned long (__stdcall* start)(void*), void* param, void* security, unsigned long flags,
                     unsigned long stackSize)
{
    m_handle = CreateThread(security, stackSize, start, param, flags, &m_threadId);
    return m_handle != 0;
}