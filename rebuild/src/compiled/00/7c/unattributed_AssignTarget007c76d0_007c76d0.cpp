// CClass_007c76d0::AssignTarget_007c76d0(CBaseObject*) -- routes the argument through a temporary CIntelligentPointer into the member pointer at +0x24
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C76D0, oracle length 71 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; lea ecx,[esp+4] ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; mov eax,[esp+10h] ; push eax ; lea ecx,[esp+8] ; mov dword ptr [esp+8],offset CIntelligentPointer::`vftable' ; call SetPItem ; lea ecx,[esp+4] ; call GetPItem ; push eax ; lea ecx,[esi+0x24] ; call SetPItem ; lea ecx,[esp+4] ; call ~CBaseIntelligentPointer ; pop esi ; add esp,8 ; ret 4
// A stack CIntelligentPointer (8 bytes: vtable 0x01238C6C from rtti-vtables.tsv + item; constructed by the
// inlined default ctor over CBaseIntelligentPointer() @ 0x00A01B10) is pointed at the argument
// with SetPItem @ 0x00A01B90, read back with GetPItem @ 0x00A01B50 into the member pointer at
// +0x24, and destroyed @ 0x00A01C10. The class comes from the manifest label when unique.
// One of 27 bodies of this shape.
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                  // +4 (an 8-byte temporary: `sub esp,8`)
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CBaseObject* GetPItem();               // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CClass_007c76d0 {
    unsigned char _pad[0x24];
    CBaseIntelligentPointer m_target;      // +0x24
    void AssignTarget_007c76d0(CBaseObject* thing);
};

void CClass_007c76d0::AssignTarget_007c76d0(CBaseObject* thing)
{
    CIntelligentPointer pointer;
    pointer.SetPItem(thing);
    m_target.SetPItem(pointer.GetPItem());
}