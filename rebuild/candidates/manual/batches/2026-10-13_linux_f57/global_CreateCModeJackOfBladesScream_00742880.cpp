// Create_CModeJackOfBladesScream(CThing*) -- factory: new CModeJackOfBladesScream(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00742880, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xbc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CModeJackOfBladesScream::CModeJackOfBladesScream ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CModeJackOfBladesScream) = 0xbc bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x00740450 on it with the
// argument, returning the new object (or null). One of 18 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CThing;

struct CModeJackOfBladesScream {
    unsigned char _body[0xbc];       // sizeof(CModeJackOfBladesScream) = 0xbc
    CModeJackOfBladesScream(CThing* owner);                   // 0x00740450
};

CModeJackOfBladesScream* __fastcall Create_CModeJackOfBladesScream(CThing* owner)
{
    return new CModeJackOfBladesScream(owner);
}
