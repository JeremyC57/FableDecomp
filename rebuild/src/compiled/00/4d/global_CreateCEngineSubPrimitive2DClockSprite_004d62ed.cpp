#pragma optimize("s",on)
// Create_CEngineSubPrimitive2DClockSprite(CThing*) -- factory: new CEngineSubPrimitive2DClockSprite(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D62ED, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xd8 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CEngineSubPrimitive2DClockSprite) = 0xd8 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x005E4030 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CEngineSubPrimitive2DClockSprite {
    unsigned char _body[0xd8];       // sizeof(CEngineSubPrimitive2DClockSprite) = 0xd8
    CEngineSubPrimitive2DClockSprite(CThing* owner);                   // 0x005E4030
};

CEngineSubPrimitive2DClockSprite* __fastcall Create_CEngineSubPrimitive2DClockSprite(CThing* owner)
{
    return new CEngineSubPrimitive2DClockSprite(owner);
}