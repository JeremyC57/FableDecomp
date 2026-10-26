// CCreatureAction_WaspQueenDeath::CCreatureAction_WaspQueenDeath(const CCreatureAction_WaspQueenDeath&) -- copy constructor: base CBase_0062f3b0(other), own vtable, CIntelligentPointer member at +0x114 copied via GetPItem/SetPItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F8560, oracle length 67 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; mov ebx,[esp+8] ; push esi ; push edi ; push ebx ; mov esi,ecx ; call CBase_0062f3b0::CBase_0062f3b0(const&) ; lea edi,[esi+0x114] ; mov ecx,edi ; mov dword ptr [esi],offset CCreatureAction_WaspQueenDeath::`vftable' ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; lea ecx,[ebx+0x114] ; mov dword ptr [edi],offset CIntelligentPointer::`vftable' ; call GetPItem ; push eax ; mov ecx,edi ; call SetPItem ; pop edi ; mov eax,esi ; pop esi ; pop ebx ; ret 4
// Copy constructor: the base copy constructor @ 0x0062F3B0 runs out of line, this class's vtable
// (0x012629BC) is installed, then the CIntelligentPointer member at +0x114 is copy-constructed with the
// family-44 copy constructor inlined: base CBaseIntelligentPointer() @ 0x00A01B10, its vtable (0x01238C6C),
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

struct CBase_0062f3b0 {
    virtual void v00();
    CBase_0062f3b0(const CBase_0062f3b0& other);     // 0x0062F3B0
};

class CCreatureAction_WaspQueenDeath : public CBase_0062f3b0 { public:
    unsigned char _pad[0x110];
    CIntelligentPointer m_target;                       // +0x114
    virtual void v00();
    CCreatureAction_WaspQueenDeath(const CCreatureAction_WaspQueenDeath& other);
};

CCreatureAction_WaspQueenDeath::CCreatureAction_WaspQueenDeath(const CCreatureAction_WaspQueenDeath& other)
    : CBase_0062f3b0(other)
    , m_target(other.m_target)
{
}