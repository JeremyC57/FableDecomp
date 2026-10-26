// CAIStateGroup_MinionReactToNoise::GetName() -- static: the AI state group name "SG_MINION_REACT_TO_NOISE" as a CCharString
// Leaf: ?GetName@CAIStateGroup_MinionReactToNoise@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE05C: push esi; push -1; push "SG_MINION_REACT_TO_NOISE"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1220fec registers this GetName with Alloc 0x900b01 (pushed before it); Alloc 0x900b01 stores the vtable of CAIStateGroup_MinionReactToNoise into the new object; literal "SG_MINION_REACT_TO_NOISE"; PDB name set ?GetName@CAIStateGroup_MinionReactToNoise@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_MinionReactToNoise@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe05c::build).
#define CAIStateGroup_MinionReactToNoise CAIStateGroup_MinionReactToNoiseLayout
#include "engine/CAIStateGroup_MinionReactToNoise.h"
#undef CAIStateGroup_MinionReactToNoise

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_MinionReactToNoise : public CAIStateGroup_MinionReactToNoiseLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_MinionReactToNoise::GetName()
{
    return CCharString((const char*)0x0128A0E4, -1);   // "SG_MINION_REACT_TO_NOISE"
}
