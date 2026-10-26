// CAIStateGroup_MinionReactToPlayerInteraction::GetName() -- static: the AI state group name "SG_MINION_REACT_TO_PLAYER_INTERACTION" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionReactToPlayerInteraction@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FD330: push esi; push -1; push "SG_MINION_REACT_TO_PLAYER_INTERACTION"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x121f944 registers this GetName with Alloc 0x901f47 (pushed before it); Alloc 0x901f47 stores the vtable of CAIStateGroup_MinionReactToPlayerInteraction into the new object; literal "SG_MINION_REACT_TO_PLAYER_INTERACTION"; PDB name set ?GetName@CAIStateGroup_MinionReactToPlayerInteraction@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionReactToPlayerInteraction@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fd330::build).
#define CAIStateGroup_MinionReactToPlayerInteraction CAIStateGroup_MinionReactToPlayerInteractionLayout
#include "engine/CAIStateGroup_MinionReactToPlayerInteraction.h"
#undef CAIStateGroup_MinionReactToPlayerInteraction

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionReactToPlayerInteraction : public CAIStateGroup_MinionReactToPlayerInteractionLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionReactToPlayerInteraction::GetName()
{
    return CCharString((const char*)0x01288D20, -1);   // "SG_MINION_REACT_TO_PLAYER_INTERACTION"
}
