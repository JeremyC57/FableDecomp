// CTCHero_00c12270::OnEntityCreatedByScript(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, every entity of the array at +0x5c (count at +0x60, element accessor @ 0x00C13620) whose creator at +0xa4 is the argument is notified with `this` (@ 0x00C21700); the count is re-read each iteration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12270, oracle length 134 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; push edi ; mov edi,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[edi+60h] ; xor esi,esi ; cmp eax,esi ; mov [esp+18h],esi ; jbe done ; push ebx ; push ebp ; mov ebp,[creator] ; lea ebx,[edi+5Ch] ; loop: push esi ; mov ecx,ebx ; call CArray::Get ; cmp [eax+0A4h],ebp ; jne next ; push edi ; mov ecx,eax ; call CEntity::OnCreated ; next: mov eax,[edi+60h] ; inc esi ; cmp esi,eax ; jb loop ; pop ebp ; pop ebx ; done: lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~PKCProtectionInstance ; ...
// The ebx/ebp saves are sunk into the loop path. The EH state store shares the zero index
// register. The manifest label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CTCHero_00c12270;
struct CEntity_00c12270 {
    unsigned char _pad[0xa4];
    void* m_pCreator;                      // +0xa4
    void OnCreated(CTCHero_00c12270* hero);   // 0x00C21700
};

struct CEntityArray_00c12270 {             // +0x5c
    long m_a;
    unsigned long m_count;                 // +4 (= +0x60)
    CEntity_00c12270* Get(unsigned long index);   // 0x00C13620
};

struct CTCHero_00c12270 {
    unsigned char _pad[0x5c];
    CEntityArray_00c12270 m_entities;      // +0x5c
    void OnEntityCreatedByScript(void* creator);
};

void CTCHero_00c12270::OnEntityCreatedByScript(void* creator)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    for (unsigned long i = 0; i < m_entities.m_count; ++i) {
        CEntity_00c12270* entity = m_entities.Get(i);
        if (entity->m_pCreator == creator)
            entity->OnCreated(this);
    }
}