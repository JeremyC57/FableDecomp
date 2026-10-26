// CAIStateGroup_EatAtHome::GetName() -- static: the AI state group name "SG_EAT_AT_HOME" as a CCharString
// Leaf: ?GetName@CAIStateGroup_EatAtHome@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE5CA: push esi; push -1; push "SG_EAT_AT_HOME"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221474 registers this GetName with Alloc 0x902e99 (pushed before it); Alloc 0x902e99 stores the vtable of CAIStateGroup_EatAtHome into the new object; literal "SG_EAT_AT_HOME"; PDB name set ?GetName@CAIStateGroup_EatAtHome@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_EatAtHome@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe5ca::build).
#define CAIStateGroup_EatAtHome CAIStateGroup_EatAtHomeLayout
#include "engine/CAIStateGroup_EatAtHome.h"
#undef CAIStateGroup_EatAtHome

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_EatAtHome : public CAIStateGroup_EatAtHomeLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_EatAtHome::GetName()
{
    return CCharString((const char*)0x0128A9DC, -1);   // "SG_EAT_AT_HOME"
}
