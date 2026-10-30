// CSoundEventManager::CountWaitingEvents() -- EH-framed: under the module lock (critical section @ 0x0143BB14), counts the events of the pool at +0x5c (count at +0x60, element accessor @ 0x00C13620) that are active (@ 0x00C20840) and waiting (@ 0x00C20EF0, state 2..4); the counter lives in the frame
// Leaf: ?CountWaitingEvents@CSoundEventManager@@QAEKXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C125E0, oracle length 148 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,0Ch ; push ebx ; push esi ; mov ebx,ecx ; push offset g_section ; lea ecx,[esp+10h] ; call CScopedLock::CScopedLock ; mov eax,[ebx+60h] ; xor esi,esi ; cmp eax,esi ; mov [esp+1Ch],esi ; mov [esp+8],esi ; jbe done ; push ebp ; push edi ; lea ebp,[ebx+5Ch] ; loop: push esi ; mov ecx,ebp ; call CSoundEventPool::Get ; mov edi,eax ; mov ecx,edi ; call CSoundEvent::IsActive ; test eax,eax ; je next ; mov ecx,edi ; call CSoundEvent::IsWaiting ; test eax,eax ; je next ; inc dword ptr [esp+10h] ; next: mov eax,[ebx+60h] ; inc esi ; cmp esi,eax ; jb loop ; pop edi ; pop ebp ; done: lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~CScopedLock ; mov ecx,[esp+14h] ; mov eax,[esp+8] ; ...
// Identity (sweep1, 2026-10-25): vtable 0x012B2748 slot 30 (0x012B27C0, its only reference) of the libpk sound-event
//   manager, between QueryBinding (slot 29, 0x00C11C00) and CountPlayingEvents (slot 31, 0x00C12680, the same walk
//   with IsPlayingPhysical/IsPlayingVirtual). The callees are the CSoundEvent state tests: IsActive @ 0x00C20840 (state
//   word +0xAC != 0) and the landed CSoundEvent::IsWaiting @ 0x00C20EF0 (state 2, 3 or 4: the describer 0x00C21040's
//   "Waiting for ..." states). Naming: libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEvent (0xB4 bytes);
//   descriptive method name (no PDB covers libpk).
// Label correction: the old row named the class CInventory_00c125e0 and the method CountSelectedBindings (callees
//   "IsBound"/"IsSelected") while its `// Leaf:` named the manifest's propagated "GetTexturesInUse", which matched no
//   symbol (the grade fell back to the largest function); the catalog's "CVtable_012b2748::vfunc_30" is the slot.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;  // 0x0143BB14: the module's critical section

class PKCProtectionInstance { public:               // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);    // 0x00C0FFC0
    ~PKCProtectionInstance();                // 0x00C0FFE0
};

struct CSoundEvent {                        // 0xb4 bytes
    long IsActive();                        // 0x00C20840
    long IsWaiting();                       // 0x00C20EF0 (state 2..4)
};

struct CSoundEventPool {                    // +0x5c: PKCFrallocStatic<CSoundEvent>
    long m_0;
    unsigned long m_count;                  // +4 (= +0x60)
    CSoundEvent* Get(unsigned long index);  // 0x00C13620
};

struct CSoundEventManager {
    unsigned char _pad[0x5c];               // vtable 0x012B2748, the event set at +0x2c
    CSoundEventPool m_events;               // +0x5c
    unsigned long CountWaitingEvents();
};

unsigned long CSoundEventManager::CountWaitingEvents()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    unsigned long count = 0;
    for (unsigned long i = 0; i < m_events.m_count; ++i) {
        CSoundEvent* event = m_events.Get(i);
        if (event->IsActive() && event->IsWaiting())
            ++count;
    }
    return count;
}
