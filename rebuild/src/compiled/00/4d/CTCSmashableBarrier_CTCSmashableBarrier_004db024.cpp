#pragma optimize("s",on)
// CTCSmashableBarrier::CTCSmashableBarrier(CThing*) -- base CTCCameraCollision(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB024, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCCameraCollision::CTCCameraCollision ; mov dword ptr [esi],offset CTCSmashableBarrier::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x007EEBE0; the compiler then stores this class's vtable (0x0124017C, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
class CThing;

class CTCCameraCollision { public:
    virtual void v00();
    CTCCameraCollision(CThing* argument);            // 0x007EEBE0
};

class CTCSmashableBarrier : public CTCCameraCollision { public:
    virtual void v00();
    CTCSmashableBarrier(CThing* argument);
};

CTCSmashableBarrier::CTCSmashableBarrier(CThing* argument)
    : CTCCameraCollision(argument)
{
}