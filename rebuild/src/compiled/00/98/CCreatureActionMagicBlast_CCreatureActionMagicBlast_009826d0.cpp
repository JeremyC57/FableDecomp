// CCreatureAction_MagicBlast::CCreatureAction_MagicBlast(const CCreatureAction_MagicBlast&) -- copy constructor: base CCreatureActionBase(other), own vtable, CIntelligentPointer member at +0xa8 copied via GetPItem/SetPItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009826D0, oracle length 67 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; mov ebx,[esp+8] ; push esi ; push edi ; push ebx ; mov esi,ecx ; call CCreatureActionBase::CCreatureActionBase(const&) ; lea edi,[esi+0xa8] ; mov ecx,edi ; mov dword ptr [esi],offset CCreatureAction_MagicBlast::`vftable' ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; lea ecx,[ebx+0xa8] ; mov dword ptr [edi],offset CIntelligentPointer::`vftable' ; call GetPItem ; push eax ; mov ecx,edi ; call SetPItem ; pop edi ; mov eax,esi ; pop esi ; pop ebx ; ret 4
// Copy constructor: the base copy constructor @ 0x0062EBC0 runs out of line, this class's vtable
// (0x0129935C) is installed, then the CIntelligentPointer member at +0xa8 is copy-constructed with the
// family-44 copy constructor inlined: base CBaseIntelligentPointer() @ 0x00A01B10, its vtable (0x0125AAEC),
// SetPItem(other.GetPItem()) (@ 0x00A01B90 / 0x00A01B50). Class names from rtti-vtables.tsv.
// One of 30 bodies of this shape.
class CBaseObject;

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

class CCreatureActionBase { public:
    virtual void v00();
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

class CCreatureAction_MagicBlast : public CCreatureActionBase { public:
    unsigned char _pad[0xa4];
    CIntelligentPointer m_target;                       // +0xa8
    virtual void v00();
    CCreatureAction_MagicBlast(const CCreatureAction_MagicBlast& other);
};

CCreatureAction_MagicBlast::CCreatureAction_MagicBlast(const CCreatureAction_MagicBlast& other)
    : CCreatureActionBase(other)
    , m_target(other.m_target)
{
}