// CAIStateGroup_BanditPlot::GetName() -- static: the AI state group name "SG_BANDIT_PLOT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_BanditPlot@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE7DD: push esi; push -1; push "SG_BANDIT_PLOT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221654 registers this GetName with Alloc 0x901cd3 (pushed before it); Alloc 0x901cd3 stores the vtable of CAIStateGroup_BanditPlot into the new object; literal "SG_BANDIT_PLOT"; PDB name set ?GetName@CAIStateGroup_BanditPlot@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_BanditPlot@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe7dd::build).
#define CAIStateGroup_BanditPlot CAIStateGroup_BanditPlotLayout
#include "engine/CAIStateGroup_BanditPlot.h"
#undef CAIStateGroup_BanditPlot

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_BanditPlot : public CAIStateGroup_BanditPlotLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_BanditPlot::GetName()
{
    return CCharString((const char*)0x0128AD9C, -1);   // "SG_BANDIT_PLOT"
}
