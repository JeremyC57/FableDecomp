// CTCTrespassable::CTCTrespassable(CThing* parent) -- CTCBase(parent), Trespassable = IsBeingTrespassedIn = false, own vtable
// Leaf: CTCTrespassable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BAD20, length 33 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800.
// Vtable 0x0126DD9C = CTCTrespassable (rtti-vtables.tsv): the chunk label "CTCEditor" is propagated.
// PDB CTCTrespassable (0x10 bytes): Trespassable +0x0c, IsBeingTrespassedIn +0x0d.
// Shape: speed TU; `xor al,al` and both byte initialisers are scheduled before the derived vtable store.
struct CThing;

struct CTCBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                          // 0x00686800
};

struct CTCTrespassable : CTCBase_Calls {
    bool Trespassable;                                      // +0x0c
    bool IsBeingTrespassedIn;                               // +0x0d
    virtual void v00();                                     // own vtable 0x0126DD9C
    CTCTrespassable(CThing* parent);
};

CTCTrespassable::CTCTrespassable(CThing* parent)
    : CTCBase_Calls(parent)
    , Trespassable(false)
    , IsBeingTrespassedIn(false)
{
}
