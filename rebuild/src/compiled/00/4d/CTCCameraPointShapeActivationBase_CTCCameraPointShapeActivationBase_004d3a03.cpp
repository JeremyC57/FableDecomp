#pragma optimize("s",on)
// CTCCameraPointShapeActivationBase::CTCCameraPointShapeActivationBase(CThing*) -- base CTCOpinionSource(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3A03, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCOpinionSource::CTCOpinionSource ; mov dword ptr [esi],offset CTCCameraPointShapeActivationBase::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x0075C180; the compiler then stores this class's vtable (0x0123AEDC, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
struct CThing;

namespace N_0075c180 { struct CTCOpinionSource {
    virtual void v00();
    CTCOpinionSource(CThing* argument);            // 0x0075C180
}; } using N_0075c180::CTCOpinionSource;

struct CTCCameraPointShapeActivationBase : CTCOpinionSource {
    virtual void v00();
    CTCCameraPointShapeActivationBase(CThing* argument);
};

CTCCameraPointShapeActivationBase::CTCCameraPointShapeActivationBase(CThing* argument)
    : CTCOpinionSource(argument)
{
}