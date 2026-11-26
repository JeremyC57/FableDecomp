// CAIStateGroup_BuyFromSeller::CAIStateGroup_BuyFromSeller() -- default constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00919A40, length 39 bytes (+ int3 padding).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0, CCharString::CCharString() @ 0x0099E4B0 (the Name
// member at +0xc). Vtables: CAIStateGroupBase 0x01288C60 then CAIStateGroup_BuyFromSeller
// 0x0128FF0C (rtti-vtables.tsv). FrameLastBought (+0x14, engine header name) is zeroed last.
// Unlike most state groups this TU inlines CAIStateGroupBase's constructor (the base vtable
// store between the CBase call and the Name construction), so the base is modelled with an
// inline constructor here.
// NF469 (r467a, ruling r469a): retail's ~CAIStateGroupBase is the implicit one, kept once at 0x008FD288 (Ego_r / FableWin flag it
//   compiler-generated). This local model's implicit copy was not retail's (a lone jmp ~CBaseClass: the model gives Name (+0x0C) no destructor), so the destructor is declared here and the
//   object references 0x008FD288's ??1CAIStateGroupBase@@UAE@XZ like the 640 landed local models; the row's own bytes are unchanged.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CCharString { public:
    char* m_pData;
    CCharString();                          // 0x0099E4B0
};

class CAIStateGroupBase : public CBaseClass { public: virtual ~CAIStateGroupBase();          // vtable 0x01288C60
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