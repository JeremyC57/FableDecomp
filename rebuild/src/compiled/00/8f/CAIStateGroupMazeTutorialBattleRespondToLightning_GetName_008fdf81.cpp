// CAIStateGroup_MazeTutorialBattleRespondToLightning::GetName() -- static: the AI state group name "SG_MAZE_TUTORIAL_BATTLE_RESPOND_TO_LIGHTNING" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MazeTutorialBattleRespondToLightning@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDF81: push esi; push -1; push "SG_MAZE_TUTORIAL_BATTLE_RESPOND_TO_LIGHTNING"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220c7c registers this GetName with Alloc 0x90279b (pushed before it); Alloc 0x90279b stores the vtable of CAIStateGroup_MazeTutorialBattleRespondToLightning into the new object; literal "SG_MAZE_TUTORIAL_BATTLE_RESPOND_TO_LIGHTNING"; PDB name set ?GetName@CAIStateGroup_MazeTutorialBattleRespondToLightning@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MazeTutorialBattleRespondToLightning@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdf81::build).
#define CAIStateGroup_MazeTutorialBattleRespondToLightning CAIStateGroup_MazeTutorialBattleRespondToLightningLayout
#include "engine/CAIStateGroup_MazeTutorialBattleRespondToLightning.h"
#undef CAIStateGroup_MazeTutorialBattleRespondToLightning

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MazeTutorialBattleRespondToLightning : public CAIStateGroup_MazeTutorialBattleRespondToLightningLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MazeTutorialBattleRespondToLightning::GetName()
{
    return CCharString((const char*)0x01289FB4, -1);   // "SG_MAZE_TUTORIAL_BATTLE_RESPOND_TO_LIGHTNING"
}
