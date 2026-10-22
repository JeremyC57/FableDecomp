struct CCharString; struct Elem40 { char _[40]; };
extern Elem40 g_dbgtable[];
struct S { char pad[0x18]; unsigned long CurrentReactionType; };
const CCharString* __fastcall GetDebugText(S* stateGroup){ return (const CCharString*)&g_dbgtable[stateGroup->CurrentReactionType]; }