// CInventory_00c12880::Reset() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, every bound entity of the array at +0x5c (count at +0x60, accessor @ 0x00C13620, bound test @ 0x00C20840) is applied with `this` and the flags at +0x7c8 (@ 0x00C21A50); then the flags are cleared, the inventory refreshed (@ 0x00C10CD0) and rebuilt (@ 0x00C12040) and the owner at +0x7bc finished (@ 0x00C1C6A0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12880, oracle length 179 bytes. Built under /EHsc.
// Retail bytes:  ... loop: push edi ; mov ecx,ebp ; call CArray::Get ; mov ebx,eax ; mov ecx,ebx ; call IsBound ; test eax,eax ; je next ; mov eax,[esi+7C8h] ; push eax ; push esi ; mov ecx,ebx ; call Apply ; next: mov eax,[esi+60h] ; inc edi ; cmp edi,eax ; jb loop ; pop ebp ; pop ebx ; done: mov ecx,esi ; mov dword ptr [esi+7C8h],0 ; call Refresh ; mov ecx,esi ; call Rebuild ; mov ecx,[esi+7BCh] ; call COwner::Finish ; ...
// The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CInventory_00c12880;
struct CEntity_00c12880 {
    long IsBound();                        // 0x00C20840
    void Apply(CInventory_00c12880* inventory, unsigned long flags);   // 0x00C21A50
};

struct CEntityArray_00c12880 {             // +0x5c
    long m_a;
    unsigned long m_count;                 // +4 (= +0x60)
    CEntity_00c12880* Get(unsigned long index);   // 0x00C13620
    unsigned long GetCount() const { return m_count; }
};

struct COwner_00c12880 { void Finish(); };   // 0x00C1C6A0

struct CInventory_00c12880 {
    unsigned char _pad[0x5c];
    CEntityArray_00c12880 m_entities;      // +0x5c
    unsigned char _pad2[0x758];
    COwner_00c12880* m_pOwner;             // +0x7bc
    unsigned char _pad3[8];
    unsigned long m_flags;                 // +0x7c8
    void Refresh();                        // 0x00C10CD0
    void Rebuild();                        // 0x00C12040
    void Reset();
};

void CInventory_00c12880::Reset()
{
    CScopedLock lock(&g_lock_0143bb14);
    for (unsigned long i = 0; i < m_entities.GetCount(); ++i) {
        CEntity_00c12880* entity = m_entities.Get(i);
        if (entity->IsBound())
            entity->Apply(this, m_flags);
    }
    m_flags = 0;
    Refresh();
    Rebuild();
    m_pOwner->Finish();
}
