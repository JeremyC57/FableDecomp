// CFactory_00c0e700::Create(void*, void*) -- EH-framed `new CObject(this, b, a)` (0x24 bytes through the global operator new @ 0x00BFEA1A, constructor @ 0x00C1D880); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E700, oracle length 107 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push 24h ; mov esi,ecx ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; mov ecx,[a] ; mov edx,[b] ; push ecx ; push edx ; push esi ; mov ecx,eax ; call CObject::CObject ; pop esi ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret 8 ; null: mov ecx,[esp+8] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 8
// A member factory passing the owner and the two arguments (the second before the first); two
// epilogues. EH state 0 covers the constructor. The manifest label (`Alloc`) is propagated; the
// classes are named by address.
struct CFactory_00c0e700;

struct CObject_00c0e700 {
    unsigned char _body[0x24];
    CObject_00c0e700(CFactory_00c0e700* owner, void* b, void* a);   // 0x00C1D880
};

struct CFactory_00c0e700 {
    CObject_00c0e700* Create(void* a, void* b);
};

CObject_00c0e700* CFactory_00c0e700::Create(void* a, void* b)
{
    return new CObject_00c0e700(this, b, a);
}