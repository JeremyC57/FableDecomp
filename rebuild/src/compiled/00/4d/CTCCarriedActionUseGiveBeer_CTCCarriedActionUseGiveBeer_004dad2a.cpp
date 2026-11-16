#pragma optimize("s",on)
// CTCCarriedActionUseGiveBeer::CTCCarriedActionUseGiveBeer(CThing&) -- base CTCCarriedActionUseGive(argument) then this class's vtable
// Leaf: ??0CTCCarriedActionUseGiveBeer@@QAE@AAVCThing@@@Z
// Respelled 2026-11-16: the base is CTCCarriedActionUseGive(CThing&) (0x004DACFD; the local stand-in carried the propagated label
//   CEnginePrimitiveGroupManager and took void*), and this constructor takes CThing& as both PDBs spell it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DAD2A, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCCarriedActionUseGive::CTCCarriedActionUseGive ; mov dword ptr [esi],offset CTCCarriedActionUseGiveBeer::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x004DACFD; the compiler then stores this class's vtable (0x0123FB3C, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.

class CThing;
class CTCCarriedActionUseGive { public:
    virtual void v00();
    CTCCarriedActionUseGive(CThing& argument);            // 0x004DACFD
};

struct CTCCarriedActionUseGiveBeer : CTCCarriedActionUseGive {
    virtual void v00();
    CTCCarriedActionUseGiveBeer(CThing& argument);
};

CTCCarriedActionUseGiveBeer::CTCCarriedActionUseGiveBeer(CThing& argument)
    : CTCCarriedActionUseGive(argument)
{
}