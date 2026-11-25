// CTCLightningNode::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCLightningNode::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCLightningNode@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E6D77, 16 bytes: mov ecx,[esp+4]; call CTCLightningNode::StatGetClassTypeName 0x004D6674; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCLightningNode@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124337C (RTTI .?AVCTCLightningNode@@) slot 22 (0x012433D4 -> 0x004E6D77) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D6674, the landed CTCLightningNode::StatGetClassTypeName (?StatGetClassTypeName@CTCLightningNode@@SI?AVCCharString@@XZ; literal "CTCLightningNode").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCLightningNode_GetClassTypeName@4 calling the made-up ?Init_004d6674@T@@QAEXXZ.
// Header: engine/CTCLightningNode.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCLightningNode CTCLightningNodeLayout
#include "engine/CTCLightningNode.h"
#undef CTCLightningNode

class CTCLightningNode : public CTCLightningNodeLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D6674
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCLightningNode::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
