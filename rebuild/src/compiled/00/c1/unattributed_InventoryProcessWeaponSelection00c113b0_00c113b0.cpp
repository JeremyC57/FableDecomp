// CObserved_00c113b0::Inventory_ProcessWeaponSelection(void*, void*) -- under a scoped lock over the global @ 0x0143BB14, finds the observer of the key in the observable member at +0x2c (@ 0x00C21E20) and notifies it through virtual slot 2 with (this, argument); EH-framed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C113B0, oracle length 114 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[key] ; push eax ; lea ecx,[esi+0x2c] ; mov dword ptr [esp+18h],0 ; call Find ; test eax,eax ; je done ; mov ecx,[argument] ; mov edx,[eax] ; push ecx ; push esi ; mov ecx,eax ; call [edx+0x8] ; done: lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 8
// Observer notification under the global observer lock: an 8-byte scoped lock is built over the
// global lock object @ 0x0143BB14 (EH state 0 while it lives), the observable member at
// +0x2c looks the key up out of line (@ 0x00C21E20) and the observer found, if any, is
// notified through its virtual slot 2 with the source object and the argument; the lock is
// destroyed (state -1). Built under /EHsc (the `__ehhandler` is not compared; the funclet lands
// through the funclet pass). The manifest label is propagated; the class is named by address.
// One of 3 bodies of this shape.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_observerLock;   // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CObserver {
    virtual void v00();
    virtual void v01();
    virtual void Notify(void* source, void* argument);   // slot 2
};

struct CObservable {
    long m_a;
    CObserver* Find(void* key);            // 0x00C21E20
};

struct CObserved_00c113b0 {
    unsigned char _pad[0x2c];
    CObservable m_observable;              // +0x2c
    void Inventory_ProcessWeaponSelection_00c113b0(void* key, void* argument);
};

void CObserved_00c113b0::Inventory_ProcessWeaponSelection_00c113b0(void* key, void* argument)
{
    CScopedLock lock(&g_observerLock);
    CObserver* observer = m_observable.Find(key);
    if (observer)
        observer->Notify(this, argument);
}