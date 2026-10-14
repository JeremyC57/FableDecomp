// CFactory_00c0e770::Create(void*, void*, void*) -- EH-framed `new CObject(this, b, c, a)` (0x28 bytes through the global operator new @ 0x00BFEA1A, constructor @ 0x00C1E050); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E770, oracle length 112 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push 28h ; mov esi,ecx ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; mov ecx,[a] ; mov edx,[c] ; push ecx ; mov ecx,[b] ; push edx ; push ecx ; push esi ; mov ecx,eax ; call CObject::CObject ; pop esi ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret 0Ch ; null: mov ecx,[esp+8] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 0Ch
// A member factory passing the owner and the three arguments (second, third, first); two
// epilogues. The manifest label (`Alloc`) is propagated; the classes are named by address.
struct CFactory_00c0e770;

struct CObject_00c0e770 {
    unsigned char _body[0x28];
    CObject_00c0e770(CFactory_00c0e770* owner, void* b, void* c, void* a);   // 0x00C1E050
};

struct CFactory_00c0e770 {
    CObject_00c0e770* Create(void* a, void* b, void* c);
};

CObject_00c0e770* CFactory_00c0e770::Create(void* a, void* b, void* c)
{
    return new CObject_00c0e770(this, b, c, a);
}