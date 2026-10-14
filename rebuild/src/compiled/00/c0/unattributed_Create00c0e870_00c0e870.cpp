// CFactory_00c0e870::Create(void*, void*) -- EH-framed `new CObject(a, b)` (0x1c bytes through the global operator new @ 0x00BFEA1A, constructor @ 0x00C1E5A0); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E870, oracle length 100 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push 1Ch ; call operator new ; add esp,4 ; mov [esp],eax ; test eax,eax ; mov dword ptr [esp+0Ch],0 ; je null ; mov ecx,[b] ; mov edx,[a] ; push ecx ; push edx ; mov ecx,eax ; call CObject::CObject ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret 8 ; null: mov ecx,[esp+4] ; xor eax,eax ; mov fs:[0],ecx ; add esp,10h ; ret 8
// A factory method whose `this` is not read; the block is kept in the frame because the
// constructor may throw (EH state 0; the funclet frees it through operator delete); the null path
// has its own epilogue. The manifest label (`Alloc`) is propagated; the classes are named by address.
struct CObject_00c0e870 {
    unsigned char _body[0x1c];
    CObject_00c0e870(void* a, void* b);    // 0x00C1E5A0
};

struct CFactory_00c0e870 {
    CObject_00c0e870* Create(void* a, void* b);
};

CObject_00c0e870* CFactory_00c0e870::Create(void* a, void* b)
{
    return new CObject_00c0e870(a, b);
}