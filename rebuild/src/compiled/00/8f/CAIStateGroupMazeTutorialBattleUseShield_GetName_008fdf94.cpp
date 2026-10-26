// CAIStateGroup_MazeTutorialBattleUseShield::GetName() -- static: the AI state group name "SG_MAZE_TUTORIAL_BATTLE_USE_SHIELD" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MazeTutorialBattleUseShield@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDF94: push esi; push -1; push "SG_MAZE_TUTORIAL_BATTLE_USE_SHIELD"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220ca4 registers this GetName with Alloc 0x9027bf (pushed before it); Alloc 0x9027bf stores the vtable of CAIStateGroup_MazeTutorialBattleUseShield into the new object; literal "SG_MAZE_TUTORIAL_BATTLE_USE_SHIELD"; PDB name set ?GetName@CAIStateGroup_MazeTutorialBattleUseShield@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MazeTutorialBattleUseShield@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdf94::build).
#define CAIStateGroup_MazeTutorialBattleUseShield CAIStateGroup_MazeTutorialBattleUseShieldLayout
#include "engine/CAIStateGroup_MazeTutorialBattleUseShield.h"
#undef CAIStateGroup_MazeTutorialBattleUseShield

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MazeTutorialBattleUseShield : public CAIStateGroup_MazeTutorialBattleUseShieldLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MazeTutorialBattleUseShield::GetName()
{
    return CCharString((const char*)0x01289FE4, -1);   // "SG_MAZE_TUTORIAL_BATTLE_USE_SHIELD"
}
