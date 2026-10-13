// CVtable_012b2644::CVtable_012b2644(CPlayerGui*) -- base CDrawQuickAccessIconsPC(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13D20, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CDrawQuickAccessIconsPC::CDrawQuickAccessIconsPC ; mov dword ptr [esi],offset CVtable_012b2644::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x00C13A00; the compiler then stores this class's vtable (0x012B2644, named
// through rtti-vtables.tsv) and returns `this`. One of 12 bodies of this shape.
struct CPlayerGui;

struct CDrawQuickAccessIconsPC {
    virtual void v00();
    CDrawQuickAccessIconsPC(CPlayerGui* argument);            // 0x00C13A00
};

struct CVtable_012b2644 : CDrawQuickAccessIconsPC {
    virtual void v00();
    CVtable_012b2644(CPlayerGui* argument);
};

CVtable_012b2644::CVtable_012b2644(CPlayerGui* argument)
    : CDrawQuickAccessIconsPC(argument)
{
}