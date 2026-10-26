// CAIStateGroup_MazeRangedCombat::GetName() -- static: the AI state group name "SG_MAZE_RANGED_COMBAT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MazeRangedCombat@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDF22: push esi; push -1; push "SG_MAZE_RANGED_COMBAT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220bb4 registers this GetName with Alloc 0x9026e7 (pushed before it); Alloc 0x9026e7 stores the vtable of CAIStateGroup_MazeRangedCombat into the new object; literal "SG_MAZE_RANGED_COMBAT"; PDB name set ?GetName@CAIStateGroup_MazeRangedCombat@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MazeRangedCombat@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdf22::build).
#define CAIStateGroup_MazeRangedCombat CAIStateGroup_MazeRangedCombatLayout
#include "engine/CAIStateGroup_MazeRangedCombat.h"
#undef CAIStateGroup_MazeRangedCombat

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MazeRangedCombat : public CAIStateGroup_MazeRangedCombatLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MazeRangedCombat::GetName()
{
    return CCharString((const char*)0x01289F28, -1);   // "SG_MAZE_RANGED_COMBAT"
}
