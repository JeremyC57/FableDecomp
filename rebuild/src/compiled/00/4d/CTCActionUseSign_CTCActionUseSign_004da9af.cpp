#pragma optimize("s",on)
// CTCActionUseSign::CTCActionUseSign(CThing*) -- base CTCActionUseBase(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA9AF, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCActionUseBase::CTCActionUseBase ; mov dword ptr [esi],offset CTCActionUseSign::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x004DA939; the compiler then stores this class's vtable (0x0123F1AC, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
class CThing;

class CTCActionUseBase { public:
    virtual void v00();
    CTCActionUseBase(CThing* argument);            // 0x004DA939
};

struct CTCActionUseSign : CTCActionUseBase {
    virtual void v00();
    CTCActionUseSign(CThing* argument);
};

CTCActionUseSign::CTCActionUseSign(CThing* argument)
    : CTCActionUseBase(argument)
{
}