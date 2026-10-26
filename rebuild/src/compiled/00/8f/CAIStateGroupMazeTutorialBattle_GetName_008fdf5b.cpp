// CAIStateGroup_MazeTutorialBattle::GetName() -- static: the AI state group name "SG_MAZE_TUTORIAL_BATTLE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MazeTutorialBattle@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDF5B: push esi; push -1; push "SG_MAZE_TUTORIAL_BATTLE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220c2c registers this GetName with Alloc 0x902753 (pushed before it); Alloc 0x902753 stores the vtable of CAIStateGroup_MazeTutorialBattle into the new object; literal "SG_MAZE_TUTORIAL_BATTLE"; PDB name set ?GetName@CAIStateGroup_MazeTutorialBattle@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MazeTutorialBattle@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdf5b::build).
#define CAIStateGroup_MazeTutorialBattle CAIStateGroup_MazeTutorialBattleLayout
#include "engine/CAIStateGroup_MazeTutorialBattle.h"
#undef CAIStateGroup_MazeTutorialBattle

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MazeTutorialBattle : public CAIStateGroup_MazeTutorialBattleLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MazeTutorialBattle::GetName()
{
    return CCharString((const char*)0x01289F78, -1);   // "SG_MAZE_TUTORIAL_BATTLE"
}
