#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionSpreadShotProjectile::CTCShotDefinitionSpreadShotProjectile(CThing*) -- base CTCShotDefinitionProjectile(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCA3A, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCShotDefinitionProjectile::CTCShotDefinitionProjectile ; mov dword ptr [esi],offset CTCShotDefinitionSpreadShotProjectile::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x004DC991; the compiler then stores this class's vtable (0x01240D84, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
struct CThing;

struct CTCShotDefinitionProjectile {
    virtual void v00();
    CTCShotDefinitionProjectile(CThing* argument);            // 0x004DC991
};

struct CTCShotDefinitionSpreadShotProjectile : CTCShotDefinitionProjectile {
    virtual void v00();
    CTCShotDefinitionSpreadShotProjectile(CThing* argument);
};

CTCShotDefinitionSpreadShotProjectile::CTCShotDefinitionSpreadShotProjectile(CThing* argument)
    : CTCShotDefinitionProjectile(argument)
{
}