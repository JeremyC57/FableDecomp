// CInventory_00c125e0::CountSelectedBindings() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, counts the entities of the array at +0x5c (count at +0x60, element accessor @ 0x00C13620) that are bound (@ 0x00C20840) and selected (@ 0x00C20EF0); the counter lives in the frame
// Leaf: GetTexturesInUse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C125E0, oracle length 148 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,0Ch ; push ebx ; push esi ; mov ebx,ecx ; push offset g_lock ; lea ecx,[esp+10h] ; call CScopedLock::CScopedLock ; mov eax,[ebx+60h] ; xor esi,esi ; cmp eax,esi ; mov [esp+1Ch],esi ; mov [esp+8],esi ; jbe done ; push ebp ; push edi ; lea ebp,[ebx+5Ch] ; loop: push esi ; mov ecx,ebp ; call CArray::Get ; mov edi,eax ; mov ecx,edi ; call CEntity::IsBound ; test eax,eax ; je next ; mov ecx,edi ; call CEntity::IsSelected ; test eax,eax ; je next ; inc dword ptr [esp+10h] ; next: mov eax,[ebx+60h] ; inc esi ; cmp esi,eax ; jb loop ; pop edi ; pop ebp ; done: lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~CScopedLock ; mov ecx,[esp+14h] ; mov eax,[esp+8] ; ...
// The sibling of the binding walks at 0x00C12270 / 0x00C120A0 returning a count. The manifest
// label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CEntity_00c125e0 {
    long IsBound();                        // 0x00C20840
    long IsSelected();                     // 0x00C20EF0
};

struct CEntityArray_00c125e0 {             // +0x5c
    long m_a;
    unsigned long m_count;                 // +4 (= +0x60)
    CEntity_00c125e0* Get(unsigned long index);   // 0x00C13620
};

struct CInventory_00c125e0 {
    unsigned char _pad[0x5c];
    CEntityArray_00c125e0 m_entities;      // +0x5c
    unsigned long CountSelectedBindings();
};

unsigned long CInventory_00c125e0::CountSelectedBindings()
{
    CScopedLock lock(&g_lock_0143bb14);
    unsigned long count = 0;
    for (unsigned long i = 0; i < m_entities.m_count; ++i) {
        CEntity_00c125e0* entity = m_entities.Get(i);
        if (entity->IsBound() && entity->IsSelected())
            ++count;
    }
    return count;
}