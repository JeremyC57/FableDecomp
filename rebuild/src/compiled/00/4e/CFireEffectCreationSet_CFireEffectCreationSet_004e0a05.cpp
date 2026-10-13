// CFireEffectCreationSet::CFireEffectCreationSet() -- base CInitBaseClass(), this vtable, then the embedded CMember_004df974 member at +0x8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0A05, oracle length 26 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CInitBaseClass::CInitBaseClass ; lea ecx,[esi+0x8] ; mov dword ptr [esi],offset CFireEffectCreationSet::`vftable' ; call CMember_004df974::CMember_004df974 ; mov eax,esi ; pop esi ; ret
// Default constructor: base CInitBaseClass() @ 0x0099A310, this class's vtable (0x012421D4, named
// through rtti-vtables.tsv), then the member object at +0x8 is constructed out of line
// @ 0x004DF974. Base and member class names come from unique manifest labels, else from the
// callee address. One of 16 bodies of this shape.
struct CInitBaseClass {
    virtual void v00();
    CInitBaseClass();                           // 0x0099A310
};

struct CMember_004df974 {
    CMember_004df974();                              // 0x004DF974
};

struct CFireEffectCreationSet : CInitBaseClass {
    unsigned char _pad[0x4];
    CMember_004df974 m_member;                       // +0x8
    virtual void v00();
    CFireEffectCreationSet();
};

CFireEffectCreationSet::CFireEffectCreationSet()
    : CInitBaseClass()
    , m_member()
{
}