// CAIStateGroup_JackOfBladesBase_00c1c110::Shutdown() -- EH-framed: a first scoped lock over the global @ 0x0143BAFC around a C-linkage flush (@ 0x00C336A0, no EH state because it cannot throw), then a second lock over the global @ 0x0143BB14 around the owner's release (@ 0x00C11F50) and its device's close (@ 0x00C092D0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C110, oracle length 129 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,10h ; push esi ; mov esi,ecx ; push offset g_lock2 ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; call Flush ; lea ecx,[esp+4] ; call ~PKCProtectionInstance ; push offset g_lock ; lea ecx,[esp+10h] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+4] ; mov dword ptr [esp+1Ch],0 ; call COwner::Release ; mov eax,[esi+4] ; mov ecx,[eax+4] ; call CDevice::Close ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+14h] ; pop esi ; mov fs:[0],ecx ; add esp,1Ch ; ret
// Two lock objects in one frame; only the second region registers an EH state. The manifest
// label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

extern PKCCriticalSection g_soundLock;        // 0x0143BAFC
extern "C" void Flush_00c336a0();          // 0x00C336A0 (C linkage: cannot throw under /EHsc)

struct CDevice_00c1c110 { void Close(); };   // 0x00C092D0

struct COwner_00c1c110 {
    unsigned char _pad[4];
    CDevice_00c1c110* m_pDevice;           // +4
    void Release();                        // 0x00C11F50
};

struct CAIStateGroup_JackOfBladesBase_00c1c110 {
    unsigned char _pad[4];
    COwner_00c1c110* m_pOwner;             // +4
    void Shutdown();
};

void CAIStateGroup_JackOfBladesBase_00c1c110::Shutdown()
{
    {
        PKCProtectionInstance flushLock(&g_soundLock);
        Flush_00c336a0();
    }
    PKCProtectionInstance lock(&g_lock_0143bb14);
    m_pOwner->Release();
    m_pOwner->m_pDevice->Close();
}