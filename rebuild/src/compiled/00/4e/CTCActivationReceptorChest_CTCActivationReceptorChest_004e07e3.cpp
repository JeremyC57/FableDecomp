#pragma optimize("s",on)
// CTCActivationReceptorChest::CTCActivationReceptorChest(CThing*) -- base CTCActivationReceptorBase(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E07E3, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
// Naming (NF455/NF456, r454b, ruling r455c): the constructor takes `CThing&`, ??0CTCActivationReceptorChest@@QAE@AAVCThing@@@Z as FableWin.pdb spells it (Ego_r inlines the
//   ctor; one-source:fablewin-pdb best guess, BEST_GUESS.tsv), was ??0CTCActivationReceptorChest@@QAE@PAVCThing@@@Z; its sole retail caller, the factory 0x004E2791
//   ?Alloc@CTCActivationReceptorChest@@SIPAVCTCBase@@AAVCThing@@@Z (both PDBs), passes its CThing& straight through. A reference passes the same pointer: object unchanged.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCActivationReceptorBase::CTCActivationReceptorBase ; mov dword ptr [esi],offset CTCActivationReceptorChest::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x007F7710; the compiler then stores this class's vtable (0x01242084, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
class CThing;

class CTCActivationReceptorBase { public:
    virtual void v00();
    CTCActivationReceptorBase(CThing& argument);            // 0x007F7710
};

struct CTCActivationReceptorChest : CTCActivationReceptorBase {
    virtual void v00();
    CTCActivationReceptorChest(CThing& argument);
};

CTCActivationReceptorChest::CTCActivationReceptorChest(CThing& argument)
    : CTCActivationReceptorBase(argument)
{
}