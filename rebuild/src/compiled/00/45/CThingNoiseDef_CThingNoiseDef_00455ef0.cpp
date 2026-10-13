// CThingNoiseDef::CThingNoiseDef() -- base CThingBaseDef(), this vtable, then the embedded CTCNoiseDef member at +0x70
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455EF0, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov esi,ecx ; call CThingBaseDef::CThingBaseDef ; lea edi,[esi+0x70] ; mov ecx,edi ; mov dword ptr [esi],offset CThingNoiseDef::`vftable' ; call CMemberBase_0044c0c0::CMemberBase_0044c0c0 ; mov dword ptr [edi],offset CTCNoiseDef::`vftable' ; pop edi ; mov eax,esi ; pop esi ; ret
// Default constructor: base CThingBaseDef() @ 0x006772D0 out of line, this class's vtable
// (0x0123388C), then the embedded member of type CTCNoiseDef at +0x70 is constructed with its
// own constructor inlined: its base CMemberBase_0044c0c0() @ 0x0044C0C0 and its vtable (0x01231E9C).
// Class names come from rtti-vtables.tsv; base-constructor names from unique manifest labels.
// One of 73 bodies of this shape.
struct CThingBaseDef {
    virtual void v00();
    CThingBaseDef();                           // 0x006772D0
};

struct CMemberBase_0044c0c0 {
    virtual void v00();
    CMemberBase_0044c0c0();                          // 0x0044C0C0
};

struct CTCNoiseDef : CMemberBase_0044c0c0 {
    virtual void v00();
    CTCNoiseDef() : CMemberBase_0044c0c0() { }            // inlined into every constructor that embeds it
};

struct CThingNoiseDef : CThingBaseDef {
    unsigned char _pad[0x6c];        // base members / earlier members, not touched here
    CTCNoiseDef m_member;                       // +0x70
    virtual void v00();
    CThingNoiseDef();
};

CThingNoiseDef::CThingNoiseDef()
    : CThingBaseDef()
    , m_member()
{
}