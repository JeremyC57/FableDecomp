// Create_CAIStateGroupBase_00c3f970() -- `new CAIStateGroupBase_00c3f970` (0x250 bytes through the class operator new @ 0x00C3C360, constructor @ 0x00C3F760); EH-framed so a throwing constructor frees the block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3F970, oracle length 89 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push 0x250 ; call operator new ; add esp,4 ; mov [esp],eax ; test eax,eax ; mov dword ptr [esp+0Ch],0 ; je null ; mov ecx,eax ; call CAIStateGroupBase_00c3f970::CAIStateGroupBase_00c3f970 ; mov ecx,[esp+4] ; mov fs:[0],ecx ; add esp,10h ; ret ; null: mov ecx,[esp+4] ; xor eax,eax ; mov fs:[0],ecx ; add esp,10h ; ret
// A factory: `new CAIStateGroupBase_00c3f970` with the class's own `operator new` (@ 0x00C3C360, cdecl, caller
// cleans the size), the block null-checked, the out-of-line constructor @ 0x00C3F760 run on it
// and returned (null when the allocation failed). The block is kept in the frame because the
// constructor may throw: EH state 0 covers it and the unwind funclet passes it to the class's
// `operator delete`. Built under /EHsc (the funclet lands through the funclet pass). The
// constructor's manifest label names the class when it is unique; sizeof is modelled by an opaque
// body. One of 3 bodies of this shape.
struct CAIStateGroupBase_00c3f970 {
    unsigned char _body[0x250];
    static void* operator new(unsigned int size);   // 0x00C3C360
    static void operator delete(void* block);
    CAIStateGroupBase_00c3f970();                               // 0x00C3F760
};

CAIStateGroupBase_00c3f970* Create_CAIStateGroupBase_00c3f970()
{
    return new CAIStateGroupBase_00c3f970;
}