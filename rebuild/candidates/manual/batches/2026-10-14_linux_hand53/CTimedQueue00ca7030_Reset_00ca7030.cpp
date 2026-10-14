// CTimedQueue_00ca7030::Reset() -- kill the multimedia timer at +0xc0 (timeKillEvent @ IAT 0x014404B0) when set, then under the critical section at +0x94 (Enter/LeaveCriticalSection @ IAT 0x0143FE0C / 0x0143FE10) clear the four words at +0x70, +0x74, +0xb8, +0xbc; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA7030, length 80 bytes.
// The chunk label "ClearTextures" is propagated; named by its effect. Shape: the zero register ebx, the section address in edi, `xor eax,eax`, `ret`.
extern "C" __declspec(dllimport) unsigned int __stdcall timeKillEvent(unsigned int timer);   // IAT 0x014404B0
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);       // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);       // IAT 0x0143FE10
struct CTimedQueue_00ca7030 {
    unsigned char _pad[0x70];
    long          m_70;                            // +0x70
    long          m_74;                            // +0x74
    unsigned char _pad78[0x1c];
    unsigned char m_section[24];                   // +0x94 (a CRITICAL_SECTION)
    unsigned char _padac[0xc];
    long          m_b8;                            // +0xb8
    long          m_bc;                            // +0xbc
    unsigned int  m_timer;                         // +0xc0
    long Reset();
};

long CTimedQueue_00ca7030::Reset()
{
    if (m_timer) {
        timeKillEvent(m_timer);
        m_timer = 0;
    }
    EnterCriticalSection(m_section);
    m_70 = 0;
    m_74 = 0;
    m_b8 = 0;
    m_bc = 0;
    LeaveCriticalSection(m_section);
    return 0;
}
