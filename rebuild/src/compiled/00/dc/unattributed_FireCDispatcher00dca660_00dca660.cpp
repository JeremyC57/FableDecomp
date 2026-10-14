// CDispatcher_00dca660::Fire() -- builds a CEvent_00dca660 (base vtable then its own, the counted pointer at +0x40 copied with addref) by value in the argument slots, calls the handler through the member function pointer at +0x34 on the target at +0x38, then sets the fired flag at +5
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCA660, oracle length 50 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; sub esp,0Ch ; mov eax,esp ; mov dword ptr [eax],offset base vtable ; mov dword ptr [eax],offset vtable ; mov esi,ecx ; mov ecx,[esi+40h] ; mov [eax+4],ecx ; mov ecx,[esi+44h] ; test ecx,ecx ; mov [eax+8],ecx ; je S ; inc dword ptr [ecx] ; S: mov ecx,[esi+38h] ; call [esi+34h] ; mov byte ptr [esi+5],1 ; pop esi ; ret
// Event dispatch: the 12-byte event (a polymorphic base whose inlined constructor stores the
// base vtable, then the class's own; a counted pointer copied from the dispatcher's member at
// +0x40 with its block's count at +0 bumped when present) is constructed directly in the
// reserved argument slots and passed by value (the callee destroys it) through the
// single-inheritance member function pointer at +0x34 on the handler object at +0x38; the
// fired flag at +5 is set afterwards. Classes are named by address; the vtables are
// relocations. One of 26 bodies of this shape.
struct SCountBlock {
    long m_references;                     // +0
};

struct CCountedPointer {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer();
};

struct CEventBase {
    virtual void v00();
    CEventBase() { }
};

struct CEvent_00dca660 : CEventBase {
    CCountedPointer m_thing;               // +4
    CEvent_00dca660(const CCountedPointer& thing) : m_thing(thing) { }
};

struct CHandler {                          // complete single-inheritance class: the member function pointer is one code address
    virtual void v00();
};

struct CDispatcher_00dca660 {
    unsigned char _pad0[5];
    bool m_fired;                          // +5
    unsigned char _pad1[0x34 - 6];
    void (CHandler::*m_pfnHandler)(CEvent_00dca660 event);   // +0x34
    CHandler* m_pHandler;                  // +0x38
    unsigned char _pad2[0x40 - 0x3c];
    CCountedPointer m_thing;               // +0x40
    void Fire_CDispatcher_00dca660();
};

void CDispatcher_00dca660::Fire_CDispatcher_00dca660()
{
    (m_pHandler->*m_pfnHandler)(CEvent_00dca660(m_thing));
    m_fired = true;
}