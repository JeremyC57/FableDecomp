// CSoundThemeDef::CSoundThemeDef() -- base CBase_00430370(), this vtable, then the embedded CMember_0045433f member at +0x54
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455EAC, oracle length 26 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_00430370::CBase_00430370 ; lea ecx,[esi+0x54] ; mov dword ptr [esi],offset CSoundThemeDef::`vftable' ; call CMember_0045433f::CMember_0045433f ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_00430370() @ 0x00430370, this class's vtable (0x01233824, named
// through rtti-vtables.tsv), then the member object at +0x54 is constructed out of line
// @ 0x0045433F. Base and member class names come from unique manifest labels, else from the
// callee address. One of 16 bodies of this shape.
struct CBase_00430370 {
    virtual void v00();
    CBase_00430370();                           // 0x00430370
};

struct CMember_0045433f {
    CMember_0045433f();                              // 0x0045433F
};

struct CSoundThemeDef : CBase_00430370 {
    unsigned char _pad[0x50];
    CMember_0045433f m_member;                       // +0x54
    virtual void v00();
    CSoundThemeDef();
};

CSoundThemeDef::CSoundThemeDef()
    : CBase_00430370()
    , m_member()
{
}