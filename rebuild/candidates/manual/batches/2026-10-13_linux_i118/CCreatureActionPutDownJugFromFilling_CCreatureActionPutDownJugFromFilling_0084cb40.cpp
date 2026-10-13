// CCreatureAction_PutDownJugFromFilling::CCreatureAction_PutDownJugFromFilling(const CCreatureAction_PutDownJugFromFilling&) -- copy constructor: base CCreatureActionBase(other), own vtable, CIntelligentPointer member at +0xa8 copied via GetPItem/SetPItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084CB40, oracle length 67 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; mov ebx,[esp+8] ; push esi ; push edi ; push ebx ; mov esi,ecx ; call CCreatureActionBase::CCreatureActionBase(const&) ; lea edi,[esi+0xa8] ; mov ecx,edi ; mov dword ptr [esi],offset CCreatureAction_PutDownJugFromFilling::`vftable' ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; lea ecx,[ebx+0xa8] ; mov dword ptr [edi],offset CIntelligentPointer::`vftable' ; call GetPItem ; push eax ; mov ecx,edi ; call SetPItem ; pop edi ; mov eax,esi ; pop esi ; pop ebx ; ret 4
// Copy constructor: the base copy constructor @ 0x0062EBC0 runs out of line, this class's vtable
// (0x01275634) is installed, then the CIntelligentPointer member at +0xa8 is copy-constructed with the
// family-44 copy constructor inlined: base CBaseIntelligentPointer() @ 0x00A01B10, its vtable (0x01238C6C),
// SetPItem(other.GetPItem()) (@ 0x00A01B90 / 0x00A01B50). Class names from rtti-vtables.tsv.
// One of 9 bodies of this shape.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;         // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CCreatureActionBase {
    virtual void v00();
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

struct CCreatureAction_PutDownJugFromFilling : CCreatureActionBase {
    unsigned char _pad[0xa4];
    CIntelligentPointer m_target;                       // +0xa8
    virtual void v00();
    CCreatureAction_PutDownJugFromFilling(const CCreatureAction_PutDownJugFromFilling& other);
};

CCreatureAction_PutDownJugFromFilling::CCreatureAction_PutDownJugFromFilling(const CCreatureAction_PutDownJugFromFilling& other)
    : CCreatureActionBase(other)
    , m_target(other.m_target)
{
}
