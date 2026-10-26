#pragma optimize("s",on)
// CTCActivationTrigger::Alloc(CThing&) -- factory: new CTCActivationTrigger(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8638, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x20 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCActivationTrigger::CTCActivationTrigger ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCActivationTrigger) = 0x20 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004D43B9 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

class CTCActivationTrigger { public:
    unsigned char _body[0x20];       // sizeof(CTCActivationTrigger) = 0x20
    CTCActivationTrigger(CThing* owner);                   // 0x004D43B9
};

CTCActivationTrigger* __fastcall CTCActivationTrigger_Alloc(CThing* owner)
{
    return new CTCActivationTrigger(owner);
}