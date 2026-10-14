// CTCHero_00c12140::ReleaseTarget(target) -- EH-framed under the module's profiling lock (0x0143BB14; @ 0x00C0FFC0 / 0x00C0FFE0, state 0; the manifest's "CTCHero_SaveDataToOutputStream" is propagated): every action of the +0x5c list (count at +0x60, element @ 0x00C13620) that is active (word +0xac @ 0x00C20840, a four-byte result) and whose +0x18 record points at the target through its +4 is interrupted (@ 0x00C21620); every key of the target's +0x2c table (count +0x34) is looked up in the registry at +0x284 (slot 17, the key passed by address from the dead argument slot) and a hit detached (@ 0x00C26390) and deleted (empty out-of-line destructor @ 0x00C26400); the +0x2c list drops the target (@ 0x00C22260) and every remaining item found for it (@ 0x00C21F50) is reset (@ 0x00C26580) and its +0x20 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12140, oracle length 291 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,8 ; push ebx ; push ebp ; push esi ; push edi ; mov ebp,ecx ; push offset g_section ; lea ecx,[esp+14h] ; call CProfileLock::CProfileLock ; mov eax,[ebp+60h] ; mov edi,[esp+28h] ; xor ebx,ebx ; cmp eax,ebx ; mov [esp+20h],ebx ; jbe keys ; acts: push ebx ; lea ecx,[ebp+5Ch] ; call CActionList::GetAt ; mov esi,eax ; mov ecx,esi ; call CAction::IsActive ; test eax,eax ; je nexta ; mov eax,[esi+18h] ; test eax,eax ; je nexta ; cmp [eax+4],edi ; jne nexta ; push ebp ; mov ecx,esi ; call CAction::Interrupt ; nexta: mov eax,[ebp+60h] ; inc ebx ; cmp ebx,eax ; jb acts ; keys: mov eax,[edi+34h] ; xor ebx,ebx ; test eax,eax ; jbe drop ; jmp keyloop ; keyloop: mov eax,[edi+2Ch] ; mov ecx,[eax+ebx*4] ; mov edx,[ebp+284h] ; mov [esp+28h],ecx ; lea ecx,[ebp+284h] ; lea eax,[esp+28h] ; push eax ; call [edx+44h] ; mov esi,eax ; test esi,esi ; je nextk ; push ebp ; mov ecx,esi ; call CEntry::Detach ; mov ecx,esi ; call CEntry::~CEntry ; push esi ; call operator delete ; add esp,4 ; nextk: mov eax,[edi+34h] ; inc ebx ; cmp ebx,eax ; jb keyloop ; drop: lea ebx,[ebp+2Ch] ; push edi ; mov ecx,ebx ; call CItemList::Remove ; push edi ; mov ecx,ebx ; call CItemList::FindNext ; mov esi,eax ; test esi,esi ; je done ; items: push ebp ; mov ecx,esi ; call CItem::Reset ; push edi ; mov ecx,ebx ; mov dword ptr [esi+20h],0 ; call CItemList::FindNext ; mov esi,eax ; test esi,esi ; jne items ; done: lea ecx,[esp+10h] ; mov dword ptr [esp+20h],-1 ; call ~CProfileLock ; ... ret 4
// The classes are named by address.
void __cdecl operator delete(void* block);
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section
struct CTCHero_00c12140;

struct CProfileLock_00c12140 {             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CProfileLock_00c12140(void* section);  // 0x00C0FFC0
    ~CProfileLock_00c12140();              // 0x00C0FFE0
};

struct CTarget_00c12140 {
    unsigned char _pad[0x2c];
    void** m_pKeys;                        // +0x2c
    long m_30;
    unsigned long m_keyCount;              // +0x34
};

struct SActionRecord_00c12140 { long m_0; CTarget_00c12140* m_pTarget; };
struct CAction_00c12140 {
    unsigned char _pad[0x18];
    SActionRecord_00c12140* m_pRecord;     // +0x18
    long IsActive();                       // 0x00C20840: non-zero when the word at +0xac is set
    void Interrupt(CTCHero_00c12140* hero);   // 0x00C21620
};
struct CActionList_00c12140 {              // +0x5c: element accessor, count at +4
    long m_0;
    unsigned long m_count;                 // +0x60
    CAction_00c12140* GetAt(unsigned long index);   // 0x00C13620
};

struct CEntry_00c12140 {
    void Detach(CTCHero_00c12140* hero);   // 0x00C26390
    ~CEntry_00c12140();                    // 0x00C26400 (empty, out of line)
};
struct CRegistry_00c12140 {                // +0x284: slot 17 finds an entry by key
    virtual void Slot00(); virtual void Slot04(); virtual void Slot08(); virtual void Slot0C();
    virtual void Slot10(); virtual void Slot14(); virtual void Slot18(); virtual void Slot1C();
    virtual void Slot20(); virtual void Slot24(); virtual void Slot28(); virtual void Slot2C();
    virtual void Slot30(); virtual void Slot34(); virtual void Slot38(); virtual void Slot3C();
    virtual void Slot40();
    virtual CEntry_00c12140* Find(void* const* key);   // slot 0x44
};

struct CItem_00c12140 {
    unsigned char _pad[0x20];
    long m_20;
    void Reset(CTCHero_00c12140* hero);    // 0x00C26580
};
struct CItemList_00c12140 {                // +0x2c
    unsigned char _body[0x30];
    void Remove(CTarget_00c12140* target); // 0x00C22260
    CItem_00c12140* FindNext(CTarget_00c12140* target);   // 0x00C21F50
};

struct CTCHero_00c12140 {
    unsigned char _pad[0x2c];
    CItemList_00c12140 m_items;            // +0x2c
    CActionList_00c12140 m_actions;        // +0x5c
    unsigned char _pad2[0x284 - 0x64];
    CRegistry_00c12140 m_registry;         // +0x284
    void ReleaseTarget(CTarget_00c12140* target);
};

void CTCHero_00c12140::ReleaseTarget(CTarget_00c12140* target)
{
    CProfileLock_00c12140 lock(g_section_0143bb14);
    for (unsigned long i = 0; i < m_actions.m_count; ++i) {
        CAction_00c12140* action = m_actions.GetAt(i);
        if (action->IsActive() && action->m_pRecord && action->m_pRecord->m_pTarget == target)
            action->Interrupt(this);
    }
    for (unsigned long k = 0; k < target->m_keyCount; ++k) {
        void* key = target->m_pKeys[k];
        CEntry_00c12140* entry = m_registry.Find(&key);
        if (entry) {
            entry->Detach(this);
            delete entry;
        }
    }
    m_items.Remove(target);
    CItem_00c12140* item;
    while ((item = m_items.FindNext(target)) != 0) {
        item->Reset(this);
        item->m_20 = 0;
    }
}
