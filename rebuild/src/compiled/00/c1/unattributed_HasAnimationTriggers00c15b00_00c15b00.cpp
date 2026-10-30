// CAnimateThing_00c15b00::HasAnimationTriggers() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, true when there is no target at +8, else the target's out-of-line query (@ 0x00C2D160) kept in bl across the lock's destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15B00, oracle length 133 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+8] ; test ecx,ecx ; mov dword ptr [esp+14h],0 ; pop esi ; jne query ; lea ecx,[esp] ; mov dword ptr [esp+10h],-1 ; call ~PKCProtectionInstance ; mov al,1 ; mov ecx,[esp+8] ; mov fs:[0],ecx ; add esp,14h ; ret ; query: push ebx ; call CTarget::Query ; lea ecx,[esp+4] ; mov bl,al ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; mov al,bl ; pop ebx ; mov fs:[0],ecx ; add esp,14h ; ret
// esi is popped before the branch and ebx pushed only on the query path (the register save is
// sunk into the path that needs it). The manifest label is propagated; the class is named by
// address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CTarget_00c15b00 {
    bool Query();                          // 0x00C2D160
};

struct CAnimateThing_00c15b00 {
    unsigned char _pad[8];
    CTarget_00c15b00* m_pTarget;           // +8
    bool HasAnimationTriggers();
};

bool CAnimateThing_00c15b00::HasAnimationTriggers()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CTarget_00c15b00* target = m_pTarget;
    if (!target)
        return true;
    return target->Query();
}