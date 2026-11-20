
// Naming (NF198, 2026-11-20, q11993): the CTCTavernGameCard* constructors take the PDB CThing& (Ego_r.pdb + FableWin.pdb
//   ??0CTCTavernGameCardBase@@QAE@AAVCThing@@@Z 0x008F1CD0, ??0CTCTavernGameCardSorting@@QAE@AAVCThing@@@Z 0x007D5AD0,
//   ??0CTCTavernGameCardPairs@@QAE@AAVCThing@@@Z 0x007D7520, ??0CTCTavernGameCardBlackjack@@QAE@AAVCThing@@@Z 0x007DB280); respelled in place
//   on the row's local models as one family with the three Alloc factories 0x004D5A11 / 0x004D5A44 / 0x004D5AAA (were CThing*; same bytes).
#pragma optimize("s",on)
// CTCTavernGameCardPairs::Alloc(CThing&) -- factory: new CTCTavernGameCardPairs(owner)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5A44, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x330 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CTCTavernGameCardPairs::CTCTavernGameCardPairs ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CTCTavernGameCardPairs) = 0x330 bytes (operator new @ 0x00BFEA1A,
// landed-trusted), null-checks the block and runs the constructor @ 0x007D7520 on it with the
// argument, returning the new object (or null). One of 338 bodies of this shape; the class
// is taken from the constructor's manifest label. sizeof is modelled by an opaque body.
class CThing;

struct CTCTavernGameCardPairs {
    unsigned char _body[0x330];       // sizeof(CTCTavernGameCardPairs) = 0x330
    CTCTavernGameCardPairs(CThing& owner);                   // 0x007D7520
};

CTCTavernGameCardPairs* __fastcall CTCTavernGameCardPairs_Alloc(CThing* owner)
{
    return new CTCTavernGameCardPairs(*owner);
}