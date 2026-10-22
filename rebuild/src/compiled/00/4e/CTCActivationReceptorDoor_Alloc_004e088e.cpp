#pragma optimize("s",on)
// CTCActivationReceptorDoor::Alloc(CThing&) -- factory: new CTCActivationReceptorDoor(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E088E, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x2c ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActivationReceptorDoor::CTCActivationReceptorDoor ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCActivationReceptorDoor) = 0x2c bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DE660 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.

struct CTCActivationReceptorDoor {
    unsigned char _body[0x2c];       // sizeof(CTCActivationReceptorDoor) = 0x2c
    CTCActivationReceptorDoor(void* owner);                   // 0x004DE660
};

CTCActivationReceptorDoor* __fastcall CTCActivationReceptorDoor_Alloc(void* owner)
{
    return new CTCActivationReceptorDoor(owner);
}