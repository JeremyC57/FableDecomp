// CGame::CGame() -- base CInitBaseClass() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00413200, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CInitBaseClass::CInitBaseClass ; mov dword ptr [esi],offset CGame::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x0099A310 runs, then the compiler stores this class's vtable (0x0122E59C
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
struct CInitBaseClass {
    virtual void v00();
    CInitBaseClass();                           // 0x0099A310
};

struct CGame : CInitBaseClass {
    virtual void v00();                    // this class has its own vtable
    CGame();
};

CGame::CGame()
    : CInitBaseClass()
{
}