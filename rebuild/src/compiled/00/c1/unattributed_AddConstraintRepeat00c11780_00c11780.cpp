// CParticleProgram_00c11780::AddConstraintRepeat(void*) -- under a scoped lock over the global @ 0x0143BB14 (ctor @ 0x00C0FFC0, dtor @ 0x00C0FFE0), the observable member at +0x7d8 adds the observer (@ 0x00C25E20); EH-framed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11780, oracle length 100 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[observer] ; push eax ; lea ecx,[esi+0x7d8] ; mov dword ptr [esp+18h],0 ; call AddObserver ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// Observer registration under the global observer lock: an 8-byte scoped lock is built over the
// global lock object @ 0x0143BB14 (EH state 0 while it lives), the observable member at
// +0x7d8 adds the observer out of line (@ 0x00C25E20), and the lock is destroyed (state -1).
// Built under /EHsc (the `__ehhandler` is not compared; the funclet lands through the funclet
// pass). The manifest label is propagated; the class is named by address. One of 3
// bodies of this shape.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_observerLock;   // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CObservable {
    long m_a;
    void AddObserver(void* observer);      // 0x00C25E20
};

struct CParticleProgram_00c11780 {
    unsigned char _pad[0x7d8];
    CObservable m_observable;              // +0x7d8
    void AddConstraintRepeat_00c11780(void* observer);
};

void CParticleProgram_00c11780::AddConstraintRepeat_00c11780(void* observer)
{
    CScopedLock lock(&g_observerLock);
    m_observable.AddObserver(observer);
}