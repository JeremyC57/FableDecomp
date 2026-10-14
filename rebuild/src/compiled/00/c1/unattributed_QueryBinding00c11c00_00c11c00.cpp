// CInventory_00c11c00::QueryBinding(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, when the observable member at +0x2c knows the key (@ 0x00C21E20) and the observer accepts `this` (virtual slot 11, byte result) its value (virtual slot 10 with `this`) is returned, else the observer is reset with `this` (virtual slot 1); -1 when nothing applies
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11C00, oracle length 171 bytes. Built under /EHsc.
// Retail bytes:  ... call CObservable::Find ; mov esi,eax ; test esi,esi ; je miss ; mov edx,[esi] ; push edi ; mov ecx,esi ; call [edx+2Ch] ; test al,al ; mov ecx,esi ; push edi ; jne hit ; mov eax,[esi] ; call [eax+4] ; miss: lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CScopedLock ; pop edi ; or eax,-1 ; ... ret 4 ; hit: mov edx,[esi] ; call [edx+28h] ; lea ecx,[esp+8] ; mov esi,eax ; ...
// The `this` push is shared by the two slot calls (hoisted above the branch). The manifest
// label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CInventory_00c11c00;
struct CObserver_00c11c00 {
    virtual void v00();
    virtual void Reset(CInventory_00c11c00* source);     // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual long GetValue(CInventory_00c11c00* source);  // slot 10
    virtual bool Accepts(CInventory_00c11c00* source);   // slot 11
};

struct CObservable_00c11c00 {
    long m_a;
    CObserver_00c11c00* Find(void* key);   // 0x00C21E20
};

struct CInventory_00c11c00 {
    unsigned char _pad[0x2c];
    CObservable_00c11c00 m_observable;     // +0x2c
    long QueryBinding(void* key);
};

long CInventory_00c11c00::QueryBinding(void* key)
{
    CScopedLock lock(&g_lock_0143bb14);
    CObserver_00c11c00* observer = m_observable.Find(key);
    if (observer) {
        if (!observer->Accepts(this)) {
            observer->Reset(this);
            return -1;
        }
        return observer->GetValue(this);
    }
    return -1;
}