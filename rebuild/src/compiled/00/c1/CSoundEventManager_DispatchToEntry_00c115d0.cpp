// CSoundEventManager::DispatchToEntry(void* key, void* argument) -- vtable 0x012B2748 slot 24: under a scoped lock over the module section @ 0x0143BB14, the event set at +0x2c looks the key up (@ 0x00C21E20) and the entry found, if any, is notified through its virtual slot 7 with (this, argument); EH-framed
// Leaf: ?DispatchToEntry@CSoundEventManager@@QAEXPAX0@Z
// Flags: /EHsc
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c1/unattributed_CTCInventoryProcessButtonAReleasec115d000c115d0_00c115d0.cpp).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C115D0, oracle length 114 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[key] ; push eax ; lea ecx,[esi+0x2c] ; mov dword ptr [esp+18h],0 ; call Find ; test eax,eax ; je done ; mov ecx,[argument] ; mov edx,[eax] ; push ecx ; push esi ; mov ecx,eax ; call [edx+0x1c] ; done: lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 8
// Observer notification under the global observer lock: an 8-byte scoped lock is built over the
// global lock object @ 0x0143BB14 (EH state 0 while it lives), the observable member at
// +0x2c looks the key up out of line (@ 0x00C21E20) and the observer found, if any, is
// notified through its virtual slot 7 with the source object and the argument; the lock is
// destroyed (state -1). Built under /EHsc (the `__ehhandler` is not compared; the funclet lands
// through the funclet pass). Renamed (sweep1, 2026-10-25) from
// _unattributed::CTCInventory_ProcessButtonARelease_c115d0 (the manifest label is propagated): this body is slot 24 of the
// CSoundEventManager vtable 0x012B2748 (0x012B27A8), and the +0x2c member is the manager's event set; the method name is
// descriptive.
// One of 3 bodies of this shape.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;   // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CObserver {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void Notify(void* source, void* argument);   // slot 7
};

struct CSoundEventSet {
    long m_a;
    CObserver* Find(void* key);            // 0x00C21E20
};

struct CSoundEventManager {
    unsigned char _pad[0x2c];
    CSoundEventSet m_eventSet;              // +0x2c
    void DispatchToEntry(void* key, void* argument);
};

void CSoundEventManager::DispatchToEntry(void* key, void* argument)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CObserver* observer = m_eventSet.Find(key);
    if (observer)
        observer->Notify(this, argument);
}