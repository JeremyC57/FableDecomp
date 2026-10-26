// CAIStateGroup_MazeTutorialBattleTeleport::GetName() -- static: the AI state group name "SG_MAZE_TUTORIAL_BATTLE_TELEPORT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MazeTutorialBattleTeleport@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDF6E: push esi; push -1; push "SG_MAZE_TUTORIAL_BATTLE_TELEPORT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220c54 registers this GetName with Alloc 0x902777 (pushed before it); Alloc 0x902777 stores the vtable of CAIStateGroup_MazeTutorialBattleTeleport into the new object; literal "SG_MAZE_TUTORIAL_BATTLE_TELEPORT"; PDB name set ?GetName@CAIStateGroup_MazeTutorialBattleTeleport@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MazeTutorialBattleTeleport@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdf6e::build).
#define CAIStateGroup_MazeTutorialBattleTeleport CAIStateGroup_MazeTutorialBattleTeleportLayout
#include "engine/CAIStateGroup_MazeTutorialBattleTeleport.h"
#undef CAIStateGroup_MazeTutorialBattleTeleport

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MazeTutorialBattleTeleport : public CAIStateGroup_MazeTutorialBattleTeleportLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MazeTutorialBattleTeleport::GetName()
{
    return CCharString((const char*)0x01289F90, -1);   // "SG_MAZE_TUTORIAL_BATTLE_TELEPORT"
}
