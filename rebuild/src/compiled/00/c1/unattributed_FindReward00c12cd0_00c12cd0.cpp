// CTCHeroExperience_00c12cd0::FindReward(CRewardKey*, void*, void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, forwards to the six-argument lookup (@ 0x00C12940) with the key's second word, the address of its third, then b, the key, c and d; the result survives the lock's destructor in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12CD0, oracle length 123 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[d] ; mov ecx,[c] ; mov edx,[b] ; push eax ; mov eax,[a] ; push ecx ; push eax ; push edx ; mov edx,[eax+4] ; lea ecx,[eax+8] ; push ecx ; push edx ; mov ecx,esi ; mov dword ptr [esp+2Ch],0 ; call Lookup ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 10h
// The callee of the -1-defaulting reward query at 0x00C13140. The manifest label is propagated;
// the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CRewardKey_00c12cd0 {
    long m_kind;                           // +0
    long m_id;                             // +4
    long m_data;                           // +8
};

struct CRewardEntry_00c12cd0;

struct CTCHeroExperience_00c12cd0 {
    CRewardEntry_00c12cd0* Lookup(long id, long* data, void* b, CRewardKey_00c12cd0* key, void* c, void* d);   // 0x00C12940
    CRewardEntry_00c12cd0* FindReward(CRewardKey_00c12cd0* key, void* b, void* c, void* d);
};

CRewardEntry_00c12cd0* CTCHeroExperience_00c12cd0::FindReward(CRewardKey_00c12cd0* key, void* b, void* c, void* d)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    return Lookup(key->m_id, &key->m_data, b, key, c, d);
}