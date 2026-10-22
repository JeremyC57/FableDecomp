#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionForcePushBall::Alloc(CThing&) -- factory: new CTCShotDefinitionForcePushBall(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DEB47, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xa8 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCShotDefinitionForcePushBall::CTCShotDefinitionForcePushBall ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCShotDefinitionForcePushBall) = 0xa8 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DCAB9 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CPlayerGuiDef;

struct CTCShotDefinitionForcePushBall {
    unsigned char _body[0xa8];       // sizeof(CTCShotDefinitionForcePushBall) = 0xa8
    CTCShotDefinitionForcePushBall(CPlayerGuiDef* owner);                   // 0x004DCAB9
};

CTCShotDefinitionForcePushBall* __fastcall CTCShotDefinitionForcePushBall_Alloc(CPlayerGuiDef* owner)
{
    return new CTCShotDefinitionForcePushBall(owner);
}