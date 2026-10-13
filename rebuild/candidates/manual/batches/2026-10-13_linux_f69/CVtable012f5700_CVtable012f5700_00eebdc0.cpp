// CVtable_012f5700::CVtable_012f5700(void*) -- base CBase_00cb8110(), the argument stored at +0x40, then this vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EEBDC0, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_00cb8110::CBase_00cb8110 ; mov eax,[esp+8] ; mov [esi+0x40],eax ; mov dword ptr [esi],offset CVtable_012f5700::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor: base CBase_00cb8110() @ 0x00CB8110, the argument kept at +0x40 and
// this class's vtable (0x012F5700, named through rtti-vtables.tsv). The argument type is not
// recoverable from the body; `void*` models it. One of 15 bodies of this shape.
struct CBase_00cb8110 {
    virtual void v00();
    CBase_00cb8110();                           // 0x00CB8110
};

struct CVtable_012f5700 : CBase_00cb8110 {
    unsigned char _pad[0x3c];
    void* m_argument;                      // +0x40
    virtual void v00();
    CVtable_012f5700(void* argument);
};

CVtable_012f5700::CVtable_012f5700(void* argument)
    : CBase_00cb8110()
    , m_argument(argument)
{
}
