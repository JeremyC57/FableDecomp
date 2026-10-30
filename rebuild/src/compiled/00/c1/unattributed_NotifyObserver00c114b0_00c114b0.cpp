// CInventory_00c114b0::NotifyObserver(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the observable member at +0x2c looks the key up (@ 0x00C21E20) and the observer found is told through virtual slot 4 with `this`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C114B0, oracle length 109 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[key] ; push eax ; lea ecx,[esi+2Ch] ; mov dword ptr [esp+18h],0 ; call CObservable::Find ; test eax,eax ; je done ; mov edx,[eax] ; push esi ; mov ecx,eax ; call [edx+10h] ; done: lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// The one-argument form of the observer notification of `2026-10-14_linux_i1116`. The manifest
// label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CInventory_00c114b0;
struct CObserver_00c114b0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Notify(CInventory_00c114b0* source);   // slot 4
};

struct CObservable_00c114b0 {
    long m_a;
    CObserver_00c114b0* Find(void* key);   // 0x00C21E20
};

struct CInventory_00c114b0 {
    unsigned char _pad[0x2c];
    CObservable_00c114b0 m_observable;     // +0x2c
    void NotifyObserver(void* key);
};

void CInventory_00c114b0::NotifyObserver(void* key)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CObserver_00c114b0* observer = m_observable.Find(key);
    if (observer)
        observer->Notify(this);
}