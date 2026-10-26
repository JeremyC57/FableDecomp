#pragma optimize("s",on)
// CTCWillResponse::Alloc(CThing&) -- factory: new CTCWillResponse(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D645D, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x14 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCWillResponse::CTCWillResponse ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCWillResponse) = 0x14 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007E3C70 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCWillResponse { public:
    unsigned char _body[0x14];       // sizeof(CTCWillResponse) = 0x14
    CTCWillResponse(CThing* owner);                   // 0x007E3C70
};

CTCWillResponse* __fastcall CTCWillResponse_Alloc(CThing* owner)
{
    return new CTCWillResponse(owner);
}