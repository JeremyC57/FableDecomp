// CAIStateGroup_PerformAction::GetName() -- static: the AI state group name "SG_PERFORM_ACTION" as a CCharString
// Leaf: ?GetName@CAIStateGroup_PerformAction@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD46B: push esi; push -1; push 0x01288F80 ("SG_PERFORM_ACTION"); mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning CCharString
//   by value (hidden result in ecx).
// Name fix (agentNF36 2026-11-16): was the stand-in free function GetActionName / CAIStateGroupNameTag. FableWin.pdb name set holds
//   ?GetName@CAIStateGroup_PerformAction@@SI?AVCCharString@@XZ; the StateGroupInfo initializer 0x0121F86F (landed) already binds it; literal "SG_PERFORM_ACTION" at 0x01288F80.
#define FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC
#include "engine/CAIStateGroup_PerformAction.h"                // declares static GetName()

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

CCharString FABLE_FASTCALL CAIStateGroup_PerformAction::GetName()
{
    return CCharString("SG_PERFORM_ACTION", -1);   // 0x01288F80
}
