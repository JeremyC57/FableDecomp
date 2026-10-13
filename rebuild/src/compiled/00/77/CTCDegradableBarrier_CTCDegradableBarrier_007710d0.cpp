// CTCDegradableBarrier::CTCDegradableBarrier(CThing*) -- base CTCCameraCollision(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007710D0, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCCameraCollision::CTCCameraCollision ; mov dword ptr [esi],offset CTCDegradableBarrier::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x008EC850; the compiler then stores this class's vtable (0x0126A6A4, named
// through rtti-vtables.tsv) and returns `this`. One of 12 bodies of this shape.
struct CThing;

struct CTCCameraCollision {
    virtual void v00();
    CTCCameraCollision(CThing* argument);            // 0x008EC850
};

struct CTCDegradableBarrier : CTCCameraCollision {
    virtual void v00();
    CTCDegradableBarrier(CThing* argument);
};

CTCDegradableBarrier::CTCDegradableBarrier(CThing* argument)
    : CTCCameraCollision(argument)
{
}