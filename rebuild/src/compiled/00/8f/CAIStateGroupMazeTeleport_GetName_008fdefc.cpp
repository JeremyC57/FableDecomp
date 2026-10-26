// CAIStateGroup_MazeTeleport::GetName() -- static: the AI state group name "SG_MAZE_TELEPORT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MazeTeleport@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDEFC: push esi; push -1; push "SG_MAZE_TELEPORT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220b8c registers this GetName with Alloc 0x902e01 (pushed before it); Alloc 0x902e01 stores the vtable of CAIStateGroup_MazeTeleport into the new object; literal "SG_MAZE_TELEPORT"; PDB name set ?GetName@CAIStateGroup_MazeTeleport@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MazeTeleport@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdefc::build).
#define CAIStateGroup_MazeTeleport CAIStateGroup_MazeTeleportLayout
#include "engine/CAIStateGroup_MazeTeleport.h"
#undef CAIStateGroup_MazeTeleport

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MazeTeleport : public CAIStateGroup_MazeTeleportLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MazeTeleport::GetName()
{
    return CCharString((const char*)0x01289F14, -1);   // "SG_MAZE_TELEPORT"
}
