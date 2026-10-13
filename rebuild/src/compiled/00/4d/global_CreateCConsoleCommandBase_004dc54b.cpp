#pragma optimize("s",on)
// Create_CConsoleCommandBase(CCharString*) -- factory: new CConsoleCommandBase(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC54B, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x28 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CConsoleCommandBase::CConsoleCommandBase ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CConsoleCommandBase) = 0x28 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x004DAE4D on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
struct CCharString;

struct CConsoleCommandBase {
    unsigned char _body[0x28];       // sizeof(CConsoleCommandBase) = 0x28
    CConsoleCommandBase(CCharString* owner);                   // 0x004DAE4D
};

CConsoleCommandBase* __fastcall Create_CConsoleCommandBase(CCharString* owner)
{
    return new CConsoleCommandBase(owner);
}