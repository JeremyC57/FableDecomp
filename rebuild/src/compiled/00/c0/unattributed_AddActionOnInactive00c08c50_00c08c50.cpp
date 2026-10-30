// CList_00c08c50::AddActionOnInactive(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, when the slot at +0x60 is empty a 0x18-byte action is allocated (global operator new @ 0x00BFEA1A, constructor @ 0x00C16050 from `this`, the argument and the id 0xAC44, EH state 1 while unowned) and stored there
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C08C50, oracle length 139 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,0Ch ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[esi+60h] ; test eax,eax ; mov dword ptr [esp+18h],0 ; jne done ; push 18h ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov byte ptr [esp+18h],1 ; je null ; mov ecx,[arg] ; push 0AC44h ; push ecx ; push esi ; mov ecx,eax ; call CAction::CAction ; jmp store ; null: xor eax,eax ; store: mov [esi+60h],eax ; done: lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~PKCProtectionInstance ; ...
// The manifest label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CList_00c08c50;
struct CAction_00c08c50 {                  // 0x18 bytes
    unsigned char _body[0x18];
    CAction_00c08c50(CList_00c08c50* list, void* target, long id);   // 0x00C16050
};

struct CList_00c08c50 {
    unsigned char _pad[0x60];
    CAction_00c08c50* m_pInactiveAction;   // +0x60
    void AddActionOnInactive(void* target);
};

void CList_00c08c50::AddActionOnInactive(void* target)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (!m_pInactiveAction)
        m_pInactiveAction = new CAction_00c08c50(this, target, 0xAC44);
}