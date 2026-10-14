// CFactory_00c0e7e0::Create(void*, void*, void*, void*) -- EH-framed `new CObject(this, b, c, d, a)` (0x2c bytes through the global operator new @ 0x00BFEA1A, constructor @ 0x00C1E110); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E7E0, oracle length 117 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push 2Ch ; mov esi,ecx ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; mov ecx,[a] ; mov edx,[d] ; push ecx ; mov ecx,[c] ; push edx ; mov edx,[b] ; push ecx ; push edx ; push esi ; mov ecx,eax ; call CObject::CObject ; pop esi ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret 10h ; null: mov ecx,[esp+8] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 10h
// The four-argument sibling of the member factories at 0x00C0E700 / 0x00C0E770: the owner and
// the arguments (second, third, fourth, first) go to the UI component constructor landed at
// 0x00C1E110. Two epilogues because the null path skips the constructor. The manifest label is
// propagated; the class is named by address.
struct CFactory_00c0e7e0;

struct CObject_00c0e7e0 {                  // 0x2c bytes
    unsigned char _body[0x2c];
    CObject_00c0e7e0(CFactory_00c0e7e0* owner, void* b, void* c, void* d, void* a);   // 0x00C1E110
};

struct CFactory_00c0e7e0 {
    CObject_00c0e7e0* Create(void* a, void* b, void* c, void* d);
};

CObject_00c0e7e0* CFactory_00c0e7e0::Create(void* a, void* b, void* c, void* d)
{
    return new CObject_00c0e7e0(this, b, c, d, a);
}