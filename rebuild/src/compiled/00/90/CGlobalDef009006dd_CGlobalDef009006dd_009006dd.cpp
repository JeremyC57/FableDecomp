// CGlobalDef_009006dd::~CGlobalDef_009006dd() -- the seven intelligent pointers at +0x44 down to +0x14, then the base (0x008FD288, a tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009006DD, length 67 bytes.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 seven times, the base destructor @ 0x008FD288 (labelled "_Dest_val<...>", propagated).
// Shape: members in reverse declaration order, the size-optimised module. The chunk label "CGlobalDef::~CGlobalDef" is shared with 0x004E37BF; named by address.
#pragma optimize("s", on)
struct CBaseIntelligentPointer {                   // 8 bytes
    virtual void v00();
    void* m_pItem;
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
struct CDefBase_008fd288 {                         // 0x14 bytes (no vtable store in this destructor: the class is not polymorphic here)
    unsigned char _body[0x14];
    ~CDefBase_008fd288();                          // 0x008FD288
};

struct CGlobalDef_009006dd : CDefBase_008fd288 {
    CBaseIntelligentPointer m_pointer14;           // +0x14
    CBaseIntelligentPointer m_pointer1c;           // +0x1c
    CBaseIntelligentPointer m_pointer24;           // +0x24
    CBaseIntelligentPointer m_pointer2c;           // +0x2c
    CBaseIntelligentPointer m_pointer34;           // +0x34
    CBaseIntelligentPointer m_pointer3c;           // +0x3c
    CBaseIntelligentPointer m_pointer44;           // +0x44
    ~CGlobalDef_009006dd();
};

CGlobalDef_009006dd::~CGlobalDef_009006dd()
{
}