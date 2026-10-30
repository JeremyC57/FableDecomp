// CInventory_00c11330::ProcessSelection(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the observable member at +0x2c looks the key up (@ 0x00C21E20); when found the observer is told through virtual slot 1 with `this` and the inventory refreshes itself out of line (@ 0x00C10CD0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C11330, oracle length 116 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[key] ; push eax ; lea ecx,[esi+2Ch] ; mov dword ptr [esp+18h],0 ; call CObservable::Find ; test eax,eax ; je done ; mov edx,[eax] ; push esi ; mov ecx,eax ; call [edx+4] ; mov ecx,esi ; call Refresh ; done: lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// The slot-1 sibling of the slot-4 notification at 0x00C114B0 with a refresh after the call.
// The manifest label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CInventory_00c11330;
struct CObserver_00c11330 {
    virtual void v00();
    virtual void Select(CInventory_00c11330* source);   // slot 1
};

struct CObservable_00c11330 {
    long m_a;
    CObserver_00c11330* Find(void* key);   // 0x00C21E20
};

struct CInventory_00c11330 {
    unsigned char _pad[0x2c];
    CObservable_00c11330 m_observable;     // +0x2c
    void Refresh();                        // 0x00C10CD0
    void ProcessSelection(void* key);
};

void CInventory_00c11330::ProcessSelection(void* key)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CObserver_00c11330* observer = m_observable.Find(key);
    if (observer) {
        observer->Select(this);
        Refresh();
    }
}