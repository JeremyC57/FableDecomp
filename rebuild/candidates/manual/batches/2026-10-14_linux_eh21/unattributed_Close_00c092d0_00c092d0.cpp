// CPixelShader_00c092d0::Close() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the optional object at +0x60 is finished (@ 0x00C15C70); when the timer at +0x74 is armed, the tick count (import) past the deadline at +0x78 triggers the shutdown landed at 0x00C091F0 and re-arms the deadline to now minus (-1000 / the rate at +0x70)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C092D0, oracle length 175 bytes. Built under /EHsc.
// Retail bytes:  ... mov ecx,[esi+60h] ; test ecx,ecx ; mov dword ptr [esp+14h],0 ; je skip ; call CTarget::Finish ; skip: mov eax,[esi+74h] ; test eax,eax ; jne armed ; ... ret ; armed: push edi ; call [__imp_GetTickCount] ; mov edi,eax ; cmp edi,[esi+78h] ; jbe done ; mov ecx,esi ; call Shutdown ; fld dword ptr [pool] ; fdiv dword ptr [esi+70h] ; call __ftol ; sub edi,eax ; mov [esi+78h],edi ; done: lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CScopedLock ; ...
// The edi save is sunk into the armed path; the pool constant is -1000.0f. The manifest label is
// propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount();   // IAT 0x0143FDFC

struct CTarget_00c092d0 { void Finish(); };   // 0x00C15C70

struct CPixelShader_00c092d0 {
    unsigned char _pad[0x60];
    CTarget_00c092d0* m_pTarget;           // +0x60
    unsigned char _pad2[0xc];
    float m_rate;                          // +0x70
    long m_armed;                          // +0x74
    unsigned long m_deadline;              // +0x78
    void Shutdown();                       // 0x00C091F0
    void Close();
};

void CPixelShader_00c092d0::Close()
{
    CScopedLock lock(&g_lock_0143bb14);
    if (m_pTarget)
        m_pTarget->Finish();
    if (!m_armed)
        return;
    unsigned long now = GetTickCount();
    if (now > m_deadline) {
        Shutdown();
        m_deadline = now - (long)(-1000.0f / m_rate);
    }
}
