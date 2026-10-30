// CActionSummonerLightning_00c14dd0::Advance() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, an out-of-line step on `this` (@ 0x00C14D80), updates of the objects at +8 (@ 0x00C2A890) and +0xc (@ 0x00C2BCA0), and the counter at +0x70 incremented
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14DD0, oracle length 112 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,esi ; mov dword ptr [esp+14h],0 ; call Step ; mov ecx,[esi+8] ; call CFirst::Update ; mov ecx,[esi+0Ch] ; call CSecond::Update ; mov eax,[esi+70h] ; inc eax ; lea ecx,[esp+4] ; mov [esi+70h],eax ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The manifest label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CFirst_00c14dd0 { void Update(); };    // 0x00C2A890
struct CSecond_00c14dd0 { void Update(); };   // 0x00C2BCA0

struct CActionSummonerLightning_00c14dd0 {
    unsigned char _pad[8];
    CFirst_00c14dd0* m_pFirst;             // +8
    CSecond_00c14dd0* m_pSecond;           // +0xc
    unsigned char _pad2[0x60];
    long m_frame;                          // +0x70
    void Step();                           // 0x00C14D80
    void Advance();
};

void CActionSummonerLightning_00c14dd0::Advance()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    Step();
    m_pFirst->Update();
    m_pSecond->Update();
    ++m_frame;
}