// CAIStateGroup_WorkSawingWood::GetName() -- static: the AI state group name "SG_WORK_SAWING_WOOD" as a CCharString
// Leaf: ?GetName@CAIStateGroup_WorkSawingWood@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDA8F: push esi; push -1; push "SG_WORK_SAWING_WOOD"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x12203e4 registers this GetName with Alloc 0x9024c4 (pushed before it); Alloc 0x9024c4 -> ctor 0x8ffd84 stores (last) the vtable of CAIStateGroup_WorkSawingWood at this+0; literal "SG_WORK_SAWING_WOOD"; PDB name set ?GetName@CAIStateGroup_WorkSawingWood@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_WorkSawingWood@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fda8f::build).
#define CAIStateGroup_WorkSawingWood CAIStateGroup_WorkSawingWoodLayout
#include "engine/CAIStateGroup_WorkSawingWood.h"
#undef CAIStateGroup_WorkSawingWood

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_WorkSawingWood : public CAIStateGroup_WorkSawingWoodLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_WorkSawingWood::GetName()
{
    return CCharString((const char*)0x0128992C, -1);   // "SG_WORK_SAWING_WOOD"
}
