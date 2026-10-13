// CAIStateGroup_ThrowLogsOnFire::CAIStateGroup_ThrowLogsOnFire() -- base CAIStateGroup_MoveGoodsBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FDAED, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CAIStateGroup_MoveGoodsBase::CAIStateGroup_MoveGoodsBase ; mov dword ptr [esi],offset CAIStateGroup_ThrowLogsOnFire::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x008FDA5D runs, then the compiler stores this class's vtable (0x012899C4
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
struct CAIStateGroup_MoveGoodsBase {
    virtual void v00();
    CAIStateGroup_MoveGoodsBase();                           // 0x008FDA5D
};

struct CAIStateGroup_ThrowLogsOnFire : CAIStateGroup_MoveGoodsBase {
    virtual void v00();                    // this class has its own vtable
    CAIStateGroup_ThrowLogsOnFire();
};

CAIStateGroup_ThrowLogsOnFire::CAIStateGroup_ThrowLogsOnFire()
    : CAIStateGroup_MoveGoodsBase()
{
}