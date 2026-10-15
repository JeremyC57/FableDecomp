// CThreadMessage::TryReceive(unsigned long*) -- polls the request event without waiting; when a message is pending, copies it to the optional out-parameter and returns 1, else returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0EB0, length 47 bytes.
// Callees: WaitForSingleObject (IAT 0x0143FEAC) with a zero timeout.
// Layout (no PDB layout; read from this class's neighbours): +0x04 request event, +0x08 reply event,
// +0x0c posted message, +0x10 reply value, +0x14 worker thread handle, +0x18 the CRITICAL_SECTION.
// Shape: an int result (`xor eax,eax` / `mov eax,1`, a bool would use al); the early return for a
// non-zero wait result comes first.
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long milliseconds);   // IAT 0x0143FEAC

struct CThreadMessage {
    void*         m_vtable;                        // +0x00
    void*         m_hRequestEvent;                 // +0x04
    void*         m_hReplyEvent;                   // +0x08
    unsigned long m_message;                       // +0x0c
    long          m_reply;                         // +0x10
    void*         m_hThread;                       // +0x14
    int TryReceive(unsigned long* message);
};

int CThreadMessage::TryReceive(unsigned long* message)
{
    if (WaitForSingleObject(m_hRequestEvent, 0) != 0)   // not WAIT_OBJECT_0
        return 0;
    if (message)
        *message = m_message;
    return 1;
}
