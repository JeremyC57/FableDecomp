// CSoundEventManager::Update() -- EH-framed per-tick update under a scoped lock over the global @ 0x0143BB14: every active event of the pool at +0x5c (count at +0x60, accessor @ 0x00C13620, CSoundEvent::IsActive @ 0x00C20840) is applied with `this` and the flags at +0x7c8 (@ 0x00C21A50); then the flags are cleared, the pending events assigned (AssignPendingItems @ 0x00C10CD0), the step @ 0x00C12040 run and the owned part at +0x7bc finished (@ 0x00C1C6A0)
// Leaf: ?Update@CSoundEventManager@@QAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12880, oracle length 179 bytes. Built under /EHsc.
// Retail bytes:  ... loop: push edi ; mov ecx,ebp ; call CArray::Get ; mov ebx,eax ; mov ecx,ebx ; call IsActive ; test eax,eax ; je next ; mov eax,[esi+7C8h] ; push eax ; push esi ; mov ecx,ebx ; call Apply ; next: mov eax,[esi+60h] ; inc edi ; cmp edi,eax ; jb loop ; pop ebp ; pop ebx ; done: mov ecx,esi ; mov dword ptr [esi+7C8h],0 ; call AssignPendingItems ; mov ecx,esi ; call Rebuild ; mov ecx,[esi+7BCh] ; call COwner::Finish ; ...
// Naming (sweep1, 2026-10-25): was landed as _unattributed::CSoundEventManager::Reset (manifest
//   "CV_BordelloScript_OnExitBrothel", propagated); descriptive (no PDB covers this library). `this` is the libpk
//   CSoundEventManager (chunk 433/434; vtable 0x012B2748): the 0xb4-byte event pool at +0x5c / +0x60 with the
//   bounds-checked accessor 0x00C13620 and CSoundEvent::IsActive 0x00C20840 (`state != 0`, the +0xac state word) is
//   the one the landed UpdateItems 0x00C11CB0 / CountPlayingEvents 0x00C12680 walk; 0x00C10CD0 is the landed
//   CSoundEventManager::AssignPendingItems; +0x7c8 is the flag word the landed SetFlagBit1 0x00C11180 sets. Its caller
//   is the audio owner's per-tick CAudioOwner::Update 0x00C091F0 (on the owner's +0x38 manager). The +0x7bc part and
//   the 0x00C12040 / 0x00C21A50 callees keep descriptive placeholder names.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CSoundEventManager;
struct CSoundEvent {
    long IsActive();                       // 0x00C20840
    void Apply(CSoundEventManager* manager, unsigned long flags);   // 0x00C21A50
};

struct CSoundEventPool {             // +0x5c
    long m_a;
    unsigned long m_count;                 // +4 (= +0x60)
    CSoundEvent* Get(unsigned long index);   // 0x00C13620
    unsigned long GetCount() const { return m_count; }
};

struct COwner_00c12880 { void Finish(); };   // 0x00C1C6A0

struct CSoundEventManager {
    unsigned char _pad[0x5c];
    CSoundEventPool m_events;               // +0x5c
    unsigned char _pad2[0x758];
    COwner_00c12880* m_pOwner;             // +0x7bc
    unsigned char _pad3[8];
    unsigned long m_flags;                 // +0x7c8
    void AssignPendingItems();             // 0x00C10CD0
    void Rebuild();                        // 0x00C12040
    void Update();
};

void CSoundEventManager::Update()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    for (unsigned long i = 0; i < m_events.GetCount(); ++i) {
        CSoundEvent* event = m_events.Get(i);
        if (event->IsActive())
            event->Apply(this, m_flags);
    }
    m_flags = 0;
    AssignPendingItems();
    Rebuild();
    m_pOwner->Finish();
}