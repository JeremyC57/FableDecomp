// CEventHitBy::CEventHitBy(CHitParameters*) -- base CEventHitBase(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083AF30, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CEventHitBase::CEventHitBase ; mov dword ptr [esi],offset CEventHitBy::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x00811760; the compiler then stores this class's vtable (0x01272FA8, named
// through rtti-vtables.tsv) and returns `this`. One of 12 bodies of this shape.
struct CHitParameters;

struct CEventHitBase {
    virtual void v00();
    CEventHitBase(CHitParameters* argument);            // 0x00811760
};

struct CEventHitBy : CEventHitBase {
    virtual void v00();
    CEventHitBy(CHitParameters* argument);
};

CEventHitBy::CEventHitBy(CHitParameters* argument)
    : CEventHitBase(argument)
{
}