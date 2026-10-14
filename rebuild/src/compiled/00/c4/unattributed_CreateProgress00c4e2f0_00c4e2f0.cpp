// CBankFile_00c4e2f0::CreateProgress() -- EH-framed: `new CProgress(1)` (0x4f0 bytes through the class operator new @ 0x00C3C360, constructor @ 0x00C604E0) stored into the member at +0xe4; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4E2F0, oracle length 91 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push 4F0h ; mov esi,ecx ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; push 1 ; mov ecx,eax ; call CProgress::CProgress ; jmp store ; null: xor eax,eax ; store: mov ecx,[esp+8] ; mov [esi+0E4h],eax ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// EH state 0 covers the constructor call (the funclet passes the block to the class operator
// delete). The manifest label (`Alloc`) is propagated; the classes are named by address (the
// member at +0xe4 is the one deleted by the destructor at 0x00C4F220).
struct CProgress_00c4e2f0 {
    unsigned char _body[0x4f0];
    static void* operator new(unsigned int size);   // 0x00C3C360
    static void operator delete(void* block);
    CProgress_00c4e2f0(long kind);         // 0x00C604E0
};

struct CBankFile_00c4e2f0 {
    unsigned char _pad[0xe4];
    CProgress_00c4e2f0* m_pProgress;       // +0xe4
    long CreateProgress();
};

long CBankFile_00c4e2f0::CreateProgress()
{
    m_pProgress = new CProgress_00c4e2f0(1);
    return 0;
}