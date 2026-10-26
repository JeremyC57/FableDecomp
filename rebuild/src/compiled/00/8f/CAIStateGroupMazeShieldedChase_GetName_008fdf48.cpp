// CAIStateGroup_MazeShieldedChase::GetName() -- static: the AI state group name "SG_MAZE_SHIELDED_CHASE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MazeShieldedChase@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDF48: push esi; push -1; push "SG_MAZE_SHIELDED_CHASE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220c04 registers this GetName with Alloc 0x90272f (pushed before it); Alloc 0x90272f stores the vtable of CAIStateGroup_MazeShieldedChase into the new object; literal "SG_MAZE_SHIELDED_CHASE"; PDB name set ?GetName@CAIStateGroup_MazeShieldedChase@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MazeShieldedChase@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdf48::build).
#define CAIStateGroup_MazeShieldedChase CAIStateGroup_MazeShieldedChaseLayout
#include "engine/CAIStateGroup_MazeShieldedChase.h"
#undef CAIStateGroup_MazeShieldedChase

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MazeShieldedChase : public CAIStateGroup_MazeShieldedChaseLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MazeShieldedChase::GetName()
{
    return CCharString((const char*)0x01289F60, -1);   // "SG_MAZE_SHIELDED_CHASE"
}
