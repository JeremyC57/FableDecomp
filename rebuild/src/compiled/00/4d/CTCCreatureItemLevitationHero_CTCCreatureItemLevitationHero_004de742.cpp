#pragma optimize("s",on)
// CTCCreatureItemLevitationHero::CTCCreatureItemLevitationHero(CThing&) -- base CTCCreatureItemLevitationBase(argument) then this class's vtable
// Leaf: ??0CTCCreatureItemLevitationHero@@QAE@AAVCThing@@@Z
// Respelled 2026-11-16: CTCCreatureItemLevitationBase(CThing&) (0x004DE6B6) as both PDBs spell it; this constructor too.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE742, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCCreatureItemLevitationBase::CTCCreatureItemLevitationBase ; mov dword ptr [esi],offset CTCCreatureItemLevitationHero::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x004DE6B6; the compiler then stores this class's vtable (0x0124167C, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
class CThing;

struct CTCCreatureItemLevitationBase {
    virtual void v00();
    CTCCreatureItemLevitationBase(CThing& argument);            // 0x004DE6B6
};

struct CTCCreatureItemLevitationHero : CTCCreatureItemLevitationBase {
    virtual void v00();
    CTCCreatureItemLevitationHero(CThing& argument);
};

CTCCreatureItemLevitationHero::CTCCreatureItemLevitationHero(CThing& argument)
    : CTCCreatureItemLevitationBase(argument)
{
}