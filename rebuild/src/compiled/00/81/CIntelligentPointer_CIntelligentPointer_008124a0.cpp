// CIntelligentPointer::CIntelligentPointer(const CIntelligentPointer&) -- copy constructor: base CBaseIntelligentPointer(), this vtable, SetPItem(other.GetPItem())
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008124A0, oracle length 37 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; mov ecx,[esp+8] ; mov dword ptr [esi],offset CIntelligentPointer::`vftable' ; call CBaseIntelligentPointer::GetPItem ; push eax ; mov ecx,esi ; call CBaseIntelligentPointer::SetPItem ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a smart-pointer instantiation: default-constructs the base
// @ 0x00A01B10, installs this instantiation's vtable (0x01264BAC, named through
// rtti-vtables.tsv; the template argument is not recoverable from the body) and re-points
// the new object at the other's item through GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90
// (the base class's accessors; SetPItem takes the item by pointer). One of 23 bodies.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();                           // 0x00A01B10
    CBaseObject* GetPItem() const;         // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other);
};

CIntelligentPointer::CIntelligentPointer(const CIntelligentPointer& other)
    : CBaseIntelligentPointer()
{
    SetPItem(other.GetPItem());
}