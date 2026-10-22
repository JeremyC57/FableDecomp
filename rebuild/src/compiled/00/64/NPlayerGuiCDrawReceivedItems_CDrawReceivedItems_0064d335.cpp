#pragma optimize("s",on)
// NPlayerGui::CDrawReceivedItems::CDrawReceivedItems(CPlayerGui*, void*) -- forwards both arguments to CDrawBase, then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064D335, oracle length 28 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push [esp+0Ch] ; mov esi,ecx ; push [esp+0Ch] ; call CDrawBase::CDrawBase ; mov dword ptr [esi],offset CVtable_0125a574::`vftable' ; mov eax,esi ; pop esi ; ret 8
// Two-argument constructor of a polymorphic class that only forwards its arguments to the base
// constructor @ 0x0064CC65 and installs this class's vtable (0x0125A574, named through
// rtti-vtables.tsv). Argument types follow the base constructor's manifest prototype. One of
// 10 bodies of this shape.
struct CPlayerGui;

struct CDrawBase {
    virtual void v00();
    CDrawBase(CPlayerGui* first, void* second);   // 0x0064CC65
};

struct CDrawReceivedItems : CDrawBase {
    virtual void v00();
    CDrawReceivedItems(CPlayerGui* first, void* second);
};

CDrawReceivedItems::CDrawReceivedItems(CPlayerGui* first, void* second)
    : CDrawBase(first, second)
{
}