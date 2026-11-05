// Leaf: ?CAIStateGroup_OpinionReactionRoot_GetDebugText@@YIPBVCCharString@@PAUS@@@Z
class CCharString; struct Elem40 { char _[40]; };
extern Elem40 g_dbgtable[];
struct S { char pad[0x18]; unsigned long CurrentReactionType; };
const CCharString* __fastcall CAIStateGroup_OpinionReactionRoot_GetDebugText(S* stateGroup){ return (const CCharString*)&g_dbgtable[stateGroup->CurrentReactionType]; }