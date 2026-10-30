// CPhysicsMeshObject_00c14970::HasContainmentVolume(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the object at +8 is queried out of line (@ 0x00C2A320) with the argument; the byte result survives the lock's destructor in bl
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14970, oracle length 103 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push ebx ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[value] ; mov ecx,[esi+8] ; push eax ; mov dword ptr [esp+1Ch],0 ; call CTarget::Has ; lea ecx,[esp+8] ; mov bl,al ; mov dword ptr [esp+18h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+10h] ; pop esi ; mov al,bl ; pop ebx ; mov fs:[0],ecx ; add esp,14h ; ret 4
// The scoped-lock shape with a bool result. The manifest label is propagated; the class is
// named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CTarget_00c14970 {
    bool Has(void* value);                 // 0x00C2A320
};

struct CPhysicsMeshObject_00c14970 {
    unsigned char _pad[8];
    CTarget_00c14970* m_pTarget;           // +8
    bool HasContainmentVolume(void* value);
};

bool CPhysicsMeshObject_00c14970::HasContainmentVolume(void* value)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    return m_pTarget->Has(value);
}