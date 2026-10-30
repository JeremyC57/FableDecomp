// CTCCameraPointFixedPoint_00c2c590::GetLeafDataFromWindow(void*, bool) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the `__fastcall` reader @ 0x00C0AEE0 fills the two 16-bit members at +0x8 and +0xa from the window
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2C590, oracle length 105 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov dl,[flag] ; mov ecx,[window] ; lea eax,[esi+0x8] ; push eax ; add esi,0xa ; push esi ; mov dword ptr [esp+1Ch],0 ; call ReadLeafData ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 8
// The reader is a `__fastcall` free function (window in ecx, flag in dl, the two output addresses
// on the stack). The class comes from the manifest label; the members are descriptive.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

void __fastcall ReadLeafData_00c2c590(void* window, bool flag, short* second, short* first);   // 0x00C0AEE0

struct CTCCameraPointFixedPoint_00c2c590 {
    unsigned char _pad[0x8];
    short m_first;                         // +0x8
    short m_second;                        // +0xa
    void GetLeafDataFromWindow(void* window, bool flag);
};

void CTCCameraPointFixedPoint_00c2c590::GetLeafDataFromWindow(void* window, bool flag)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    ReadLeafData_00c2c590(window, flag, &m_second, &m_first);
}