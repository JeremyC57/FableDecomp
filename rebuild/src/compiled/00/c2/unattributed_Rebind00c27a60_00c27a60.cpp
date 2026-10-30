// CInventoryItem_00c27a60::Rebind(COwner*) -- EH-framed: unregisters from the owner's registry at +0x2c (@ 0x00C21EA0), runs its own virtual slot 5, then under a scoped lock over the global @ 0x0143BAFC releases the blocks at +0xc and +8 through two `__stdcall` helpers (@ 0x00C35980, @ 0x00C36090), zeroes +0xc, +8 and +4 and re-registers with the owner (@ 0x00C22040)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27A60, oracle length 143 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push ebx ; push esi ; push edi ; mov edi,[owner] ; mov esi,ecx ; add edi,2Ch ; push esi ; mov ecx,edi ; call CRegistry::Remove ; mov eax,[esi] ; mov ecx,esi ; call [eax+14h] ; push offset g_lock2 ; lea ecx,[esp+10h] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+0Ch] ; xor ebx,ebx ; push ecx ; mov [esp+20h],ebx ; call FreeC ; mov edx,[esi+8] ; push edx ; call Free8 ; push esi ; mov ecx,edi ; mov [esi+0Ch],ebx ; mov [esi+8],ebx ; mov [esi+4],ebx ; call CRegistry::Add ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~PKCProtectionInstance ; ...
// The lock is taken after the unregister and the virtual notification; the zero stores follow
// statement order (+0xc, +8, +4). The manifest label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

extern PKCCriticalSection g_soundLock;        // 0x0143BAFC
void __stdcall FreeBlockC_00c35980(void* block);  // 0x00C35980
void __stdcall FreeBlock8_00c36090(void* block);  // 0x00C36090

struct CInventoryItem_00c27a60;
struct CRegistry_00c27a60 {                // +0x2c of the owner
    void Remove(CInventoryItem_00c27a60* item);   // 0x00C21EA0
    void Add(CInventoryItem_00c27a60* item);      // 0x00C22040
};

struct COwner_00c27a60 {
    unsigned char _pad[0x2c];
    CRegistry_00c27a60 m_registry;         // +0x2c
};

struct CInventoryItem_00c27a60 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnRebind();               // slot 5
    void* m_pFirst;                        // +4
    void* m_pBlock8;                       // +8
    void* m_pBlockC;                       // +0xc
    void Rebind(COwner_00c27a60* owner);
};

void CInventoryItem_00c27a60::Rebind(COwner_00c27a60* owner)
{
    owner->m_registry.Remove(this);
    OnRebind();
    PKCProtectionInstance lock(&g_soundLock);
    FreeBlockC_00c35980(m_pBlockC);
    FreeBlock8_00c36090(m_pBlock8);
    m_pBlockC = 0;
    m_pBlock8 = 0;
    m_pFirst = 0;
    owner->m_registry.Add(this);
}