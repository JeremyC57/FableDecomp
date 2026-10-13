// CSimVoicesDef::CSimVoicesDef() -- base CBase_00430370(), this vtable, then the embedded CMember_00455e39 member at +0x3c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455E72, oracle length 26 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_00430370::CBase_00430370 ; lea ecx,[esi+0x3c] ; mov dword ptr [esi],offset CSimVoicesDef::`vftable' ; call CMember_00455e39::CMember_00455e39 ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_00430370() @ 0x00430370, this class's vtable (0x012337BC, named
// through rtti-vtables.tsv), then the member object at +0x3c is constructed out of line
// @ 0x00455E39. Base and member class names come from unique manifest labels, else from the
// callee address. One of 16 bodies of this shape.
struct CBase_00430370 {
    virtual void v00();
    CBase_00430370();                           // 0x00430370
};

struct CMember_00455e39 {
    CMember_00455e39();                              // 0x00455E39
};

struct CSimVoicesDef : CBase_00430370 {
    unsigned char _pad[0x38];
    CMember_00455e39 m_member;                       // +0x3c
    virtual void v00();
    CSimVoicesDef();
};

CSimVoicesDef::CSimVoicesDef()
    : CBase_00430370()
    , m_member()
{
}