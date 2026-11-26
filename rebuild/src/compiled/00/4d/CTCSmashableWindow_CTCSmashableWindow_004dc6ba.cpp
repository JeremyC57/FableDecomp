#pragma optimize("s",on)
// CTCSmashableWindow::CTCSmashableWindow(CThing*) -- base CTCCameraCollision(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC6BA, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
// Naming (NF455/NF456, r454b, ruling r455c): the constructor takes `CThing&`, ??0CTCSmashableWindow@@QAE@AAVCThing@@@Z as FableWin.pdb spells it (Ego_r inlines the
//   ctor; one-source:fablewin-pdb best guess, BEST_GUESS.tsv), was ??0CTCSmashableWindow@@QAE@PAVCThing@@@Z; its sole retail caller, the factory 0x004DE34E
//   ?Alloc@CTCSmashableWindow@@SIPAVCTCBase@@AAVCThing@@@Z (both PDBs), passes its CThing& straight through. A reference passes the same pointer: object unchanged.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CTCCameraCollision::CTCCameraCollision ; mov dword ptr [esi],offset CTCSmashableWindow::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x007EEBE0; the compiler then stores this class's vtable (0x01240BA4, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
class CThing;

class CTCCameraCollision { public:
    virtual void v00();
    CTCCameraCollision(CThing* argument);            // 0x007EEBE0
};

class CTCSmashableWindow : public CTCCameraCollision { public:
    virtual void v00();
    CTCSmashableWindow(CThing& argument);
};

CTCSmashableWindow::CTCSmashableWindow(CThing& argument)
    : CTCCameraCollision(&argument)
{
}