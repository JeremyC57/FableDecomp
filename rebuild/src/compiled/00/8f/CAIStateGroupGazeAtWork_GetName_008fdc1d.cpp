// CAIStateGroup_GazeAtWork::GetName() -- static: the AI state group name "SG_GAZE_AT_WORK" as a CCharString
// Leaf: ?GetName@CAIStateGroup_GazeAtWork@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FDC1D: push esi; push -1; push "SG_GAZE_AT_WORK"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122054c registers this GetName with Alloc 0x8fdc07 (pushed before it); Alloc 0x8fdc07 -> ctor 0x9182d0 stores (last) the vtable of CAIStateGroup_GazeAtWork at this+0; literal "SG_GAZE_AT_WORK"; PDB name set ?GetName@CAIStateGroup_GazeAtWork@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_GazeAtWork@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fdc1d::build).
#define CAIStateGroup_GazeAtWork CAIStateGroup_GazeAtWorkLayout
#include "engine/CAIStateGroup_GazeAtWork.h"
#undef CAIStateGroup_GazeAtWork

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_GazeAtWork : public CAIStateGroup_GazeAtWorkLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_GazeAtWork::GetName()
{
    return CCharString((const char*)0x01289B68, -1);   // "SG_GAZE_AT_WORK"
}
