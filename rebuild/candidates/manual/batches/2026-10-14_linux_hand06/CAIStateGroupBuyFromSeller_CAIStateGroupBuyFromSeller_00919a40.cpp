// CAIStateGroup_BuyFromSeller::CAIStateGroup_BuyFromSeller() -- default constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00919A40, length 39 bytes (+ int3 padding).
// Callees: CBase::CBase @ 0x0099A2D0, CCharString::CCharString() @ 0x0099E4B0 (the Name
// member at +0xc). Vtables: CAIStateGroupBase 0x01288C60 then CAIStateGroup_BuyFromSeller
// 0x0128FF0C (rtti-vtables.tsv). FrameLastBought (+0x14, engine header name) is zeroed last.
// Unlike most state groups this TU inlines CAIStateGroupBase's constructor (the base vtable
// store between the CBase call and the Name construction), so the base is modelled with an
// inline constructor here.
struct CBase {
    virtual ~CBase();
    CBase();                                // 0x0099A2D0
};

struct CCharString {
    char* m_pData;
    CCharString();                          // 0x0099E4B0
};

struct CAIStateGroupBase : CBase {          // vtable 0x01288C60
    void*       PBrain;                     // +0x04
    long        ScriptAIPriority;           // +0x08
    CCharString Name;                       // +0x0c
    bool        Terminated;                 // +0x10
    unsigned char Priority;                 // +0x11
    bool        Enabled;                    // +0x12
    CAIStateGroupBase() : Name() { }
};

struct CAIStateGroup_BuyFromSeller : CAIStateGroupBase {    // vtable 0x0128FF0C
    long FrameLastBought;                   // +0x14
    CAIStateGroup_BuyFromSeller();
};

CAIStateGroup_BuyFromSeller::CAIStateGroup_BuyFromSeller()
    : FrameLastBought(0)
{
}
