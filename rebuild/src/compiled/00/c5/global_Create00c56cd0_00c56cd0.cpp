// Create_00c56cd0() -- EH-framed `new CDerived` (0x88 bytes through the class operator new @ 0x00C3C360): the out-of-line base constructor @ 0x00C72600 then the derived vtable (0x012B8D48) stored inline; the block stays in the frame for the operator-delete funclet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C56CD0, oracle length 83 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push 88h ; call operator new ; mov esi,eax ; add esp,4 ; mov [esp+4],esi ; xor eax,eax ; cmp esi,eax ; mov [esp+10h],eax ; je null ; mov ecx,esi ; call CBase::CBase ; mov dword ptr [esi],offset vtable ; null: mov eax,esi ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// A factory whose derived class has an inline empty constructor over an out-of-line base
// constructor (the base's own vtable store is dead and dropped, the derived vtable is stored after
// the call). The class's own operator new/delete are used (EH state 0 covers the constructor; the
// funclet frees the block through operator delete). The manifest label (`Alloc`) is propagated;
// the classes are named by address.
struct CBase_00c56cd0 {
    virtual void v00();
    unsigned char _body[0x84];
    CBase_00c56cd0();                           // 0x00C72600
};

struct CDerived_00c56cd0 : CBase_00c56cd0 {         // vtable 0x012B8D48
    static void* operator new(unsigned int size);   // 0x00C3C360
    static void operator delete(void* block);
    CDerived_00c56cd0() { }
};

CDerived_00c56cd0* Create_00c56cd0()
{
    return new CDerived_00c56cd0;
}