// CItemManager_00c26de0::EquipItem(CItem*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the item is equipped with the owner at +4 and its slot byte at +0x16 (@ 0x00C26090); when it has a node at +0 the node's word at +0x1c records whether the result was not negative and the owner's theme elements are updated for that node (@ 0x00C11950); a negative result detaches the item from the owner (@ 0x00C26390) and deletes it (destructor @ 0x00C26400, then the global operator delete)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C26DE0, oracle length 169 bytes. Built under /EHsc.
// Retail bytes:  ... mov esi,[item] ; movsx eax,byte ptr [esi+16h] ; mov ecx,[edi+4] ; push eax ; push ecx ; mov ecx,esi ; mov dword ptr [esp+24h],0 ; call CItem::Equip ; mov ebx,eax ; mov eax,[esi] ; test eax,eax ; je skip ; xor edx,edx ; test ebx,ebx ; setge dl ; push eax ; mov [eax+1Ch],dx ; mov ecx,[edi+4] ; call COwner::UpdateAllThemeElements ; skip: test ebx,ebx ; jge done ; mov eax,[edi+4] ; push eax ; mov ecx,esi ; call CItem::Detach ; mov ecx,esi ; call ~CItem ; push esi ; call operator delete ; add esp,4 ; done: ...
// The signed byte slot is sign-extended for the long parameter; the `>= 0` flag is widened to the
// 16-bit word. The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CNode_00c26de0 {
    unsigned char _pad[0x1c];
    short m_equipped;                      // +0x1c
};

struct COwner_00c26de0 {
    void UpdateAllThemeElements(CNode_00c26de0* node);   // 0x00C11950
};

struct CItem_00c26de0 {
    CNode_00c26de0* m_pNode;               // +0
    unsigned char _pad[0x12];
    signed char m_slot;                    // +0x16
    long Equip(COwner_00c26de0* owner, long slot);   // 0x00C26090
    void Detach(COwner_00c26de0* owner);   // 0x00C26390
    ~CItem_00c26de0();                     // 0x00C26400
};

struct CItemManager_00c26de0 {
    unsigned char _pad[4];
    COwner_00c26de0* m_pOwner;             // +4
    void EquipItem(CItem_00c26de0* item);
};

void CItemManager_00c26de0::EquipItem(CItem_00c26de0* item)
{
    CScopedLock lock(&g_lock_0143bb14);
    long result = item->Equip(m_pOwner, item->m_slot);
    CNode_00c26de0* node = item->m_pNode;
    if (node) {
        node->m_equipped = result >= 0;
        m_pOwner->UpdateAllThemeElements(node);
    }
    if (result < 0) {
        item->Detach(m_pOwner);
        delete item;
    }
}
