// CVtable_012b3420_Derived_00c202f0::~CVtable_012b3420_Derived_00c202f0() -- EH-framed, novtable: the string member at +0x10 is destroyed, then the inlined empty destructor of the two-vptr base stores its two vtables (+8: 0x012B3444, +0: 0x012B3420)
// Grade: DIFFER(ours v retail) -- 74v74: the two closing vtable stores come out +0 then +8; retail stores +8 then +0
// Leaf: ??1CVtable_012b3420_Derived_00c202f0
// Flags: /EHsc
// Retail: Fable.exe @ 0x00C202F0, 74 bytes (+ int3 padding). Built with C++ exceptions (fs:[0] frame, handler 0x012129F8).
// Callee: the member's destructor @ 0x00C0BA40 (a CCharString-style release: null-data warning, then the shared
// empty-string check against 0x0139C8FC; labelled "CCharStringData::Release").
// Label: the manifest's "CTargetPoint::CTargetPoint" is wrong -- this is a DESTRUCTOR: its only caller 0x00C202D0 is the
// scalar deleting destructor (`call 0x00C202F0 ; test [esp+8],1 ; operator delete`), and PDB CTargetPoint (0x1c bytes,
// CIntelligentPointer + Index + TargetingPosition + FrameRejected) has no vtable. The class (no RTTI) cannot be named from
// the body; named after its base's primary vtable, as the landed family 0x00C20000 / 0x00C1FF90.
// Tried (4 grades): (1) the +8 vptr as a second base of the novtable derived class -> a null-checked this-adjust for the
// second base destructor and two EH states (90 bytes); (2) THIS FORM, one base holding both vptrs with a user empty inline
// destructor -> exact apart from the store order; (3) an intermediate base with an implicit destructor over two bases with
// user empty destructors, and (4) the same with a novtable user-destructor intermediate -> the right +8/+0 order but the
// second base's inlined destructor gets the null-checked this-adjust (88 bytes).
// Next: a two-vptr base whose vtables are stored by its OWN inline destructor in +8/+0 order -- e.g. the second
// interface declared FIRST in a pack-free layout trick is not available; try a `__forceinline` second-base destructor or
// a member-object model with a polymorphic second sub-object whose EH state is merged (lane gap suspected: vptr store order).
struct CStringMember_00c0ba40 {
    void* m_pData;
    ~CStringMember_00c0ba40();                     // 0x00C0BA40
};

struct IInterfaceA {                               // first interface (no destructor of its own)
    virtual void v00();
    long m_word4;                                  // +0x04
};

struct IInterfaceB {                               // second interface, at +8
    virtual void w00();
    long m_word4;                                  // +0x04
};

struct CVtable_012b3420 : IInterfaceA, IInterfaceB {   // vtables 0x012B3420 (+0) / 0x012B3444 (+8)
    virtual void v00();
    virtual void w00();
    ~CVtable_012b3420() { }
};

struct __declspec(novtable) CVtable_012b3420_Derived_00c202f0 : CVtable_012b3420 {
    CStringMember_00c0ba40 m_member;               // +0x10
    virtual void v00();
    virtual void w00();
    ~CVtable_012b3420_Derived_00c202f0();
};

CVtable_012b3420_Derived_00c202f0::~CVtable_012b3420_Derived_00c202f0()
{
}
