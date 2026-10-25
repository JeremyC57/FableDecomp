// CSoundEventManager::ReleaseTarget(target) -- EH-framed under the module's profiling lock (0x0143BB14; @ 0x00C0FFC0 / 0x00C0FFE0, state 0; the manifest's "CTCHero_SaveDataToOutputStream" is propagated): every event of the +0x5c pool (count at +0x60, element @ 0x00C13620) that is active (word +0xac @ 0x00C20840, a four-byte result) and whose +0x18 record points at the target through its +4 is stopped (CSoundEvent::Stop @ 0x00C21620); every key of the target's +0x2c table (count +0x34) is looked up in the registry at +0x284 (slot 17, the key passed by address from the dead argument slot) and a hit detached (@ 0x00C26390) and deleted (empty out-of-line destructor @ 0x00C26400); the +0x2c event set drops the target (@ 0x00C22260) and every remaining item found for it (@ 0x00C21F50) is reset (@ 0x00C26580) and its +0x20 cleared
// Leaf: ?ReleaseTarget@CSoundEventManager@@QAEXPAUCTarget_00c12140@@@Z
// Flags: /EHsc
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c1/CTCHero_CTCHeroReleaseTarget00c12140_00c12140.cpp).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12140, oracle length 291 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,8 ; push ebx ; push ebp ; push esi ; push edi ; mov ebp,ecx ; push offset g_section ; lea ecx,[esp+14h] ; call CProfileLock::CProfileLock ; mov eax,[ebp+60h] ; mov edi,[esp+28h] ; xor ebx,ebx ; cmp eax,ebx ; mov [esp+20h],ebx ; jbe keys ; acts: push ebx ; lea ecx,[ebp+5Ch] ; call CActionList::GetAt ; mov esi,eax ; mov ecx,esi ; call CAction::IsActive ; test eax,eax ; je nexta ; mov eax,[esi+18h] ; test eax,eax ; je nexta ; cmp [eax+4],edi ; jne nexta ; push ebp ; mov ecx,esi ; call CAction::Stop ; nexta: mov eax,[ebp+60h] ; inc ebx ; cmp ebx,eax ; jb acts ; keys: mov eax,[edi+34h] ; xor ebx,ebx ; test eax,eax ; jbe drop ; jmp keyloop ; keyloop: mov eax,[edi+2Ch] ; mov ecx,[eax+ebx*4] ; mov edx,[ebp+284h] ; mov [esp+28h],ecx ; lea ecx,[ebp+284h] ; lea eax,[esp+28h] ; push eax ; call [edx+44h] ; mov esi,eax ; test esi,esi ; je nextk ; push ebp ; mov ecx,esi ; call CEntry::Detach ; mov ecx,esi ; call CEntry::~CEntry ; push esi ; call operator delete ; add esp,4 ; nextk: mov eax,[edi+34h] ; inc ebx ; cmp ebx,eax ; jb keyloop ; drop: lea ebx,[ebp+2Ch] ; push edi ; mov ecx,ebx ; call CItemList::Remove ; push edi ; mov ecx,ebx ; call CItemList::FindNext ; mov esi,eax ; test esi,esi ; je done ; events: push ebp ; mov ecx,esi ; call CItem::Reset ; push edi ; mov ecx,ebx ; mov dword ptr [esi+20h],0 ; call CItemList::FindNext ; mov esi,eax ; test esi,esi ; jne events ; done: lea ecx,[esp+10h] ; mov dword ptr [esp+20h],-1 ; call ~CProfileLock ; ... ret 4
// Identity: renamed (sweep1, 2026-10-25) from CTCHero_00c12140::ReleaseTarget: `this` is the CSoundEventManager (vtable
//   0x012B2748): the +0x5C pool of 0xB4-byte CSoundEvents (accessor 0x00C13620, IsActive 0x00C20840, Stop 0x00C21620, as in
//   Shutdown 0x00C123B0) and the +0x2C event set; FINDINGS chunk 433 lists CTCHero among the manager's propagated names.
//   The other classes keep their address names.
void __cdecl operator delete(void* block);
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section
struct CSoundEventManager;

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
struct CSoundEvent {
    unsigned char _pad[0x18];
    SActionRecord_00c12140* m_pRecord;     // +0x18
    long IsActive();                       // 0x00C20840: non-zero when the word at +0xac is set
    void Stop(CSoundEventManager* manager);   // 0x00C21620
};
struct CSoundEventPool {              // +0x5c: element accessor, count at +4
    long m_0;
    unsigned long m_count;                 // +0x60
    CSoundEvent* GetAt(unsigned long index);   // 0x00C13620
};

struct CEntry_00c12140 {
    void Detach(CSoundEventManager* manager);   // 0x00C26390
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
    void Reset(CSoundEventManager* manager);    // 0x00C26580
};
struct CSoundEventSet {                // +0x2c
    unsigned char _body[0x30];
    void Remove(CTarget_00c12140* target); // 0x00C22260
    CItem_00c12140* FindNext(CTarget_00c12140* target);   // 0x00C21F50
};

struct CSoundEventManager {
    unsigned char _pad[0x2c];
    CSoundEventSet m_eventSet;            // +0x2c
    CSoundEventPool m_events;        // +0x5c
    unsigned char _pad2[0x284 - 0x64];
    CRegistry_00c12140 m_registry;         // +0x284
    void ReleaseTarget(CTarget_00c12140* target);
};

void CSoundEventManager::ReleaseTarget(CTarget_00c12140* target)
{
    CProfileLock_00c12140 lock(g_section_0143bb14);
    for (unsigned long i = 0; i < m_events.m_count; ++i) {
        CSoundEvent* event = m_events.GetAt(i);
        if (event->IsActive() && event->m_pRecord && event->m_pRecord->m_pTarget == target)
            event->Stop(this);
    }
    for (unsigned long k = 0; k < target->m_keyCount; ++k) {
        void* key = target->m_pKeys[k];
        CEntry_00c12140* entry = m_registry.Find(&key);
        if (entry) {
            entry->Detach(this);
            delete entry;
        }
    }
    m_eventSet.Remove(target);
    CItem_00c12140* item;
    while ((item = m_eventSet.FindNext(target)) != 0) {
        item->Reset(this);
        item->m_20 = 0;
    }
}