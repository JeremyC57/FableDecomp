// CreateHeroStatsListener_00c74360() -- EH-framed: allocates the global listener (@ 0x0143CCDC) through the listeners' class operator new (@ 0x00C763C0, 4 bytes): the out-of-line kind (constructor @ 0x00C88A30, EH state 0 while the allocation is unowned) when the global flag byte @ 0x0143BC7C is set, else the inline kind (vtable 0x012BF02C); false when the allocation failed, else the reference count @ 0x0143CCE0 is set to 1 and true returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74360, oracle length 138 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; mov al,[g_flag] ; test al,al ; push 4 ; je inline ; call operator new ; add esp,4 ; mov [esp],eax ; test eax,eax ; mov dword ptr [esp+0Ch],0 ; je null ; mov ecx,eax ; call CListenerA::CListenerA ; jmp store ; inline: call operator new ; add esp,4 ; test eax,eax ; je null ; mov dword ptr [eax],offset vtableB ; jmp store ; null: xor eax,eax ; store: test eax,eax ; mov [g_pListener],eax ; jne ok ; xor al,al ; ... ret ; ok: mov eax,1 ; mov [g_count],eax ; ... ret
// The `push 4` is shared by both allocations; only the out-of-line constructor needs an EH state
// (its unwind frees the block through the class operator delete @ 0x00C763D0). The manifest label
// is a description; named by the shape.
extern bool g_useOutOfLineListener_0143bc7c;      // 0x0143BC7C

struct CListenerBase_00c74360 {
    virtual void v00();
    static void* operator new(unsigned int size);   // 0x00C763C0
    static void operator delete(void* block);       // 0x00C763D0
};

struct CListenerA_00c74360 : CListenerBase_00c74360 {
    CListenerA_00c74360();                 // 0x00C88A30
};

struct CListenerB_00c74360 : CListenerBase_00c74360 {   // vtable 0x012BF02C
    CListenerB_00c74360() { }
};

extern CListenerBase_00c74360* g_pListener_0143ccdc;   // 0x0143CCDC
extern long g_listenerCount_0143cce0;                   // 0x0143CCE0

bool CreateHeroStatsListener_00c74360()
{
    CListenerBase_00c74360* listener;
    if (g_useOutOfLineListener_0143bc7c)
        listener = new CListenerA_00c74360;
    else
        listener = new CListenerB_00c74360;
    g_pListener_0143ccdc = listener;
    if (!listener)
        return false;
    g_listenerCount_0143cce0 = 1;
    return true;
}
