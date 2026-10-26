#pragma optimize("s",on)
// CTCDNavigationSeed::CTCDNavigationSeed(CThing*) -- base CTCDriverBase(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D41F6, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCDriverBase::CTCDriverBase ; mov dword ptr [esi],offset CTCDNavigationSeed::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x00771200; the compiler then stores this class's vtable (0x0123B944, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
class CThing;

class CTCDriverBase { public:
    virtual void v00();
    CTCDriverBase(CThing* argument);            // 0x00771200
};

class CTCDNavigationSeed : public CTCDriverBase { public:
    virtual void v00();
    CTCDNavigationSeed(CThing* argument);
};

CTCDNavigationSeed::CTCDNavigationSeed(CThing* argument)
    : CTCDriverBase(argument)
{
}