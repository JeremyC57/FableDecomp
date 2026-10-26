// CAIStateGroup_MazeDefensiveMeleeCombat::GetName() -- static: the AI state group name "SG_MAZE_DEFENSIVE_MELEE_COMBAT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MazeDefensiveMeleeCombat@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDF35: push esi; push -1; push "SG_MAZE_DEFENSIVE_MELEE_COMBAT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220bdc registers this GetName with Alloc 0x90270b (pushed before it); Alloc 0x90270b stores the vtable of CAIStateGroup_MazeDefensiveMeleeCombat into the new object; literal "SG_MAZE_DEFENSIVE_MELEE_COMBAT"; PDB name set ?GetName@CAIStateGroup_MazeDefensiveMeleeCombat@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MazeDefensiveMeleeCombat@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdf35::build).
#define CAIStateGroup_MazeDefensiveMeleeCombat CAIStateGroup_MazeDefensiveMeleeCombatLayout
#include "engine/CAIStateGroup_MazeDefensiveMeleeCombat.h"
#undef CAIStateGroup_MazeDefensiveMeleeCombat

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MazeDefensiveMeleeCombat : public CAIStateGroup_MazeDefensiveMeleeCombatLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MazeDefensiveMeleeCombat::GetName()
{
    return CCharString((const char*)0x01289F40, -1);   // "SG_MAZE_DEFENSIVE_MELEE_COMBAT"
}
