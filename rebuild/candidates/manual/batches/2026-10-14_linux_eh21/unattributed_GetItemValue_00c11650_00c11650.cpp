// CInventory_00c11650::GetItemValue(void*, float*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, when the observable member at +0x2c does not know the key (@ 0x00C21E20) the out word is zeroed and the default value @ 0x0122DED8 (1.0f) returned, else the observer's virtual slot 9 (with `this` and the out pointer) yields the float, parked in the dead out-pointer slot across the lock's destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11650, oracle length 165 bytes. Built under /EHsc.
// Retail bytes:  ... call CObservable::Find ; test eax,eax ; mov ecx,[out] ; jne hit ; mov [ecx],eax ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; fld dword ptr [pool] ; pop esi ; ... ret 8 ; hit: mov edx,[eax] ; push ecx ; push esi ; mov ecx,eax ; call [edx+24h] ; fstp dword ptr [out] ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; fld dword ptr [out] ; ...
// The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CInventory_00c11650;
struct CObserver_00c11650 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual float GetValue(CInventory_00c11650* source, float* out);   // slot 9
};

struct CObservable_00c11650 {
    long m_a;
    CObserver_00c11650* Find(void* key);   // 0x00C21E20
};

// the pool @ 0x0122DED8 holds 1.0f

struct CInventory_00c11650 {
    unsigned char _pad[0x2c];
    CObservable_00c11650 m_observable;     // +0x2c
    float GetItemValue(void* key, float* out);
};

float CInventory_00c11650::GetItemValue(void* key, float* out)
{
    CScopedLock lock(&g_lock_0143bb14);
    CObserver_00c11650* observer = m_observable.Find(key);
    if (!observer) {
        *out = 0;
        return 1.0f;
    }
    return observer->GetValue(this, out);
}
