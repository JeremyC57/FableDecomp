// CAIStateGroup_ServeFoodAtHome::GetName() -- static: the AI state group name "SG_SERVE_FOOD_AT_HOME" as a CCharString
// Leaf: ?GetName@CAIStateGroup_ServeFoodAtHome@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FE603: push esi; push -1; push "SG_SERVE_FOOD_AT_HOME"; mov esi, ecx; call
//   CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax, esi; pop esi; ret -- a static __fastcall
//   returning CCharString by value (hidden result in ecx).
// Identity (namepass 2026-10-26): static init call 0x1221424 registers this GetName with Alloc 0x902cb8 (pushed before it); Alloc 0x902cb8 stores the vtable of CAIStateGroup_ServeFoodAtHome into the new object; literal "SG_SERVE_FOOD_AT_HOME"; PDB name set ?GetName@CAIStateGroup_ServeFoodAtHome@@SI?AVCCharString@@XZ and ?Alloc@CAIStateGroup_ServeFoodAtHome@@SIPAVCAIStateGroupBase@@XZ.
// Callee: CCharString(const char*, long) 0x0099EBF0 = PDB ??0CCharString@@QAE@PBDJ@Z (was the invented
//   FableCharStringResult_008fe603::build).
#define CAIStateGroup_ServeFoodAtHome CAIStateGroup_ServeFoodAtHomeLayout
#include "engine/CAIStateGroup_ServeFoodAtHome.h"
#undef CAIStateGroup_ServeFoodAtHome

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CAIStateGroup_ServeFoodAtHome : public CAIStateGroup_ServeFoodAtHomeLayout
{
public:
    static CCharString __fastcall GetName();
};

CCharString __fastcall CAIStateGroup_ServeFoodAtHome::GetName()
{
    return CCharString((const char*)0x0128AA00, -1);   // "SG_SERVE_FOOD_AT_HOME"
}
