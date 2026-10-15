// CNavigationTask_012b3fdc::CNavigationTask_012b3fdc() -- constructor: own vtable, four zeroed words, the embedded 0x50-byte navigation request (inline constructor), two zeroed shorts
// Leaf: ??0CNavigationTask_012b3fdc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C266E0, length 90 bytes.
// Callees: CMessageListOwner::CMessageListOwner @ 0x00C203A0, CNavigationLayer::CNavigationLayer @ 0x00C20390
// (the request's two member constructors, the request's own constructor 0x00C10A30 being inlined here).
// Vtable: 0x012B3FDC (no complete-object locator: a no-RTTI module; sizeof 0x78 from `operator new(0x78)`
// in the only caller 0x00C12FE8). Class named by its vtable.
// Shape: a polymorphic root with an inline constructor: the vtable store first, then the plain
// initialisers +4/+8/+0x14/+0x18 through the shared ebx zero (the words at +0xc/+0x10/+0x1c/+0x20 are
// not initialised), the request at +0x24 addressed through esi, then the two shorts at +0x74/+0x76.
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
    SNavigationRequest()
        : m_1c(0), m_20(0), m_scaleX(1.0f), m_scaleY(1.0f), m_2c(0), m_40(0), m_44(0), m_48(0), m_4c(0)
    {
    }
};

struct CNavigationTask_012b3fdc {             // 0x78 bytes (operator new(0x78) at 0x00C12FCC), vtable 0x012B3FDC
    virtual void v00();
    long               m_04;                  // +0x04
    long               m_08;                  // +0x08
    long               m_0c;                  // +0x0c (not initialised here)
    long               m_10;                  // +0x10 (not initialised here)
    long               m_14;                  // +0x14
    long               m_18;                  // +0x18
    long               m_1c;                  // +0x1c (not initialised here)
    long               m_20;                  // +0x20 (not initialised here)
    SNavigationRequest m_request;             // +0x24
    short              m_74;                  // +0x74
    short              m_76;                  // +0x76
    CNavigationTask_012b3fdc();
};

CNavigationTask_012b3fdc::CNavigationTask_012b3fdc()
    : m_04(0), m_08(0), m_14(0), m_18(0), m_74(0), m_76(0)
{
}