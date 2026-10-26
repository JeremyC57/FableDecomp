// CAIStateGroup_WhisperRetreat::GetName() -- static: the AI state group name "SG_WHISPER_RETREAT" as a CCharString
// Leaf: ?GetName@CAIStateGroup_WhisperRetreat@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE547: push esi; push -1; push "SG_WHISPER_RETREAT"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x122135c registers this GetName with Alloc 0x902c29 (pushed before it); Alloc 0x902c29 -> ctor 0x901867 stores (last) the vtable of CAIStateGroup_WhisperRetreat at this+0; literal "SG_WHISPER_RETREAT"; PDB name set ?GetName@CAIStateGroup_WhisperRetreat@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_WhisperRetreat@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe547::build).
#define CAIStateGroup_WhisperRetreat CAIStateGroup_WhisperRetreatLayout
#include "engine/CAIStateGroup_WhisperRetreat.h"
#undef CAIStateGroup_WhisperRetreat

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_WhisperRetreat : public CAIStateGroup_WhisperRetreatLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_WhisperRetreat::GetName()
{
    return CCharString((const char*)0x0128A910, -1);   // "SG_WHISPER_RETREAT"
}
