#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionThrow::CTCShotDefinitionThrow(CThing*) -- base CTCShotDefinitionBase(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D545A, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCShotDefinitionBase::CTCShotDefinitionBase ; mov dword ptr [esi],offset CTCShotDefinitionThrow::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x007BF460; the compiler then stores this class's vtable (0x0123CDC4, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
class CThing;

struct CTCShotDefinitionBase {
    virtual void v00();
    CTCShotDefinitionBase(CThing* argument);            // 0x007BF460
};

struct CTCShotDefinitionThrow : CTCShotDefinitionBase {
    virtual void v00();
    CTCShotDefinitionThrow(CThing* argument);
};

CTCShotDefinitionThrow::CTCShotDefinitionThrow(CThing* argument)
    : CTCShotDefinitionBase(argument)
{
}