// CTCPlot::CTCPlot(CThing*) -- base CTCDiceGame(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB433, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCDiceGame::CTCDiceGame ; mov dword ptr [esi],offset CTCPlot::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x004DB3DB; the compiler then stores this class's vtable (0x012409B4, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
struct CThing;

struct CTCDiceGame {
    virtual void v00();
    CTCDiceGame(CThing* argument);            // 0x004DB3DB
};

struct CTCPlot : CTCDiceGame {
    virtual void v00();
    CTCPlot(CThing* argument);
};

CTCPlot::CTCPlot(CThing* argument)
    : CTCDiceGame(argument)
{
}
