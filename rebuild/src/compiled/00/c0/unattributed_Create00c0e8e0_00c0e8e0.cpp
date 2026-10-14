// CFactory_00c0e8e0::Create(void*) -- EH-framed `new CObject(this, argument)` (0x20 bytes through the global operator new @ 0x00BFEA1A, constructor @ 0x00C1E6D0); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E8E0, oracle length 102 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push 20h ; mov esi,ecx ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; mov ecx,[argument] ; push ecx ; push esi ; mov ecx,eax ; call CObject::CObject ; pop esi ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret 4 ; null: mov ecx,[esp+8] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// A member factory passing the owner and the argument; two epilogues (unlike 0x00C37990, which
// merges them). EH state 0 covers the constructor (the funclet frees the block). The manifest
// label (`Alloc`) is propagated; the classes are named by address.
struct CFactory_00c0e8e0;

struct CObject_00c0e8e0 {
    unsigned char _body[0x20];
    CObject_00c0e8e0(CFactory_00c0e8e0* owner, void* argument);   // 0x00C1E6D0
};

struct CFactory_00c0e8e0 {
    CObject_00c0e8e0* Create(void* argument);
};

CObject_00c0e8e0* CFactory_00c0e8e0::Create(void* argument)
{
    return new CObject_00c0e8e0(this, argument);
}