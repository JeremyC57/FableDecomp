// CInventory_00c120a0::ProcessKeyboardBinding(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, every entity of the array at +0x5c (count at +0x60, element accessor @ 0x00C13620) that is bound (@ 0x00C20840) and whose key at +0x18 is the argument is processed with `this` (@ 0x00C21620); the count is re-read each iteration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C120A0, oracle length 148 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push ebx ; push edi ; mov ebx,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[ebx+60h] ; xor edi,edi ; cmp eax,edi ; mov [esp+18h],edi ; jbe done ; push ebp ; push esi ; lea ebp,[ebx+5Ch] ; loop: push edi ; mov ecx,ebp ; call CArray::Get ; mov esi,eax ; mov ecx,esi ; call CEntity::IsBound ; test eax,eax ; je next ; mov eax,[key] ; cmp [esi+18h],eax ; jne next ; push ebx ; mov ecx,esi ; call CEntity::Process ; next: mov eax,[ebx+60h] ; inc edi ; cmp edi,eax ; jb loop ; pop esi ; pop ebp ; done: lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~PKCProtectionInstance ; ...
// The sibling of the creator walk at 0x00C12270 with an extra bound test; the ebp/esi saves are
// sunk into the loop path. The manifest label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CInventory_00c120a0;
struct CEntity_00c120a0 {
    unsigned char _pad[0x18];
    void* m_pKey;                          // +0x18
    long IsBound();                        // 0x00C20840
    void Process(CInventory_00c120a0* inventory);   // 0x00C21620
};

struct CEntityArray_00c120a0 {             // +0x5c
    long m_a;
    unsigned long m_count;                 // +4 (= +0x60)
    CEntity_00c120a0* Get(unsigned long index);   // 0x00C13620
};

struct CInventory_00c120a0 {
    unsigned char _pad[0x5c];
    CEntityArray_00c120a0 m_entities;      // +0x5c
    void ProcessKeyboardBinding(void* key);
};

void CInventory_00c120a0::ProcessKeyboardBinding(void* key)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    for (unsigned long i = 0; i < m_entities.m_count; ++i) {
        CEntity_00c120a0* entity = m_entities.Get(i);
        if (entity->IsBound() && entity->m_pKey == key)
            entity->Process(this);
    }
}