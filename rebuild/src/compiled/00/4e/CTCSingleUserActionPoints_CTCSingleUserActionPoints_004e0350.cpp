#pragma optimize("s",on)
// CTCSingleUserActionPoints::CTCSingleUserActionPoints(CThing*) -- base CTCUserActionPointBase(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0350, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCUserActionPointBase::CTCUserActionPointBase ; mov dword ptr [esi],offset CTCSingleUserActionPoints::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x007FEC60; the compiler then stores this class's vtable (0x01241EAC, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
class CThing;

struct CTCUserActionPointBase {
    virtual void v00();
    CTCUserActionPointBase(CThing* argument);            // 0x007FEC60
};

class CTCSingleUserActionPoints : public CTCUserActionPointBase { public:
    virtual void v00();
    CTCSingleUserActionPoints(CThing* argument);
};

CTCSingleUserActionPoints::CTCSingleUserActionPoints(CThing* argument)
    : CTCUserActionPointBase(argument)
{
}