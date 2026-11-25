// CTCExpression::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCExpression::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCExpression@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB072, 16 bytes: mov ecx,[esp+4]; call CTCExpression::StatGetClassTypeName 0x004D4B72; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCExpression@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012401F4 (RTTI .?AVCTCExpression@@) slot 22 (0x0124024C -> 0x004DB072) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4B72, the landed CTCExpression::StatGetClassTypeName (?StatGetClassTypeName@CTCExpression@@SI?AVCCharString@@XZ; literal "CTCExpression").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCExpression_GetClassTypeName@4 calling the made-up ?Init_004d4b72@T@@QAEXXZ.
// Header: engine/CTCExpression.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form); prelude as the landed includer
//   0x007EF200 CTCCarriedActionUseExpression::OnActionUse (its defines, CThing.h, CThingCreatureBase.h, CExpressionDef.h: CTCExpression.h's
//   inline accessors need CExpressionDef complete; smaller preludes hang VC7.1 under the lane).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_NOTEQUAL
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
#define FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISPLAYERCHARACTER
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUEST
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_PLAYER_GETNUMBER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CExpressionDef.h"
#define CTCExpression CTCExpressionLayout
#include "engine/CTCExpression.h"
#undef CTCExpression

class CTCExpression : public CTCExpressionLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4B72
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCExpression::GetClassTypeName() const
{
    return StatGetClassTypeName();
}

