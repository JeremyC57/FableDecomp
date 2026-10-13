// CDefPointeeBase::CDefPointeeBase() -- base CResource() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042D910, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CResource::CResource ; mov dword ptr [esi],offset CDefPointeeBase::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x009FBEC0 runs, then the compiler stores this class's vtable (0x01230BA0
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
struct CResource {
    virtual void v00();
    CResource();                           // 0x009FBEC0
};

struct CDefPointeeBase : CResource {
    virtual void v00();                    // this class has its own vtable
    CDefPointeeBase();
};

CDefPointeeBase::CDefPointeeBase()
    : CResource()
{
}