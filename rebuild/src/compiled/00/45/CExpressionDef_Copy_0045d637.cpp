// CExpressionDef::Copy(const CDefClassBase* other) -- copy every member of another CExpressionDef: `*this = *(const CExpressionDef*)other;`
// Leaf: ?Copy@CExpressionDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045D637, 5 bytes: a thiscall tail call, `jmp` to CExpressionDef::operator= 0x0045D63C.
// Identity (NF425, 2026-11-25): self ?Copy@CExpressionDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CExpressionDef@@QAEAAV0@ABV0@@Z: bound at 0x0045D63C (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the
//   ancestor operator= ??4CParentDefClassBase (0x00431ED0): a derived class's assignment.
// Respell (NF425): was the stand-in self _CExpressionDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: the class's own header engine/CExpressionDef.h with the prelude of 00/4d/CTCExpression_GetClassTypeName_004db072.cpp (the
//   header declares Copy and operator=).
#define FABLE_RETAIL_CCHARSTRING
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

void CExpressionDef::Copy(const CDefClassBase* other)
{
    *this = *(const CExpressionDef*)other;
}
