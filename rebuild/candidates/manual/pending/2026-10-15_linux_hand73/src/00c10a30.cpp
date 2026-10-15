// SNavigationRequest::SNavigationRequest() -- constructor of the 0x50-byte navigation request record: message-list owner, zero words, two 1.0f scales, an empty navigation layer, zero tail
// Leaf: ??0SNavigationRequest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10A30, length 57 bytes. No vtable.
// Callees: CMessageListOwner::CMessageListOwner @ 0x00C203A0 (on +0), CNavigationLayer::CNavigationLayer
// @ 0x00C20390 (on +0x30) -- both landed (0x00C203A0, 0x00C20390), out of line.
// Shape: initialiser list in declaration order around the two member constructors; the integer zeros share
// edi, the two 1.0f scales share eax (`mov eax,3F800000h`), the word at +0x44 is `mov [esi+44h],di`.
// The same record is built inline on the stack by the landed 0x00C12D50 / 0x00C13310 (named there
// SNavigationRequest_00c13310) and embedded at +0x24 of the 0x00C266E0 task object; callers 0x00C12EED, 0x00C61DAD.
// The manifest label "CScriptModuleBase::CScriptModuleBase" is propagated from an unrelated body.
struct CMessageListOwner {                    // 0x1c bytes
    unsigned char _body[0x1c];
    CMessageListOwner();                      // 0x00C203A0
};

struct CNavigationLayer {                     // 0x10 bytes
    void* m_pFirst;
    void* m_pSecond;
    void* m_pThird;
    unsigned char m_flag;
    CNavigationLayer();                       // 0x00C20390
};

struct SNavigationRequest {                   // 0x50 bytes
    CMessageListOwner m_messages;             // +0x00
    long              m_1c;                   // +0x1c
    long              m_20;                   // +0x20
    float             m_scaleX;               // +0x24
    float             m_scaleY;               // +0x28
    long              m_2c;                   // +0x2c
    CNavigationLayer  m_layer;                // +0x30
    long              m_40;                   // +0x40
    short             m_44;                   // +0x44
    long              m_48;                   // +0x48
    long              m_4c;                   // +0x4c
    SNavigationRequest();
};

SNavigationRequest::SNavigationRequest()
    : m_1c(0), m_20(0), m_scaleX(1.0f), m_scaleY(1.0f), m_2c(0), m_40(0), m_44(0), m_48(0), m_4c(0)
{
}
