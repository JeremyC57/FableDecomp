// CGuiControlTree::SelectNextEntryByName(void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the tree member at +0xc selects out of line (@ 0x00C0B570) with (name, 0, context)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C38600, oracle length 104 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[context] ; mov ecx,[name] ; push eax ; push 0 ; push ecx ; lea ecx,[esi+0Ch] ; mov dword ptr [esp+20h],0 ; call CTree::Select ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 8
// The scoped-lock shape with three forwarded arguments on an embedded member. The class comes
// from the manifest label; the tree member is descriptive.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CGuiTree_00c38600 {
    void Select(void* name, void* previous, void* context);   // 0x00C0B570
};

class CGuiControlTree { public:
    unsigned char _pad[0xc];
    CGuiTree_00c38600 m_tree;              // +0xc
    void SelectNextEntryByName(void* name, void* context);
};

void CGuiControlTree::SelectNextEntryByName(void* name, void* context)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    m_tree.Select(name, 0, context);
}