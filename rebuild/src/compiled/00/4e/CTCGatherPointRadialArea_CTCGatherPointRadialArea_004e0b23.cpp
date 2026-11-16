#pragma optimize("s",on)
// CTCGatherPointRadialArea::CTCGatherPointRadialArea(CThing&) -- base CTCGatherPointBase(argument) then this class's vtable
// Leaf: ??0CTCGatherPointRadialArea@@QAE@AAVCThing@@@Z
// Respelled 2026-11-16: CTCGatherPointBase(CThing&) (0x004E0AAB) as both PDBs spell it; this constructor too.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0B23, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCGatherPointBase::CTCGatherPointBase ; mov dword ptr [esi],offset CTCGatherPointRadialArea::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x004E0AAB; the compiler then stores this class's vtable (0x012422E4, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
class CThing;

struct CTCGatherPointBase {
    virtual void v00();
    CTCGatherPointBase(CThing& argument);            // 0x004E0AAB
};

struct CTCGatherPointRadialArea : CTCGatherPointBase {
    virtual void v00();
    CTCGatherPointRadialArea(CThing& argument);
};

CTCGatherPointRadialArea::CTCGatherPointRadialArea(CThing& argument)
    : CTCGatherPointBase(argument)
{
}