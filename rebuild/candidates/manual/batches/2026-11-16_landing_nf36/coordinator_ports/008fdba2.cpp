// CAIStateGroup_PrepareMeal::GetName() -- static: the AI state group name "SG_PREPARE_MEAL" as a CCharString
// Leaf: ?GetName@CAIStateGroup_PrepareMeal@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDBA2: push esi; push -1; push 0x01289B38 ("SG_PREPARE_MEAL"); mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall returning CCharString
//   by value (hidden result in ecx).
// Name fix (agentNF36 2026-11-16): was the stand-in free function GetActionName / CAIStateGroupNameTag. FableWin.pdb name set holds
//   ?GetName@CAIStateGroup_PrepareMeal@@SI?AVCCharString@@XZ; the StateGroupInfo initializer 0x0121F86F (landed) already binds it; literal "SG_PREPARE_MEAL" at 0x01289B38.
#define FABLE_RETAIL_CAISTATEGROUP_GETNAME_ALLOC
#include "engine/CAIStateGroup_PrepareMeal.h"                // declares static GetName()

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

CCharString FABLE_FASTCALL CAIStateGroup_PrepareMeal::GetName()
{
    return CCharString("SG_PREPARE_MEAL", -1);   // 0x01289B38
}
