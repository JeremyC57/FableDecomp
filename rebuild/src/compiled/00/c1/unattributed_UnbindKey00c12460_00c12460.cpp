// CInventory_00c12460::UnbindKey(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, every bound entity of the array at +0x5c (count at +0x60, accessor @ 0x00C13620, bound test @ 0x00C20840) whose word at +0x10 equals the key is released with `this` (@ 0x00C21620); then every observer the observable at +0x2c yields for the key (@ 0x00C21FF0, re-queried after each) is detached from `this` (@ 0x00C26580)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12460, oracle length 186 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esi+60h] ; mov ebp,[esp+28h] ; xor ebx,ebx ; cmp eax,ebx ; mov [esp+20h],ebx ; jbe second ; loop: push ebx ; lea ecx,[esi+5Ch] ; call CArray::Get ; mov edi,eax ; mov ecx,edi ; call IsBound ; test eax,eax ; je next ; cmp [edi+0x10],ebp ; jne next ; push esi ; mov ecx,edi ; call Release ; next: mov eax,[esi+60h] ; inc ebx ; cmp ebx,eax ; jb loop ; second: lea edi,[esi+2Ch] ; push ebp ; mov ecx,edi ; call Find ; test eax,eax ; je done ; detach: push esi ; mov ecx,eax ; call Detach ; push ebp ; mov ecx,edi ; call Find ; test eax,eax ; jne detach ; done: ...
// The sibling of this body (0x00C12520) differs only in the compared entity word and the finder.
// The manifest label is a description; the class is named by address (the callee labels are
// propagated).
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CInventory_00c12460;
struct CEntity_00c12460 {
    unsigned char _pad[0x10];
    void* m_pKey;                          // +0x10
    long IsBound();                        // 0x00C20840
    void Release(CInventory_00c12460* inventory);   // 0x00C21620
};

struct CEntityArray_00c12460 {                // +0x5c
    long m_a;
    unsigned long m_count;                 // +4 (= +0x60)
    CEntity_00c12460* Get(unsigned long index);   // 0x00C13620
};

struct CObserver_00c12460 {
    void Detach(CInventory_00c12460* inventory);   // 0x00C26580
};

struct CObservable_00c12460 {                 // +0x2c
    long m_a;
    CObserver_00c12460* Find(void* key);      // 0x00C21FF0
};

struct CInventory_00c12460 {
    unsigned char _pad[0x2c];
    CObservable_00c12460 m_observable;        // +0x2c
    unsigned char _pad2[0x2c];
    CEntityArray_00c12460 m_entities;         // +0x5c
    void UnbindKey(void* key);
};

void CInventory_00c12460::UnbindKey(void* key)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    for (unsigned long i = 0; i < m_entities.m_count; ++i) {
        CEntity_00c12460* entity = m_entities.Get(i);
        if (entity->IsBound() && entity->m_pKey == key)
            entity->Release(this);
    }
    for (CObserver_00c12460* observer = m_observable.Find(key); observer; observer = m_observable.Find(key))
        observer->Detach(this);
}