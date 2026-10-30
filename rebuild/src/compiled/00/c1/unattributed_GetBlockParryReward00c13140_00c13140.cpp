// CTCHeroExperience_00c13140::GetBlockParryReward(void*, void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the entry found out of line with the three arguments and 0 (@ 0x00C12CD0) yields its value at +0xc, else -1; the result survives the lock's destructor in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13140, oracle length 122 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[c] ; mov ecx,[b] ; mov edx,[a] ; push 0 ; push eax ; push ecx ; push edx ; mov ecx,esi ; mov dword ptr [esp+24h],0 ; call Find ; test eax,eax ; je miss ; mov esi,[eax+0Ch] ; jmp done ; miss: or esi,-1 ; done: lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 0Ch
// The manifest label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CRewardEntry_00c13140 {
    unsigned char _pad[0xc];
    long m_value;                          // +0xc
};

struct CTCHeroExperience_00c13140 {
    CRewardEntry_00c13140* Find(void* a, void* b, void* c, long flags);   // 0x00C12CD0
    long GetBlockParryReward(void* a, void* b, void* c);
};

long CTCHeroExperience_00c13140::GetBlockParryReward(void* a, void* b, void* c)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CRewardEntry_00c13140* entry = Find(a, b, c, 0);
    long value = entry ? entry->m_value : -1;
    return value;
}